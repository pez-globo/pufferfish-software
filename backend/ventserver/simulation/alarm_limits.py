"""Servicing of AlarmLimits and AlarmLimitsRequest states."""

import dataclasses
import typing
from typing import Mapping, Optional, Tuple

import attr

import betterproto

from ventserver.protocols.backend import alarms, log, states
from ventserver.protocols.protobuf import mcu_pb


# Update Functions

def transform_limits_range(
        floor: int, ceiling: int, requested_min: int, requested_max: int
) -> Tuple[int, int]:
    """Return requested, clamped between floor and ceiling."""
    if requested_min > requested_max:
        (requested_min, requested_max) = (requested_max, requested_min)
    requested_min = min(ceiling, max(floor, requested_min))
    requested_max = min(ceiling, max(floor, requested_max))
    return (requested_min, requested_max)


def service_limits_range(
        request: mcu_pb.Range, response: mcu_pb.Range, floor: int, ceiling: int,
        code: mcu_pb.LogEventCode, events_log: alarms.Manager
) -> None:
    """Handle the request's alarm limits range."""
    old_response = dataclasses.replace(response)
    (response.lower, response.upper) = transform_limits_range(
        floor, ceiling, request.lower, request.upper
    )
    if (
            old_response.lower == response.lower and
            old_response.upper == response.upper
    ):
        return

    new_response = dataclasses.replace(response)
    events_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
        code=code, type=mcu_pb.LogEventType.alarm_limits,
        old_range=old_response, new_range=new_response
    )))


# Services


class Service:
    """Base class for the AlarmLimits/AlarmLimitsRequest service."""

    FIO2_TOLERANCE = 2  # % FiO2
    FIO2_MIN = 21  # % FiO2
    FIO2_MAX = 100  # % FiO2
    SPO2_MIN = 0  # % SpO2
    SPO2_MAX = 100  # % SpO2
    HR_MIN = 0  # bpm
    HR_MAX = 200  # bpm

    # Update methods

    def transform(
            self,
            parameters: mcu_pb.Parameters,  # pylint: disable=unused-argument
            request: mcu_pb.AlarmLimitsRequest, response: mcu_pb.AlarmLimits,
            events_log: alarms.Manager
    ) -> None:
        """Update the alarm limits."""
        fio2_range = mcu_pb.Range(
            lower=int(parameters.fio2 - self.FIO2_TOLERANCE),
            upper=int(parameters.fio2 + self.FIO2_TOLERANCE)
        )
        service_limits_range(
            fio2_range, response.fio2, self.FIO2_MIN, self.FIO2_MAX,
            mcu_pb.LogEventCode.fio2_alarm_limits_changed, events_log
        )
        service_limits_range(
            request.spo2, response.spo2, self.SPO2_MIN, self.SPO2_MAX,
            mcu_pb.LogEventCode.spo2_alarm_limits_changed, events_log
        )
        service_limits_range(
            request.hr, response.hr, self.HR_MIN, self.HR_MAX,
            mcu_pb.LogEventCode.hr_alarm_limits_changed, events_log
        )


class PCAC(Service):
    """Alarm limits servicing for PC-AC mode."""


class HFNC(Service):
    """Alarm limits servicing for HFNC mode."""

    FLOW_TOLERANCE = 2  # L/min
    FLOW_MIN = -2  # L/min
    FLOW_MAX = 80  # L/min

    def transform(
            self, parameters: mcu_pb.Parameters,
            request: mcu_pb.AlarmLimitsRequest, response: mcu_pb.AlarmLimits,
            events_log: alarms.Manager
    ) -> None:
        """Update the alarm limits."""
        super().transform(parameters, request, response, events_log)
        flow_range = mcu_pb.Range(
            lower=int(parameters.flow - self.FLOW_TOLERANCE),
            upper=int(parameters.flow + self.FLOW_TOLERANCE)
        )
        service_limits_range(
            flow_range, response.flow, self.FLOW_MIN, self.FLOW_MAX,
            mcu_pb.LogEventCode.flow_alarm_limits_changed, events_log
        )


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services = {
        mcu_pb.VentilationMode.hfnc: HFNC()
    }

    def transform(
            self, store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ], events_log: alarms.Manager
    ) -> None:
        """Update the alarm limits for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, store[states.StateSegment.PARAMETERS]
        )
        request = typing.cast(
            mcu_pb.AlarmLimitsRequest,
            store[states.StateSegment.ALARM_LIMITS_REQUEST]
        )
        response = typing.cast(
            mcu_pb.AlarmLimits, store[states.StateSegment.ALARM_LIMITS]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        self._active_service.transform(
            parameters, request, response, events_log
        )
