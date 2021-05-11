"""Sans-I/O backend service protocol."""

import logging
import typing
from typing import Optional, Union

import attr

from ventserver.protocols import events
from ventserver.protocols.backend import log, states
from ventserver.protocols.devices import frontend, mcu
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import channels
from ventserver.sansio import protocols


@attr.s
class ExternalAlarmEvent(events.Event):
    """External alarm input event."""

    time: float = attr.ib()
    active: bool = attr.ib()
    code: mcu_pb.LogEventCode = attr.ib()

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


ReceiveDataEvent = states.ReceiveEvent
ReceiveEvent = Union[ExternalAlarmEvent, states.ReceiveEvent]
OutputEvent = states.SendEvent
SendEvent = states.SendEvent


# Filters


@attr.s
class ReceiveFilter(protocols.Filter[ReceiveEvent, OutputEvent]):
    """Filter which passes input data in an event class."""

    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    _buffer: channels.DequeChannel[ReceiveEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    current_time: float = attr.ib(default=0)
    store: states.Store = attr.ib()

    # State Synchronizers
    _state_synchronizers: states.Synchronizers = attr.ib()

    # Events Log
    _event_log_receiver: log.EventLogReceiver = attr.ib(
        factory=log.EventLogReceiver
    )
    _event_log_sender: log.EventLogSender = attr.ib(factory=log.EventLogSender)
    local_log_source: log.LocalLogSource = attr.ib(factory=log.LocalLogSource)

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

        # Process input event
        self._update_clock(event)
        if isinstance(event, states.ReceiveEvent):
            self._state_synchronizers.input(event)
        elif isinstance(event, ExternalAlarmEvent):
            pass
            # print('ALARM UPDATE RECEIVED:', event)

        # Maintain internal data connections
        self._handle_log_events_receiving()
        self._handle_log_events_sending()

        # Output any scheduled outbound state update
        return self._state_synchronizers.output()

    def _update_clock(self, event: ReceiveEvent) -> None:
        """Handle any clock update."""
        if event.time is None:
            return

        self.current_time = event.time
        self.local_log_source.input(log.LocalLogInputEvent(
            current_time=self.current_time
        ))

    def _handle_log_events_receiving(self) -> None:
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
        self._event_log_receiver.input(log.ReceiveRemoteInputEvent(
            current_time=self.current_time, next_log_events=next_log_events,
            active_log_events=active_log_events
        ))
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


@attr.s
class SendFilter(protocols.Filter[SendEvent, OutputEvent]):
    """Filter which unwraps output data from an event class."""

    _buffer: channels.DequeChannel[SendEvent] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[SendEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[OutputEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if event is None:
            return None

        if isinstance(event, OutputEvent):
            return event

        return None


# Protocols


def get_mcu_send(
        backend_output: Optional[OutputEvent]
) -> Optional[mcu.UpperEvent]:
    """Convert a OutputEvent to an MCUUpperEvent."""
    if backend_output is None:
        return None
    if backend_output.mcu_send is None:
        return None

    return backend_output.mcu_send


def get_frontend_send(
        backend_output: Optional[OutputEvent]
) -> Optional[frontend.UpperEvent]:
    """Convert a OutputEvent to an FrontendUpperEvent."""
    if backend_output is None:
        return None
    if backend_output.frontend_send is None:
        return None

    return backend_output.frontend_send


def get_file_send(
        backend_output: Optional[OutputEvent]
) -> Optional[mcu.UpperEvent]:
    """Convert a OutputEvent to an MCUUpperEvent."""
    if backend_output is None:
        return None
    if backend_output.file_send is None:
        return None

    return backend_output.file_send
