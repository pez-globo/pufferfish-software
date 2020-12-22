"""Servicing of AlarmLimits and AlarmLimitsRequest states."""

import typing
from typing import Mapping, Optional, Tuple, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb


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


# Services


class Service:
    """Base class for the AlarmLimits/AlarmLimitsRequest service."""

    SPO2_MIN = 0
    SPO2_MAX = 100
    FIO2_MIN = 21
    FIO2_MAX = 100

    # Update methods

    def transform(
            self, request: mcu_pb.AlarmLimitsRequest,
            response: mcu_pb.AlarmLimits
    ) -> None:
        """Update the alarm limits."""
        (response.fio2.lower, response.fio2.upper) = transform_limits_range(
            self.FIO2_MIN, self.FIO2_MAX,
            request.fio2.lower, request.fio2.upper,
            response.fio2.lower, response.fio2.upper
        )
        (response.spo2.lower, response.spo2.upper) = transform_limits_range(
            self.SPO2_MIN, self.SPO2_MAX,
            request.spo2.lower, request.spo2.upper,
            response.spo2.lower, response.spo2.upper
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
        mcu_pb.VentilationMode.pc_ac: PCAC(),
        mcu_pb.VentilationMode.hfnc: HFNC()
    }

    def transform(self, all_states: Mapping[
            Type[betterproto.Message], Optional[betterproto.Message]
    ]) -> None:
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

        self._active_service.transform(request, response)
