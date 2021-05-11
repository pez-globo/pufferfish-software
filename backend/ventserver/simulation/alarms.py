"""Servicing of Parameters and ParametersRequest states."""

import typing
from typing import Dict, Mapping, Optional, Union

import attr

import betterproto

from ventserver.protocols.backend import alarms, states
from ventserver.protocols.protobuf import mcu_pb


# Simulators


def transform_parameter_alarms(
        lower_limit: int, upper_limit: int, value: Union[float, int],
        too_low_code: mcu_pb.LogEventCode, too_high_code: mcu_pb.LogEventCode,
        events_log: alarms.Manager
) -> None:
    """Update the alarms for a particular parameter."""
    if value < lower_limit:
        events_log.input(alarms.AlarmActivationEvent(
            code=too_low_code, event_type=mcu_pb.LogEventType.patient,
            lower_limit=lower_limit, upper_limit=upper_limit
        ))
    else:
        events_log.input(alarms.AlarmDeactivationEvent(codes=[too_low_code]))

    if value > upper_limit:
        events_log.input(alarms.AlarmActivationEvent(
            code=too_high_code, event_type=mcu_pb.LogEventType.patient,
            lower_limit=lower_limit, upper_limit=upper_limit
        ))
    else:
        events_log.input(alarms.AlarmDeactivationEvent(codes=[too_high_code]))


@attr.s
class Service:
    """Base class for a breathing circuit simulator."""

    ALARM_CODES = {
        mcu_pb.LogEventCode.fio2_too_low,
        mcu_pb.LogEventCode.fio2_too_high,
        mcu_pb.LogEventCode.flow_too_low,
        mcu_pb.LogEventCode.flow_too_high,
        mcu_pb.LogEventCode.spo2_too_low,
        mcu_pb.LogEventCode.spo2_too_high,
        mcu_pb.LogEventCode.hr_too_low,
        mcu_pb.LogEventCode.hr_too_high,
    }

    # Log Events

    def transform(
            self, parameters: mcu_pb.Parameters,
            alarm_limits: mcu_pb.AlarmLimits,
            sensor_measurements: mcu_pb.SensorMeasurements,
            events_log: alarms.Manager
    ) -> None:
        """Update the simulation."""
        if not parameters.ventilating:
            # Deactivate all alarms
            events_log.input(alarms.AlarmDeactivationEvent(
                codes=self.ALARM_CODES
            ))
            return

        transform_parameter_alarms(
            alarm_limits.fio2.lower, alarm_limits.fio2.upper,
            sensor_measurements.fio2,
            mcu_pb.LogEventCode.fio2_too_low,
            mcu_pb.LogEventCode.fio2_too_high,
            events_log
        )
        transform_parameter_alarms(
            alarm_limits.spo2.lower, alarm_limits.spo2.upper,
            sensor_measurements.spo2,
            mcu_pb.LogEventCode.spo2_too_low,
            mcu_pb.LogEventCode.spo2_too_high,
            events_log
        )
        transform_parameter_alarms(
            alarm_limits.hr.lower, alarm_limits.hr.upper,
            sensor_measurements.hr,
            mcu_pb.LogEventCode.hr_too_low,
            mcu_pb.LogEventCode.hr_too_high,
            events_log
        )


@attr.s
class PCAC(Service):
    """Breathing circuit simulator in PC-AC mode."""


@attr.s
class HFNC(Service):
    """Breathing circuit simulator in HFNC mode."""

    def transform(
            self, parameters: mcu_pb.Parameters,
            alarm_limits: mcu_pb.AlarmLimits,
            sensor_measurements: mcu_pb.SensorMeasurements,
            events_log: alarms.Manager
    ) -> None:
        """Update the simulation."""
        super().transform(
            parameters, alarm_limits, sensor_measurements, events_log
        )
        if not parameters.ventilating:
            return
        transform_parameter_alarms(
            alarm_limits.flow.lower, alarm_limits.flow.upper,
            sensor_measurements.flow,
            mcu_pb.LogEventCode.flow_too_low,
            mcu_pb.LogEventCode.flow_too_high,
            events_log
        )


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services: Dict[mcu_pb.VentilationMode, Service] = attr.ib()

    @_services.default
    def init_services(self) -> Dict[
            mcu_pb.VentilationMode, Service
    ]:  # pylint: disable=no-self-use
        """Initialize the services."""
        return {
            mcu_pb.VentilationMode.pc_ac: PCAC(),
            mcu_pb.VentilationMode.hfnc: HFNC()
        }

    def transform(
            self, store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ], events_log: alarms.Manager
    ) -> None:
        """Update the parameters for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, store[states.StateSegment.PARAMETERS]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        alarm_limits = typing.cast(
            mcu_pb.AlarmLimits, store[states.StateSegment.ALARM_LIMITS]
        )
        sensor_measurements = typing.cast(
            mcu_pb.SensorMeasurements,
            store[states.StateSegment.SENSOR_MEASUREMENTS]
        )
        self._active_service.transform(
            parameters, alarm_limits, sensor_measurements, events_log
        )
