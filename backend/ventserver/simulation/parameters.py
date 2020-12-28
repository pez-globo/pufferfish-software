"""Servicing of Parameters and ParametersRequest states."""

import abc
import typing
from typing import Mapping, Optional, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb


# Services


class Service(abc.ABC):
    """Base class for the Parameters/ParametersRequest service."""

    FIO2_MIN = 21
    FIO2_MAX = 100

    @abc.abstractmethod
    def mode_active(self, parameters: mcu_pb.Parameters) -> bool:
        """Determine whether the mode is active in the parameters."""

    # Update methods

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters,
    ) -> None:
        """Update the parameters."""

    def transform_fio2(self, fio2_request: float, fio2: float) -> float:
        """Update the FiO2 parameter."""
        if self.FIO2_MIN <= fio2_request <= self.FIO2_MAX:
            return fio2_request
        return fio2


class PCAC(Service):
    """Parameters servicing for PC-AC mode."""

    def mode_active(self, parameters: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return parameters.mode == mcu_pb.VentilationMode.pc_ac

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters
    ) -> None:
        """Implement ParametersService.transform."""
        response.mode = request.mode
        if not self.mode_active(response):
            return

        response.ventilating = request.ventilating
        if request.rr > 0:
            response.rr = request.rr
        if request.ie > 0:
            response.ie = request.ie
        if request.pip > 0:
            response.pip = request.pip
        response.peep = request.peep
        response.fio2 = self.transform_fio2(
            request.fio2, response.fio2
        )


class HFNC(Service):
    """Parameters servicing for HFNC mode."""

    def mode_active(self, response: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return response.mode == mcu_pb.VentilationMode.hfnc

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters
    ) -> None:
        """Implement ParametersService.transform."""
        response.mode = request.mode
        if not self.mode_active(response):
            return

        response.ventilating = request.ventilating
        if 0 <= request.flow <= 80:
            response.flow = request.flow
        response.fio2 = self.transform_fio2(
            request.fio2, response.fio2
        )


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
        """Update the parameters for the requested mode."""
        request = typing.cast(
            mcu_pb.ParametersRequest, all_states[mcu_pb.ParametersRequest]
        )
        response = typing.cast(
            mcu_pb.Parameters, all_states[mcu_pb.Parameters]
        )
        self._active_service = self._services.get(request.mode, None)

        if self._active_service is None:
            return

        self._active_service.transform(request, response)
