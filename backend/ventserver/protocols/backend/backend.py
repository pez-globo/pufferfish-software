"""Sans-I/O backend service protocol."""

import logging
import typing
from typing import Optional, Union

import attr

from ventserver.protocols import events
from ventserver.protocols.backend import alarms, log, states
from ventserver.protocols.devices import frontend, mcu
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.sansio import channels
from ventserver.sansio import protocols


@attr.s
class ExternalLogEvent(events.Event):
    """External alarm input event.

    The active flag must be set to True or False for alarms, while it should be
    set to None for non-alarm events.
    """

    wall_time: float = attr.ib()
    monotonic_time: float = attr.ib()
    code: mcu_pb.LogEventCode = attr.ib()
    active: Optional[bool] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class AlarmMuteCancellationEvent(events.Event):
    """External alarm mute cancellation event.

    This should only be used to temporarily cancel any mute in the absence of
    the firmware, until the firmware reconnects.
    """

    wall_time: float = attr.ib()
    monotonic_time: float = attr.ib()
    source: mcu_pb.AlarmMuteSource = attr.ib()

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class OutputEvent(events.Event):
    """Output event."""

    states_send: Optional[states.SendEvent] = attr.ib(default=None)
    sysclock_setting: Optional[float] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        if self.sysclock_setting is not None:
            return True

        if self.states_send is not None:
            return self.states_send.has_data()

        return False


ReceiveDataEvent = states.ReceiveEvent
ReceiveEvent = Union[
    states.ReceiveEvent, ExternalLogEvent, AlarmMuteCancellationEvent
]
SendEvent = states.SendEvent


LOG_EVENT_TYPES = {
    mcu_pb.LogEventCode.backend_mcu_connection_down:
        mcu_pb.LogEventType.system,
    mcu_pb.LogEventCode.backend_frontend_connection_down:
        mcu_pb.LogEventType.system,
    mcu_pb.LogEventCode.backend_frontend_connection_up:
        mcu_pb.LogEventType.system
}


# Filters


@attr.s
class Receiver(protocols.Filter[ReceiveEvent, OutputEvent]):
    """Filter which passes input data in an event class."""

    _logger = logging.getLogger('.'.join((__name__, 'Receiver')))

    _buffer: channels.DequeChannel[ReceiveEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    wall_time: float = attr.ib(default=0)
    monotonic_time: float = attr.ib(default=0)
    store: states.Store = attr.ib()

    # State Synchronizers
    _state_synchronizers: states.Synchronizers = attr.ib()

    # Events Log
    _event_log_receiver: log.EventLogReceiver = attr.ib(
        factory=log.EventLogReceiver
    )
    _event_log_sender: log.EventLogSender = attr.ib(factory=log.EventLogSender)
    _local_alarms: alarms.Manager = attr.ib(factory=alarms.Manager)

    @store.default
    def init_store(self) -> states.Store:  # pylint: disable=no-self-use
        """Initialize the synchronizable states.

        Each pair consists of the type class to specify the states, and an
        actual object to store the state values.
        """
        return {type: None for type in states.StateSegment}

    @_state_synchronizers.default
    def init_state_synchronizers(self) -> \
            states.Synchronizers:  # pylint: disable=no-self-use
        """Initialize the mcu state synchronizer."""
        return states.Synchronizers(store=self.store)

    def input(self, event: Optional[ReceiveEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[OutputEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if event is None:
            return None

        output_event = OutputEvent()

        # Process input event
        self._update_clock(event)
        if isinstance(event, states.ReceiveEvent):
            self._state_synchronizers.input(event)
        elif isinstance(event, ExternalLogEvent):
            self._handle_local_log_event(event)
        elif isinstance(event, AlarmMuteCancellationEvent):
            self._handle_alarm_mute_cancellation(event)

        # Run internal services
        output_event.sysclock_setting = self._handle_system_settings()

        # Maintain internal data connections
        self._handle_mcu_log_events()
        self._handle_log_events_receiving()
        self._handle_log_events_sending()

        # Output any scheduled outbound state update
        output_event.states_send = self._state_synchronizers.output()

        # Produce output
        # Note: we only want to return None when there are no inputs to
        # process; if we processed an input but our output_event does not have
        # any data inside it, we might still have more inputs to process (in
        # which case we should not return None, since None signals that there is
        # no more filter processing to do until the next time an input is
        # passed into the filter).
        return output_event

    def _update_clock(self, event: ReceiveEvent) -> None:
        """Handle any clock update."""
        if event.wall_time is None:
            return

        self.wall_time = event.wall_time
        # The backend receive filter doesn't need to know about the event's
        # monotonic time - monotonic time is only needed by
        # self._state_synchronizers, and the backend receive filter forwards
        # the event to self._state_synchronizers anyways.

    def _handle_local_log_event(self, event: ExternalLogEvent) -> None:
        """Handle any locally-generated log events.

        This includes active log events (active alarms).
        """
        try:
            event_type = LOG_EVENT_TYPES[event.code]
        except KeyError:
            self._logger.error('Unrecognized local event type %s', event.code)
            return

        if event.active is None:
            self._local_alarms.input(log.LocalLogInputEvent(
                wall_time=self.wall_time, new_event=mcu_pb.LogEvent(
                    code=event.code, type=event_type
                )
            ))
        elif event.active:
            self._local_alarms.input(alarms.AlarmActivationEvent(
                wall_time=self.wall_time,
                monotonic_time=self.monotonic_time,
                code=event.code, event_type=event_type
            ))
        else:
            self._local_alarms.input(alarms.AlarmDeactivationEvent(
                wall_time=self.wall_time,
                monotonic_time=self.monotonic_time,
                codes=[event.code]
            ))
        self._event_log_receiver.input(self._local_alarms.output())

    def _handle_mcu_log_events(self) -> None:
        """Handle any updates to log events from the MCU.

        This includes active log events (active alarms).
        """
        next_log_events = typing.cast(
            Optional[mcu_pb.NextLogEvents],
            self.store[states.StateSegment.NEXT_LOG_EVENTS_MCU]
        )
        active_log_events = typing.cast(
            Optional[mcu_pb.ActiveLogEvents],
            self.store[states.StateSegment.ACTIVE_LOG_EVENTS_MCU]
        )
        self._event_log_receiver.input(log.ReceiveInputEvent(
            source=log.EventSource.MCU, wall_time=self.wall_time,
            next_log_events=next_log_events, active_log_events=active_log_events
        ))

    def _handle_log_events_receiving(self) -> None:
        """Handle any received log events which need to be routed.

        This includes active log events (active alarms).
        """
        while True:
            output_event = self._event_log_receiver.output()
            if output_event is None or not output_event.has_data():
                return

            if output_event.expected_log_event is not None:
                self.store[
                    states.StateSegment.EXPECTED_LOG_EVENT_MCU
                ] = output_event.expected_log_event
            # Pass new log events to sender
            expected_log_event = typing.cast(
                Optional[mcu_pb.ExpectedLogEvent],
                self.store[states.StateSegment.EXPECTED_LOG_EVENT_BE]
            )
            self._event_log_sender.input(log.SendInputEvent(
                new_log_events=output_event.new_elements,
                # trigger sender to produce output by giving expected log event
                expected_log_event=expected_log_event
            ))
            # Update active log events
            if output_event.active_log_events is not None:
                self.store[states.StateSegment.ACTIVE_LOG_EVENTS_BE] = \
                    output_event.active_log_events
            # TODO: pass new elements to a log events sender for saving to disk

    def _handle_log_events_sending(self) -> None:
        """Handle any updates to log events for the frontend."""
        expected_log_event = typing.cast(
            Optional[mcu_pb.ExpectedLogEvent],
            self.store[states.StateSegment.EXPECTED_LOG_EVENT_BE]
        )
        self._event_log_sender.input(log.SendInputEvent(
            expected_log_event=expected_log_event
        ))
        while True:
            next_log_events = self._event_log_sender.output()
            if next_log_events is None:
                return

            self.store[states.StateSegment.NEXT_LOG_EVENTS_BE] = next_log_events

    def _handle_alarm_mute_cancellation(
            self, event: AlarmMuteCancellationEvent
    ) -> None:
        """Handle any externally-generated alarm mute cancellation event."""
        alarm_mute = typing.cast(
            Optional[mcu_pb.AlarmMute],
            self.store[states.StateSegment.ALARM_MUTE]
        )
        if alarm_mute is None:
            self._logger.error('AlarmMute was not initialized in the store!')
            return

        if event.source == mcu_pb.AlarmMuteSource.backend_mcu_loss:
            log_event_code = mcu_pb.LogEventCode.alarms_unmuted_backend_mcu_loss
        else:
            self._logger.error(
                'Unexpected alarm mute cancellation source %s!', event.source
            )

        if alarm_mute.active:
            alarm_mute.active = False
            alarm_mute.source = event.source
            self._local_alarms.input(log.LocalLogInputEvent(
                wall_time=self.wall_time, new_event=mcu_pb.LogEvent(
                    code=log_event_code,
                    type=mcu_pb.LogEventType.system
                )
            ))
            self._event_log_receiver.input(self._local_alarms.output())

    def _handle_system_settings(self) -> Optional[float]:
        """Run the SystemSettings request/response service.

        Returns the new system (wall) clock time to set on the system, if it
        should be changed.
        """
        request = typing.cast(
            Optional[frontend_pb.SystemSettingsRequest],
            self.store[states.StateSegment.SYSTEM_SETTINGS_REQUEST]
        )
        if request is None:
            return None

        response = typing.cast(
            Optional[frontend_pb.SystemSettings],
            self.store[states.StateSegment.SYSTEM_SETTINGS]
        )
        if response is None:
            self._logger.error(
                'SystemSettings was not initialized in the store!'
            )
            return None

        if request.seq_num != (response.seq_num + 1) % (2 ** 32):
            return None

        response.display_brightness = request.display_brightness
        if int(request.date) != int(self.wall_time):  # rounds to nearest second
            self._local_alarms.input(log.LocalLogInputEvent(
                wall_time=self.wall_time, new_event=mcu_pb.LogEvent(
                    code=mcu_pb.LogEventCode.sysclock_changed,
                    type=mcu_pb.LogEventType.system,
                    old_float=self.wall_time,
                    new_float=request.date
                )
            ))
            self._event_log_receiver.input(self._local_alarms.output())
            # TODO: reset clock synchronization in the log manager
        response.date = request.date
        response.seq_num = request.seq_num

        return response.date


# Protocols


def get_mcu_send(
        backend_output: Optional[SendEvent]
) -> Optional[mcu.UpperEvent]:
    """Convert a OutputEvent to an MCUUpperEvent."""
    if backend_output is None:
        return None

    mcu_send = backend_output.mcu_send
    if mcu_send is None:
        return None

    return mcu_send


def get_frontend_send(
        backend_output: Optional[SendEvent]
) -> Optional[frontend.UpperEvent]:
    """Convert a OutputEvent to an FrontendUpperEvent."""
    if backend_output is None:
        return None

    frontend_send = backend_output.frontend_send
    if frontend_send is None:
        return None

    return frontend_send


def get_file_send(
        backend_output: Optional[SendEvent]
) -> Optional[mcu.UpperEvent]:
    """Convert a OutputEvent to an MCUUpperEvent."""
    if backend_output is None:
        return None

    file_send = backend_output.file_send
    if file_send is None:
        return None

    return file_send
