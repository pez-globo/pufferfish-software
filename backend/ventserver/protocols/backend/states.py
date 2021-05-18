"""Sans-I/O backend service state synchronization protocol."""

import collections
import enum
import logging
from typing import Dict, Mapping, Optional, Type

import attr

import betterproto

from ventserver.protocols import events, exceptions
from ventserver.protocols.application import states
from ventserver.protocols.devices import frontend, mcu
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.sansio import protocols


@enum.unique
class StateSegment(enum.Enum):
    """Enum for addressing state segments in the state store."""
    # mcu_pb
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
    ALARM_MUTE = enum.auto()
    ALARM_MUTE_REQUEST = enum.auto()
    BATTERY_POWER = enum.auto()
    SCREEN_STATUS = enum.auto()
    # frontend_pb
    ROTARY_ENCODER = enum.auto()
    SYSTEM_SETTING = enum.auto()
    SYSTEM_SETTING_REQUEST = enum.auto()
    FRONTEND_DISPLAY = enum.auto()
    FRONTEND_DISPLAY_REQUEST = enum.auto()


Store = Dict[StateSegment, Optional[betterproto.Message]]


MCU_INPUT_TYPES: Mapping[Type[betterproto.Message], StateSegment] = {
    mcu_pb.SensorMeasurements: StateSegment.SENSOR_MEASUREMENTS,
    mcu_pb.CycleMeasurements: StateSegment.CYCLE_MEASUREMENTS,
    mcu_pb.Parameters: StateSegment.PARAMETERS,
    mcu_pb.AlarmLimits: StateSegment.ALARM_LIMITS,
    mcu_pb.NextLogEvents: StateSegment.NEXT_LOG_EVENTS_MCU,
    mcu_pb.ActiveLogEvents: StateSegment.ACTIVE_LOG_EVENTS_MCU,
    mcu_pb.AlarmMute: StateSegment.ALARM_MUTE,
    mcu_pb.BatteryPower: StateSegment.BATTERY_POWER,
    mcu_pb.ScreenStatus: StateSegment.SCREEN_STATUS,
}
MCU_OUTPUT_SCHEDULE = collections.deque([
    states.ScheduleEntry(time=0.02, type=StateSegment.PARAMETERS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
    states.ScheduleEntry(time=0.02, type=StateSegment.ALARM_LIMITS_REQUEST),
    states.ScheduleEntry(time=0.02, type=StateSegment.EXPECTED_LOG_EVENT_MCU),
    states.ScheduleEntry(time=0.02, type=StateSegment.ALARM_MUTE_REQUEST),
])

FRONTEND_INPUT_TYPES: Mapping[Type[betterproto.Message], StateSegment] = {
    mcu_pb.ParametersRequest: StateSegment.PARAMETERS_REQUEST,
    mcu_pb.AlarmLimitsRequest: StateSegment.ALARM_LIMITS_REQUEST,
    mcu_pb.ExpectedLogEvent: StateSegment.EXPECTED_LOG_EVENT_BE,
    mcu_pb.AlarmMuteRequest: StateSegment.ALARM_MUTE_REQUEST,
    frontend_pb.RotaryEncoder: StateSegment.ROTARY_ENCODER,
    frontend_pb.SystemSettingRequest: StateSegment.SYSTEM_SETTING_REQUEST,
    # Frontend protobuf message isn't defined yet:
    # frontend_pb.FrontendDisplay: StateSegment.FRONTEND_DISPLAY_REQUEST,
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
    states.ScheduleEntry(time=0.01, type=StateSegment.ALARM_MUTE),
    states.ScheduleEntry(time=0.01, type=StateSegment.ALARM_MUTE_REQUEST),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.BATTERY_POWER),
    states.ScheduleEntry(time=0.01, type=StateSegment.SCREEN_STATUS),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.CYCLE_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.ROTARY_ENCODER),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.SYSTEM_SETTING),
    states.ScheduleEntry(time=0.01, type=StateSegment.SYSTEM_SETTING_REQUEST),
    states.ScheduleEntry(time=0.01, type=StateSegment.SENSOR_MEASUREMENTS),
    states.ScheduleEntry(time=0.01, type=StateSegment.FRONTEND_DISPLAY),
    states.ScheduleEntry(time=0.01, type=StateSegment.FRONTEND_DISPLAY_REQUEST),
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
    """Store synchronizers receive event."""

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
class SendEvent(events.Event):
    """Store synchronizers send event."""

    mcu_send: Optional[mcu.UpperEvent] = attr.ib(default=None)
    frontend_send: Optional[frontend.UpperEvent] = attr.ib(default=None)
    file_send: Optional[frontend.UpperEvent] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.mcu_send is not None or self.frontend_send is not None


# Filters


@attr.s
class Synchronizers(protocols.Filter[ReceiveEvent, SendEvent]):
    """Helper class for updating the backend's state synchronizers.

    Warning: if store is used by other things, this filter is only safe to
    use in synchronous environments, such as part of another Filter which
    completely owns store. However, if access to store is only done
    through the input and output methods, then this Filter is safe to use in
    concurrent environments.
    """

    _logger = logging.getLogger('.'.join((__name__, 'Synchronizers')))

    store: Store = attr.ib()

    # State sending synchronizers
    _mcu: states.Synchronizer[StateSegment] = attr.ib()
    _frontend: states.Synchronizer[StateSegment] = attr.ib()
    _file: states.Synchronizer[StateSegment] = attr.ib()

    @_mcu.default
    def init_mcu(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the mcu state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.store,
            output_schedule=MCU_OUTPUT_SCHEDULE
        )

    @_frontend.default
    def init_frontend(self) -> \
            states.Synchronizer[StateSegment]:
        """Initialize the frontend state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.store,
            output_schedule=FRONTEND_OUTPUT_SCHEDULE
        )

    @_file.default
    def init_file(self) -> \
            states.Synchronizer[StateSegment]:  # pylint: disable=no-self-use
        """Initialize the file state synchronizer."""
        return states.Synchronizer(
            segment_types=StateSegment, all_states=self.store,
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
        # We directly input states into store, instead of passing them in
        # through the StateSynchronizer objects; we're only using those to
        # generate outputs.
        self._handle_inbound_state(event.mcu_receive, MCU_INPUT_TYPES)
        self._handle_inbound_state(event.file_receive, FILE_INPUT_TYPES)
        self._handle_inbound_state(event.frontend_receive, FRONTEND_INPUT_TYPES)

    def output(self) -> Optional[SendEvent]:
        """Emit the next output event."""
        output_event = SendEvent()
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
            self.store[segment_type] = segment
        except KeyError:
            self._logger.exception(
                'Received state segment type is not a valid state: %s',
                segment_type
            )
