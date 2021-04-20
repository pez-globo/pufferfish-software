"""Sans-I/O backend service protocol."""

import collections
import dataclasses
import enum
import logging
import typing
from typing import Dict, Optional, Union

import attr

import betterproto

from ventserver.protocols import events
from ventserver.protocols import exceptions
from ventserver.protocols import frontend
from ventserver.protocols import mcu
from ventserver.protocols.application import lists, states
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


MCU_SYNCHRONIZER_SCHEDULE = collections.deque([
    states.ScheduleEntry(time=0.02, type=StateSegment.PARAMETERS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
    states.ScheduleEntry(time=0.02, type=StateSegment.ALARM_LIMITS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
])

FRONTEND_SYNCHRONIZER_SCHEDULE = collections.deque([
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

FILE_SYNCHRONIZER_SCHEDULE = collections.deque([
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


@attr.s
class Announcement(events.Event):
    """Backend send input event."""

    message: bytes = attr.ib(default=b'ping')

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


SendEvent = Union[Announcement, OutputEvent]


# Filters


@attr.s
class ReceiveFilter(protocols.Filter[ReceiveEvent, OutputEvent]):
    """Filter which passes input data in an event class."""

    MCU_INPUT_TYPES = {
        mcu_pb.SensorMeasurements: StateSegment.SENSOR_MEASUREMENTS,
        mcu_pb.CycleMeasurements: StateSegment.CYCLE_MEASUREMENTS,
        mcu_pb.Parameters: StateSegment.PARAMETERS,
        mcu_pb.AlarmLimits: StateSegment.ALARM_LIMITS,
        mcu_pb.NextLogEvents: StateSegment.NEXT_LOG_EVENTS_MCU,
        mcu_pb.ActiveLogEvents: StateSegment.ACTIVE_LOG_EVENTS_MCU,
    }
    FRONTEND_INPUT_TYPES = {
        mcu_pb.ParametersRequest: StateSegment.PARAMETERS_REQUEST,
        mcu_pb.AlarmLimitsRequest: StateSegment.ALARM_LIMITS_REQUEST,
        mcu_pb.ExpectedLogEvent: StateSegment.EXPECTED_LOG_EVENT_BE,
        frontend_pb.RotaryEncoder: StateSegment.ROTARY_ENCODER
    }
    FILE_INPUT_TYPES = {
        mcu_pb.Parameters: StateSegment.PARAMETERS,
        mcu_pb.ParametersRequest: StateSegment.PARAMETERS_REQUEST,
        mcu_pb.AlarmLimits: StateSegment.ALARM_LIMITS,
        mcu_pb.AlarmLimitsRequest: StateSegment.ALARM_LIMITS_REQUEST,
    }

    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    _buffer: channels.DequeChannel[ReceiveEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    current_time: float = attr.ib(default=0)
    all_states: Dict[StateSegment, Optional[betterproto.Message]] = attr.ib()

    # State Synchronizers
    _mcu_state_synchronizer: states.Synchronizer[StateSegment] = attr.ib()
    _frontend_state_synchronizer: states.Synchronizer[StateSegment] = attr.ib()
    _file_state_synchronizer: states.Synchronizer[StateSegment] = attr.ib()

    # Events Log
    log_events_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent] = attr.ib()
    _log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent] = attr.ib()
    _events_log_next_id: int = attr.ib(default=0)

    # Time Synchronization
    _start_time_mcu: Optional[int] = attr.ib(default=None)
    _start_time_backend: Optional[float] = attr.ib(default=None)

    @all_states.default
    def init_all_states(self) -> Dict[
            StateSegment, Optional[betterproto.Message]
    ]:  # pylint: disable=no-self-use
        """Initialize the synchronizable states.

        Each pair consists of the type class to specify the states, and an
        actual object to store the state values.
        """
        return {type: None for type in StateSegment}

    @_mcu_state_synchronizer.default
    def init_mcu_synchronizer(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the mcu state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=MCU_SYNCHRONIZER_SCHEDULE
        )

    @_frontend_state_synchronizer.default
    def init_frontend_synchronizer(self) -> \
            states.Synchronizer[StateSegment]:
        """Initialize the frontend state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=FRONTEND_SYNCHRONIZER_SCHEDULE
        )

    @_file_state_synchronizer.default
    def init_file_synchronizer(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the file state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.all_states,
            output_schedule=FILE_SYNCHRONIZER_SCHEDULE
        )

    @log_events_receiver.default
    def init_log_events_list_receiver(self) -> lists.ReceiveSynchronizer[
            mcu_pb.LogEvent
    ]:   # pylint: disable=no-self-use
        """Initialize the frontend log events list sender."""
        return lists.ReceiveSynchronizer()

    @_log_events_sender.default
    def init_log_events_list_sender(self) -> lists.SendSynchronizer[
            mcu_pb.LogEvent
    ]:   # pylint: disable=no-self-use
        """Initialize the frontend log events list sender."""
        return lists.SendSynchronizer(segment_type=mcu_pb.NextLogEvents)

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
        self._handle_mcu_inbound_state(event)
        self._handle_file_inbound_state(event)
        self._handle_frontend_inbound_state(event)

        # Maintain internal data connections
        self._handle_log_events_receiving()
        self._handle_log_events_sending()

        # Output any scheduled outbound state update
        mcu_send = None
        try:
            mcu_send = self._mcu_state_synchronizer.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('MCU State Synchronizer:')
        frontend_send = None
        try:
            frontend_send = self._frontend_state_synchronizer.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('Frontend State Synchronizer:')
        file_send = None
        try:
            file_send = self._file_state_synchronizer.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('File State Synchronizer:')
        return OutputEvent(
            mcu_send=mcu_send,
            frontend_send=frontend_send,
            file_send=file_send
        )

    def _update_clock(self, event: ReceiveEvent) -> None:
        """Handle any clock update."""
        if event.time is not None:
            self.current_time = event.time
        self._mcu_state_synchronizer.input(states.UpdateEvent(
            time=self.current_time
        ))
        self._frontend_state_synchronizer.input(states.UpdateEvent(
            time=self.current_time
        ))
        self._file_state_synchronizer.input(states.UpdateEvent(
            time=self.current_time
        ))

    def _handle_mcu_inbound_state(self, event: ReceiveEvent) -> None:
        """Handle any inbound state update from the MCU."""
        if (
                event.mcu_receive is None
                or type(event.mcu_receive) not in self.MCU_INPUT_TYPES
        ):
            return

        try:
            self._mcu_state_synchronizer.input(states.UpdateEvent(
                pb_message=event.mcu_receive,
                segment_type=self.MCU_INPUT_TYPES[type(event.mcu_receive)]
            ))
        except exceptions.ProtocolDataError:
            self._logger.exception(
                'MCU State Synchronizer: %s', event.mcu_receive
            )

        try:
            self._file_state_synchronizer.input(states.UpdateEvent(
                pb_message=event.mcu_receive,
                segment_type=self.MCU_INPUT_TYPES[type(event.mcu_receive)]
            ))
        except exceptions.ProtocolDataError:
            self._logger.exception(
                'File Save State Synchronizer Save: %s', event.mcu_receive
            )

    def _handle_file_inbound_state(self, event: ReceiveEvent) -> None:
        """Handle any inbound state update from the filesystem."""
        if (
                event.file_receive is None
                or type(event.file_receive) not in self.MCU_INPUT_TYPES
        ):
            return

        try:
            self._file_state_synchronizer.input(states.UpdateEvent(
                pb_message=event.file_receive,
                segment_type=self.FILE_INPUT_TYPES[type(event.file_receive)]
            ))
        except exceptions.ProtocolDataError:
            self._logger.exception(
                'File State Synchronizer Read: %s', event.file_receive
            )

    def _handle_frontend_inbound_state(self, event: ReceiveEvent) -> None:
        """Handle any inbound state update from the frontend."""
        if (
                event.frontend_receive is None
                or type(event.frontend_receive) not in self.FRONTEND_INPUT_TYPES
        ):
            return

        try:
            self._frontend_state_synchronizer.input(states.UpdateEvent(
                pb_message=event.frontend_receive,
                segment_type=self.FRONTEND_INPUT_TYPES[
                    type(event.frontend_receive)
                ]
            ))
        except exceptions.ProtocolDataError:
            self._logger.exception(
                'Frontend State Synchronizer: %s', event.frontend_receive
            )

    def _handle_log_events_receiving(self) -> None:
        """Handle any updates to log events list receiving."""
        # TODO: decompose event log sync into a separate class in this module
        next_log_events = typing.cast(
            mcu_pb.NextLogEvents,
            self.all_states[StateSegment.NEXT_LOG_EVENTS_MCU]
        )
        self.log_events_receiver.input(next_log_events)
        update_event = self.log_events_receiver.output()
        if update_event is None:
            return

        if update_event.next_expected is not None:
            self.all_states[
                StateSegment.EXPECTED_LOG_EVENT_MCU
            ] = mcu_pb.ExpectedLogEvent(id=update_event.next_expected)
        new_elements = []
        for element in update_event.new_elements:
            new_element = dataclasses.replace(element)
            new_element.id = self._events_log_next_id
            self._events_log_next_id += 1
            # TODO: decompose time sync into a separate class in this module
            if self._start_time_mcu is None:
                self._start_time_mcu = new_element.time
                self._start_time_backend = self.current_time
            if self._start_time_backend is None:
                raise exceptions.ProtocolDataError(
                    'Backend protocol time was not initialized!'
                )
            new_element.time = int(
                new_element.time - self._start_time_mcu
                + self._start_time_backend * 1000
            )
            new_elements.append(new_element)
        self._log_events_sender.input(lists.UpdateEvent(
            new_elements=new_elements
        ))
        # TODO: mark events for saving to disk

    def _handle_log_events_sending(self) -> None:
        """Handle any updates to log events list sending."""
        expected_log_event = typing.cast(
            mcu_pb.ExpectedLogEvent,
            self.all_states[StateSegment.EXPECTED_LOG_EVENT_BE]
        )
        if expected_log_event is not None:
            self._log_events_sender.input(lists.UpdateEvent(
                next_expected=expected_log_event.id  # input needed for output
            ))
        next_log_events = self._log_events_sender.output()
        if next_log_events is None:
            return

        assert isinstance(next_log_events, mcu_pb.NextLogEvents)
        self.all_states[StateSegment.NEXT_LOG_EVENTS_BE] = next_log_events
        # TODO: translate MCU event IDs to backend event IDs
        self.all_states[StateSegment.ACTIVE_LOG_EVENTS_BE] = \
            self.all_states[StateSegment.ACTIVE_LOG_EVENTS_MCU]


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

        if isinstance(event, Announcement):
            message = mcu_pb.Announcement(announcement=event.message)
            return OutputEvent(mcu_send=message, frontend_send=message)

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
