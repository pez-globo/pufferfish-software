"""Servicing of Parameters and ParametersRequest states."""

import time
import typing
from typing import Dict, Mapping, Optional, Type, Union

import attr

import betterproto

from ventserver.protocols.application import lists
from ventserver.protocols.protobuf import mcu_pb


# Simulators


@attr.s
class Manager:
    """Shared alarms manager."""

    current_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time

    next_log_event_id: int = attr.ib(default=0)
    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self.current_time = current_time * 1000 - self.initial_time

    def transform_active_log_event_ids(
            self, active_log_events: mcu_pb.ActiveLogEvents
    ) -> None:
        """Update the list of active log events."""
        active_log_events.id = list(self.active_alarm_ids.values())

    # Log Events

    def activate_alarm(
            self, code: mcu_pb.LogEventCode, value: float,
            lower_limit: int, upper_limit: int,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Create a new Log Event if it is not active."""
        if code in self.active_alarm_ids:
            return

        log_event = mcu_pb.LogEvent(
            id=self.next_log_event_id, time=int(self.current_time),
            code=code, new_value=value,
            alarm_limits=mcu_pb.Range(lower=lower_limit, upper=upper_limit)
        )
        log_events_sender.input(lists.UpdateEvent(new_element=log_event))
        self.active_alarm_ids[code] = self.next_log_event_id
        self.next_log_event_id += 1

    def deactivate_alarm(self, code: mcu_pb.LogEventCode) -> None:
        """Dectivate the Log Event if it's active."""
        if code not in self.active_alarm_ids:
            return

        self.active_alarm_ids.pop(code)


@attr.s
class Service:
    """Base class for a breathing circuit simulator."""

    ALARM_CODES = {
        mcu_pb.LogEventCode.spo2_too_low,
        mcu_pb.LogEventCode.spo2_too_high,
        mcu_pb.LogEventCode.fio2_too_low,
        mcu_pb.LogEventCode.fio2_too_high,
    }

    _manager: Manager = attr.ib(factory=Manager)

    # Log Events

    def transform(
            self, parameters: mcu_pb.Parameters,
            alarm_limits: mcu_pb.AlarmLimits,
            sensor_measurements: mcu_pb.SensorMeasurements,
            active_log_events: mcu_pb.ActiveLogEvents,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the simulation."""
        if not parameters.ventilating:
            self.deactivate_alarms(active_log_events)
            return

        self.transform_parameter_alarms(
            alarm_limits.spo2.lower, alarm_limits.spo2.upper,
            sensor_measurements.spo2,
            mcu_pb.LogEventCode.spo2_too_low,
            mcu_pb.LogEventCode.spo2_too_high,
            log_events_sender
        )
        self.transform_parameter_alarms(
            alarm_limits.fio2.lower, alarm_limits.fio2.upper,
            sensor_measurements.fio2,
            mcu_pb.LogEventCode.fio2_too_low,
            mcu_pb.LogEventCode.fio2_too_high,
            log_events_sender
        )
        self._manager.transform_active_log_event_ids(active_log_events)

    # Update methods

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self._manager.update_clock(current_time)

    def transform_parameter_alarms(
            self, lower_limit: int, upper_limit: int, value: Union[float, int],
            too_low_code: mcu_pb.LogEventCode,
            too_high_code: mcu_pb.LogEventCode,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the alarms for a particular parameter."""
        if value < lower_limit:
            self._manager.activate_alarm(
                too_low_code, value, lower_limit, upper_limit,
                log_events_sender
            )
        else:
            self._manager.deactivate_alarm(too_low_code)

        if value > upper_limit:
            self._manager.activate_alarm(
                too_high_code, value, lower_limit, upper_limit,
                log_events_sender
            )
        else:
            self._manager.deactivate_alarm(too_high_code)

    def deactivate_alarms(
            self, active_log_events: mcu_pb.ActiveLogEvents
    ) -> None:
        """Deactivate all alarms for all parameters."""
        for code in self.ALARM_CODES:
            self._manager.deactivate_alarm(code)
        self._manager.transform_active_log_event_ids(active_log_events)


@attr.s
class PCAC(Service):
    """Breathing circuit simulator in PC-AC mode."""


@attr.s
class HFNC(Service):
    """Breathing circuit simulator in HFNC mode."""


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _manager: Manager = attr.ib(factory=Manager)
    _active_service: Optional[Service] = attr.ib(default=None)
    _services: Dict[mcu_pb.VentilationMode, Service] = attr.ib()

    @_services.default
    def init_services(self) -> Dict[mcu_pb.VentilationMode, Service]:
        """Initialize the services."""
        return {
            mcu_pb.VentilationMode.pc_ac: PCAC(manager=self._manager),
            mcu_pb.VentilationMode.hfnc: HFNC(manager=self._manager)
        }

    def transform(
            self, current_time: float, all_states: Mapping[
                Type[betterproto.Message], Optional[betterproto.Message]
            ], log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the parameters for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, all_states[mcu_pb.Parameters]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        alarm_limits = typing.cast(
            mcu_pb.AlarmLimits, all_states[mcu_pb.AlarmLimits]
        )
        sensor_measurements = typing.cast(
            mcu_pb.SensorMeasurements,
            all_states[mcu_pb.SensorMeasurements]
        )
        active_log_events = typing.cast(
            mcu_pb.ActiveLogEvents, all_states[mcu_pb.ActiveLogEvents]
        )
        self._active_service.update_clock(current_time)
        self._active_service.transform(
            parameters, alarm_limits, sensor_measurements,
            active_log_events, log_events_sender
        )
