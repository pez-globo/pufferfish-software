"""Servicing of AlarmLimits and AlarmLimitsRequest states."""

import dataclasses
import typing
from typing import Mapping, Optional, Tuple, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb
from ventserver.simulation import log


def transform_limits_range(
        floor: int, ceiling: int, requested_min: int, requested_max: int,
        current_min: int, current_max: int
) -> Tuple[int, int]:
    """Return requested if between floor and ceiling, or else return current."""
    if floor <= requested_min <= requested_max <= ceiling:
        return (requested_min, requested_max)
    current_min = max(current_min, floor)
    current_max = min(current_max, ceiling)

    return (current_min, current_max)


def service_limits_range(
        request: mcu_pb.Range, response: mcu_pb.Range, floor: int, ceiling: int,
        code: mcu_pb.LogEventCode, log_manager: log.Manager
) -> None:
    """Handle the request's alarm limits range."""
    if request.lower == response.lower and request.upper == response.upper:
        return

    (new_lower, new_upper) = transform_limits_range(
        floor, ceiling, request.lower, request.upper,
        response.lower, response.upper
    )
    old_response = dataclasses.replace(response)
    response.lower = new_lower
    response.upper = new_upper
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
                Type[betterproto.Message], Optional[betterproto.Message]
            ], log_manager: log.Manager
    ) -> None:
        """Update the alarm limits for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, all_states[mcu_pb.Parameters]
        )
        request = typing.cast(
            mcu_pb.AlarmLimitsRequest, all_states[mcu_pb.AlarmLimitsRequest]
        )
        response = typing.cast(
            mcu_pb.AlarmLimits, all_states[mcu_pb.AlarmLimits]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        log_manager.update_clock(current_time)
        self._active_service.transform(request, response, log_manager)
