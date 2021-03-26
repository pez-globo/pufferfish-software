"""Servicing of Parameters and ParametersRequest states."""

import abc
import typing
from typing import Mapping, Optional, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb
from ventserver.simulation import log


# Update Functions

def service_mode(
        request: mcu_pb.ParametersRequest, response: mcu_pb.Parameters,
        log_manager: log.Manager
) -> None:
    """Handle the request's ventilation mode."""
    if response.mode == request.mode:
        return

    log_manager.add_event(mcu_pb.LogEvent(
        code=mcu_pb.LogEventCode.ventilation_mode_changed,
        type=mcu_pb.LogEventType.control,
        old_mode=response.mode, new_mode=request.mode
    ))
    response.mode = request.mode


def service_ventilating(
        request: mcu_pb.ParametersRequest, response: mcu_pb.Parameters,
        log_manager: log.Manager
) -> None:
    """Handle the request's ventilation operation status."""
    if response.ventilating == request.ventilating:
        return

    log_manager.add_event(mcu_pb.LogEvent(
        code=mcu_pb.LogEventCode.ventilation_operation_changed,
        type=mcu_pb.LogEventType.control,
        old_bool=response.ventilating, new_bool=request.ventilating
    ))
    response.ventilating = request.ventilating


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
            response: mcu_pb.Parameters, log_manager: log.Manager
    ) -> None:
        """Update the parameters."""

    def service_fio2(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, log_manager: log.Manager
    ) -> None:
        """Handle the request's FiO2."""
        if not self.FIO2_MIN <= request.fio2 <= self.FIO2_MAX:
            return

        if response.fio2 == request.fio2:
            return

        print(request.fio2, response.fio2)
        log_manager.add_event(mcu_pb.LogEvent(
            code=mcu_pb.LogEventCode.fio2_setting_changed,
            type=mcu_pb.LogEventType.control,
            old_float=response.fio2, new_float=request.fio2
        ))
        response.fio2 = request.fio2


class PCAC(Service):
    """Parameters servicing for PC-AC mode."""

    def mode_active(self, parameters: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return parameters.mode == mcu_pb.VentilationMode.pc_ac

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, log_manager: log.Manager
    ) -> None:
        """Implement ParametersService.transform."""
        service_mode(request, response, log_manager)
        if not self.mode_active(response):
            return

        service_ventilating(request, response, log_manager)
        if request.rr > 0:
            response.rr = request.rr
        if request.ie > 0:
            response.ie = request.ie
        if request.pip > 0:
            response.pip = request.pip
        response.peep = request.peep
        self.service_fio2(request, response, log_manager)


class HFNC(Service):
    """Parameters servicing for HFNC mode."""

    FLOW_MIN = 0
    FLOW_MAX = 80

    def mode_active(self, response: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return response.mode == mcu_pb.VentilationMode.hfnc

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, log_manager: log.Manager
    ) -> None:
        """Implement ParametersService.transform."""
        service_mode(request, response, log_manager)
        if not self.mode_active(response):
            return

        service_ventilating(request, response, log_manager)
        self.service_flow(request, response, log_manager)
        self.service_fio2(request, response, log_manager)

    def service_flow(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, log_manager: log.Manager
    ) -> None:
        """Handle the request's flow rate."""
        if not self.FLOW_MIN <= request.flow <= self.FLOW_MAX:
            return

        if response.flow == request.flow:
            return

        log_manager.add_event(mcu_pb.LogEvent(
            code=mcu_pb.LogEventCode.flow_setting_changed,
            type=mcu_pb.LogEventType.control,
            old_float=response.flow, new_float=request.flow
        ))
        response.flow = request.flow


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services = {
        mcu_pb.VentilationMode.pc_ac: PCAC(),
        mcu_pb.VentilationMode.hfnc: HFNC()
    }

    def transform(
            self, current_time: float, all_states: Mapping[
                Type[betterproto.Message], Optional[betterproto.Message]
            ], log_manager: log.Manager
    ) -> None:
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

        log_manager.update_clock(current_time)
        self._active_service.transform(request, response, log_manager)
