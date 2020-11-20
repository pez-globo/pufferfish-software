"""Servicing of AlarmLimits and AlarmLimitsRequest states."""

import typing
from typing import Mapping, Optional, Tuple, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb


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
        (response.fio2_min, response.fio2_max) = self.transform_fio2(
            request.fio2_min, request.fio2_max,
            response.fio2_min, response.fio2_max
        )
        (response.spo2_min, response.spo2_max) = self.transform_spo2(
            request.spo2_min, request.spo2_max,
            response.spo2_min, response.spo2_max
        )

    def transform_fio2(
            self, fio2_min_request: int, fio2_max_request: int,
            fio2_min: int, fio2_max: int
    ) -> Tuple[int, int]:
        """Update the FiO2 alarm limits."""
        if (
                self.FIO2_MIN <= fio2_min_request
                <= fio2_max_request <= self.FIO2_MAX
        ):
            return (fio2_min_request, fio2_max_request)
        return (fio2_min, fio2_max)

    def transform_spo2(
            self, spo2_min_request: int, spo2_max_request: int,
            spo2_min: int, spo2_max: int
    ) -> Tuple[int, int]:
        """Update the SpO2 alarm limits."""
        if (
                self.SPO2_MIN <= spo2_min_request
                <= spo2_max_request <= self.SPO2_MAX
        ):
            return (spo2_min_request, spo2_max_request)
        return (spo2_min, spo2_max)


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
