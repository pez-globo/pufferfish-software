"""Servicing of AlarmLimits and AlarmLimitsRequest states."""

import dataclasses
import typing
from typing import Mapping, Optional, Tuple

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb
from ventserver.protocols import backend
from ventserver.simulation import log


# Update Functions

def transform_limits_range(
        floor: int, ceiling: int, requested_min: int, requested_max: int,
        current_min: int, current_max: int
) -> Tuple[int, int]:
    """Return requested if between floor and ceiling, or else return current."""
    if current_min > current_max:
        (current_min, current_max) = (current_max, current_min)
    if requested_min > requested_max:
        (requested_min, requested_max) = (requested_max, requested_min)
    if not floor <= current_min <= current_max <= ceiling:
        requested_min = min(ceiling, max(floor, requested_min))
        requested_max = min(ceiling, max(floor, requested_max))
    if not floor <= requested_min <= requested_max <= ceiling:
        return (current_min, current_max)

    return (requested_min, requested_max)


def service_limits_range(
        request: mcu_pb.Range, response: mcu_pb.Range, floor: int, ceiling: int,
        code: mcu_pb.LogEventCode, log_manager: log.Manager
) -> None:
    """Handle the request's alarm limits range."""
    old_response = dataclasses.replace(response)
    (response.lower, response.upper) = transform_limits_range(
        floor, ceiling, request.lower, request.upper,
        response.lower, response.upper
    )
    if (
            old_response.lower == response.lower and
            old_response.upper == response.upper
    ):
        return

    new_response = dataclasses.replace(response)
    log_manager.add_event(mcu_pb.LogEvent(
        code=code, type=mcu_pb.LogEventType.alarm_limits,
        old_range=old_response, new_range=new_response
    ))


# Services


class Service:
    """Base class for the AlarmLimits/AlarmLimitsRequest service."""

    FIO2_MIN = 21
    FIO2_MAX = 100
    SPO2_MIN = 0
    SPO2_MAX = 100
    HR_MIN = 0
    HR_MAX = 200

    # Update methods

    def transform(
            self, request: mcu_pb.AlarmLimitsRequest,
            response: mcu_pb.AlarmLimits, log_manager: log.Manager
    ) -> None:
        """Update the alarm limits."""
        service_limits_range(
            request.fio2, response.fio2, self.FIO2_MIN, self.FIO2_MAX,
            mcu_pb.LogEventCode.fio2_alarm_limits_changed, log_manager
        )
        service_limits_range(
            request.spo2, response.spo2, self.SPO2_MIN, self.SPO2_MAX,
            mcu_pb.LogEventCode.spo2_alarm_limits_changed, log_manager
        )
        service_limits_range(
            request.hr, response.hr, self.HR_MIN, self.HR_MAX,
            mcu_pb.LogEventCode.hr_alarm_limits_changed, log_manager
        )


class PCAC(Service):
    """Alarm limits servicing for PC-AC mode."""


class HFNC(Service):
    """Alarm limits servicing for HFNC mode."""


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services = {
        mcu_pb.VentilationMode.hfnc: HFNC()
    }

    def transform(
            self, current_time: float, all_states: Mapping[
                backend.StateSegment, Optional[betterproto.Message]
            ], log_manager: log.Manager
    ) -> None:
        """Update the alarm limits for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, all_states[backend.StateSegment.PARAMETERS]
        )
        request = typing.cast(
            mcu_pb.AlarmLimitsRequest,
            all_states[backend.StateSegment.ALARM_LIMITS_REQUEST]
        )
        response = typing.cast(
            mcu_pb.AlarmLimits, all_states[backend.StateSegment.ALARM_LIMITS]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        log_manager.update_clock(current_time)
        self._active_service.transform(request, response, log_manager)
