"""Sans-I/O backend service protocol."""

import collections
import enum
import logging
import typing
from typing import Dict, Mapping, Optional, Type

import attr

import betterproto

from ventserver.protocols import events
from ventserver.protocols import exceptions
from ventserver.protocols import frontend
from ventserver.protocols import log
from ventserver.protocols import mcu
from ventserver.protocols.application import states
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.sansio import channels
from ventserver.sansio import protocols


@enum.unique
class StateSegment(enum.Enum):
    """Enum for addressing state segments in the state store."""
    SENSOR_MEASUREMENTS = enum.auto()
    CYCLE_MEASUREMENTS = enum.auto()
    PARAMETERS = enum.auto()
    PARAMETERS_REQUEST = enum.auto()
    ALARM_LIMITS = enum.auto()
    ALARM_LIMITS_REQUEST = enum.auto()
    EXPECTED_LOG_EVENT_MCU = enum.auto()
    NEXT_LOG_EVENTS_MCU = enum.auto()
    ACTIVE_LOG_EVENTS_MCU = enum.auto()
    EXPECTED_LOG_EVENT_BE = enum.auto()
    NEXT_LOG_EVENTS_BE = enum.auto()
    ACTIVE_LOG_EVENTS_BE = enum.auto()
    ROTARY_ENCODER = enum.auto()


MCU_INPUT_TYPES: Mapping[Type[betterproto.Message], StateSegment] = {
    mcu_pb.SensorMeasurements: StateSegment.SENSOR_MEASUREMENTS,
    mcu_pb.CycleMeasurements: StateSegment.CYCLE_MEASUREMENTS,
    mcu_pb.Parameters: StateSegment.PARAMETERS,
    mcu_pb.AlarmLimits: StateSegment.ALARM_LIMITS,
    mcu_pb.NextLogEvents: StateSegment.NEXT_LOG_EVENTS_MCU,
    mcu_pb.ActiveLogEvents: StateSegment.ACTIVE_LOG_EVENTS_MCU,
}
MCU_OUTPUT_SCHEDULE = collections.deque([
    states.ScheduleEntry(time=0.02, type=StateSegment.PARAMETERS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
    states.ScheduleEntry(time=0.02, type=StateSegment.ALARM_LIMITS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
])

FRONTEND_INPUT_TYPES: Mapping[Type[betterproto.Message], StateSegment] = {
    mcu_pb.ParametersRequest: StateSegment.PARAMETERS_REQUEST,
    mcu_pb.AlarmLimitsRequest: StateSegment.ALARM_LIMITS_REQUEST,
    mcu_pb.ExpectedLogEvent: StateSegment.EXPECTED_LOG_EVENT_BE,
    frontend_pb.RotaryEncoder: StateSegment.ROTARY_ENCODER
}
FRONTEND_OUTPUT_SCHEDULE = collections.deque([
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.PARAMETERS),
    states.ScheduleEntry(time=0.01, type=StateSegment.PARAMETERS_REQUEST),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.ALARM_LIMITS),
    states.ScheduleEntry(time=0.01, type=StateSegment.ALARM_LIMITS_REQUEST),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.NEXT_LOG_EVENTS_BE),
    states.ScheduleEntry(time=0.01, type=StateSegment.ACTIVE_LOG_EVENTS_BE),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.ROTARY_ENCODER),
    states.ScheduleEntry(time=0.01, type=StateSegment.CYCLE_MEASUREMENTS),
])

FILE_INPUT_TYPES: Mapping[Type[betterproto.Message], StateSegment] = {
    mcu_pb.Parameters: StateSegment.PARAMETERS,
    mcu_pb.ParametersRequest: StateSegment.PARAMETERS_REQUEST,
    mcu_pb.AlarmLimits: StateSegment.ALARM_LIMITS,
    mcu_pb.AlarmLimitsRequest: StateSegment.ALARM_LIMITS_REQUEST,
}
FILE_OUTPUT_SCHEDULE = collections.deque([
    states.ScheduleEntry(time=0.3, type=StateSegment.PARAMETERS),
    states.ScheduleEntry(time=0.3, type=StateSegment.PARAMETERS_REQUEST),
    states.ScheduleEntry(time=0.3, type=StateSegment.ALARM_LIMITS),
    states.ScheduleEntry(time=0.3, type=StateSegment.ALARM_LIMITS_REQUEST),
])


# Events


@attr.s
class ReceiveEvent(events.Event):
    """Backend receive input event."""

    time: Optional[float] = attr.ib(default=None)
    mcu_receive: Optional[mcu.UpperEvent] = attr.ib(default=None)
    frontend_receive: Optional[frontend.UpperEvent] = attr.ib(default=None)
    file_receive: Optional[mcu.UpperEvent] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.time is not None
            or self.mcu_receive is not None
            or self.frontend_receive is not None
        )


@attr.s
class OutputEvent(events.Event):
    """Backend receive output/send output/send input event."""

    mcu_send: Optional[mcu.UpperEvent] = attr.ib(default=None)
    frontend_send: Optional[frontend.UpperEvent] = attr.ib(default=None)
    file_send: Optional[frontend.UpperEvent] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.mcu_send is not None or self.frontend_send is not None


SendEvent = OutputEvent


# Filters


@attr.s
class StateSynchronizers(protocols.Filter[ReceiveEvent, OutputEvent]):
    """Helper class for updating the backend's state synchronizers.

    Warning: if all_states is used by other things, this filter is only safe to
    use in synchronous environments, such as part of another Filter which
    completely owns all_states. However, if access to all_states is only done
    through the input and output methods, then this Filter is safe to use in
    concurrent environments.
    """

    _logger = logging.getLogger('.'.join((__name__, 'StateSynchronizers')))

    all_states: Dict[StateSegment, Optional[betterproto.Message]] = attr.ib()

    # State sending synchronizers
    _mcu: states.Synchronizer[StateSegment] = attr.ib()
    _frontend: states.Synchronizer[StateSegment] = attr.ib()
    _file: states.Synchronizer[StateSegment] = attr.ib()

    @_mcu.default
    def init_mcu(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the mcu state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=MCU_OUTPUT_SCHEDULE
        )

    @_frontend.default
    def init_frontend(self) -> \
            states.Synchronizer[StateSegment]:
        """Initialize the frontend state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=FRONTEND_OUTPUT_SCHEDULE
        )

    @_file.default
    def init_file(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the file state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=FILE_OUTPUT_SCHEDULE
        )

    def input(self, event: Optional[ReceiveEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        # Update synchronizer clocks
        clock_update_event: states.UpdateEvent[StateSegment] = \
            states.UpdateEvent(time=event.time)
        self._mcu.input(clock_update_event)
        self._frontend.input(clock_update_event)
        self._file.input(clock_update_event)

        # Handle inbound state segments
        # We directly input states into all_states, instead of passing them in
        # through the StateSynchronizer objects; we're only using those to
        # generate outputs.
        self._handle_inbound_state(event.mcu_receive, MCU_INPUT_TYPES)
        self._handle_inbound_state(event.file_receive, FILE_INPUT_TYPES)
        self._handle_inbound_state(event.frontend_receive, FRONTEND_INPUT_TYPES)

    def output(self) -> Optional[OutputEvent]:
        """Emit the next output event."""
        output_event = OutputEvent()
        try:
            output_event.mcu_send = self._mcu.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('MCU State Synchronizer:')
        try:
            output_event.frontend_send = self._frontend.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('Frontend State Synchronizer:')
        try:
            output_event.file_send = self._file.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('File State Synchronizer:')
        return output_event

    def _handle_inbound_state(
            self, segment: Optional[betterproto.Message],
            input_types: Mapping[Type[betterproto.Message], StateSegment]
    ) -> None:
        """Handle an inbound state update."""
        if segment is None:
            return

        if type(segment) not in input_types:
            return

        segment_type = input_types[type(segment)]
        self._logger.debug('Received: %s', segment)
        try:
            self.all_states[segment_type] = segment
        except KeyError:
            self._logger.exception(
                'Received state segment type is not a valid state: %s',
                segment_type
            )


@attr.s
class ReceiveFilter(protocols.Filter[ReceiveEvent, OutputEvent]):
    """Filter which passes input data in an event class."""

    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    _buffer: channels.DequeChannel[ReceiveEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    current_time: float = attr.ib(default=0)
    all_states: Dict[StateSegment, Optional[betterproto.Message]] = attr.ib()

    # State Synchronizers
    _state_synchronizers: StateSynchronizers = attr.ib()

    # Events Log
    _event_log_receiver: log.EventLogReceiver = attr.ib(
        factory=log.EventLogReceiver
    )
    _event_log_sender: log.EventLogSender = attr.ib(factory=log.EventLogSender)

    @all_states.default
    def init_all_states(self) -> Dict[
            StateSegment, Optional[betterproto.Message]
    ]:  # pylint: disable=no-self-use
        """Initialize the synchronizable states.

        Each pair consists of the type class to specify the states, and an
        actual object to store the state values.
        """
        return {type: None for type in StateSegment}

    @_state_synchronizers.default
    def init_state_synchronizers(self) -> \
            StateSynchronizers:  # pylint: disable=no-self-use
        """Initialize the mcu state synchronizer."""
        return StateSynchronizers(all_states=self.all_states)

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
        self._state_synchronizers.input(event)

        # Maintain internal data connections
        self._handle_log_events_receiving()
        self._handle_log_events_sending()

        # Output any scheduled outbound state update
        return self._state_synchronizers.output()

    def _update_clock(self, event: ReceiveEvent) -> None:
        """Handle any clock update."""
        if event.time is not None:
            self.current_time = event.time

    def _handle_log_events_receiving(self) -> None:
        """Handle any updates to log events from the MCU.

        This includes active log events (active alarms).
        """
        next_log_events = typing.cast(
            Optional[mcu_pb.NextLogEvents],
            self.all_states[StateSegment.NEXT_LOG_EVENTS_MCU]
        )
        active_log_events = typing.cast(
            Optional[mcu_pb.ActiveLogEvents],
            self.all_states[StateSegment.ACTIVE_LOG_EVENTS_MCU]
        )
        self._event_log_receiver.input(log.ReceiveInputEvent(
            current_time=self.current_time, next_log_events=next_log_events,
            active_log_events=active_log_events
        ))
        while True:
            output_event = self._event_log_receiver.output()
            if output_event is None or not output_event.has_data():
                return

            if output_event.expected_log_event is not None:
                self.all_states[
                    StateSegment.EXPECTED_LOG_EVENT_MCU
                ] = output_event.expected_log_event
            # Pass new log events to sender
            expected_log_event = typing.cast(
                Optional[mcu_pb.ExpectedLogEvent],
                self.all_states[StateSegment.EXPECTED_LOG_EVENT_BE]
            )
            self._event_log_sender.input(log.SendInputEvent(
                new_log_events=output_event.new_elements,
                # trigger sender to produce output by giving expected log event
                expected_log_event=expected_log_event
            ))
            # Update active log events
            if output_event.active_log_events is not None:
                self.all_states[StateSegment.ACTIVE_LOG_EVENTS_BE] = \
                    output_event.active_log_events
            # TODO: pass new elements to a log events sender for saving to disk

    def _handle_log_events_sending(self) -> None:
        """Handle any updates to log events for the frontend."""
        expected_log_event = typing.cast(
            Optional[mcu_pb.ExpectedLogEvent],
            self.all_states[StateSegment.EXPECTED_LOG_EVENT_BE]
        )
        self._event_log_sender.input(log.SendInputEvent(
            expected_log_event=expected_log_event
        ))
        while True:
            next_log_events = self._event_log_sender.output()
            if next_log_events is None:
                return

            self.all_states[StateSegment.NEXT_LOG_EVENTS_BE] = next_log_events


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
