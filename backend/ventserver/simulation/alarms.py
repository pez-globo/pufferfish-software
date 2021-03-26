"""Servicing of Parameters and ParametersRequest states."""

import typing
from typing import Dict, Mapping, Optional, Type, Union

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb
from ventserver.simulation import log


# Simulators


@attr.s
class Manager:
    """Shared alarms manager."""

    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def transform_active_log_event_ids(
            self, active_log_events: mcu_pb.ActiveLogEvents
    ) -> None:
        """Update the list of active log events."""
        active_log_events.id = list(self.active_alarm_ids.values())

    # Log Events

    def activate_alarm(
            self, code: mcu_pb.LogEventCode, event_type: mcu_pb.LogEventType,
            lower_limit: int, upper_limit: int,
            log_manager: log.Manager
    ) -> None:
        """Create a new Log Event if it is not active."""
        if code in self.active_alarm_ids:
            return

        log_event = mcu_pb.LogEvent(
            code=code, type=event_type, alarm_limits=mcu_pb.Range(
                lower=lower_limit, upper=upper_limit
            )
        )
        self.active_alarm_ids[code] = log_manager.add_event(log_event)

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
            log_manager: log.Manager
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
            log_manager
        )
        self.transform_parameter_alarms(
            alarm_limits.fio2.lower, alarm_limits.fio2.upper,
            sensor_measurements.fio2,
            mcu_pb.LogEventCode.fio2_too_low,
            mcu_pb.LogEventCode.fio2_too_high,
            log_manager
        )
        self._manager.transform_active_log_event_ids(active_log_events)

    # Update methods

    def transform_parameter_alarms(
            self, lower_limit: int, upper_limit: int, value: Union[float, int],
            too_low_code: mcu_pb.LogEventCode,
            too_high_code: mcu_pb.LogEventCode,
            log_manager: log.Manager
    ) -> None:
        """Update the alarms for a particular parameter."""
        if value < lower_limit:
            self._manager.activate_alarm(
                too_low_code, mcu_pb.LogEventType.patient,
                lower_limit, upper_limit, log_manager
            )
        else:
            self._manager.deactivate_alarm(too_low_code)

        if value > upper_limit:
            self._manager.activate_alarm(
                too_high_code, mcu_pb.LogEventType.patient,
                lower_limit, upper_limit, log_manager
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
            ], log_manager: log.Manager
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
        log_manager.update_clock(current_time)
        self._active_service.transform(
            parameters, alarm_limits, sensor_measurements,
            active_log_events, log_manager
        )
