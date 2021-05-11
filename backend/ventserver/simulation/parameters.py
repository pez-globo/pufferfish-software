"""Servicing of Parameters and ParametersRequest states."""

import abc
import typing
from typing import Mapping, Optional

import attr

import betterproto

from ventserver.protocols.application import lists
from ventserver.protocols.backend import log, states
from ventserver.protocols.protobuf import mcu_pb


# Update Functions

def service_mode(
        request: mcu_pb.ParametersRequest, response: mcu_pb.Parameters,
        simulated_log: log.LocalLogSource,
        log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Handle the request's ventilation mode."""
    if response.mode == request.mode:
        return

    simulated_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
        code=mcu_pb.LogEventCode.ventilation_mode_changed,
        type=mcu_pb.LogEventType.control,
        old_mode=response.mode, new_mode=request.mode
    )))
    log_receiver.input(mcu_pb.NextLogEvents(
        elements=simulated_log.output().new_events
    ))
    response.mode = request.mode


def service_ventilating(
        request: mcu_pb.ParametersRequest, response: mcu_pb.Parameters,
        simulated_log: log.LocalLogSource,
        log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Handle the request's ventilation operation status."""
    if response.ventilating == request.ventilating:
        return

    simulated_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
        code=mcu_pb.LogEventCode.ventilation_operation_changed,
        type=mcu_pb.LogEventType.control,
        old_bool=response.ventilating, new_bool=request.ventilating
    )))
    log_receiver.input(mcu_pb.NextLogEvents(
        elements=simulated_log.output().new_events
    ))
    response.ventilating = request.ventilating


def transform_parameter(
        floor: float, ceiling: float, request: float, current: float
) -> float:
    """Return requested if between floor and ceiling, or else return current."""
    if not floor <= current <= ceiling:
        request = min(ceiling, max(floor, request))
    if floor <= request <= ceiling:
        return request

    return current


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
            response: mcu_pb.Parameters, simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the parameters."""

    def service_fio2(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Handle the request's FiO2."""
        old_response = response.fio2
        response.fio2 = transform_parameter(
            self.FIO2_MIN, self.FIO2_MAX, request.fio2, response.fio2)
        if old_response == response.fio2:
            return

        simulated_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
            code=mcu_pb.LogEventCode.fio2_setting_changed,
            type=mcu_pb.LogEventType.control,
            old_float=old_response, new_float=response.fio2
        )))
        log_receiver.input(mcu_pb.NextLogEvents(
            elements=simulated_log.output().new_events
        ))


class PCAC(Service):
    """Parameters servicing for PC-AC mode."""

    def mode_active(self, parameters: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return parameters.mode == mcu_pb.VentilationMode.pc_ac

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Implement ParametersService.transform."""
        service_mode(request, response, simulated_log, log_receiver)
        if not self.mode_active(response):
            return

        service_ventilating(request, response, simulated_log, log_receiver)
        if request.rr > 0:
            response.rr = request.rr
        if request.ie > 0:
            response.ie = request.ie
        if request.pip > 0:
            response.pip = request.pip
        response.peep = request.peep
        self.service_fio2(request, response, simulated_log, log_receiver)


class HFNC(Service):
    """Parameters servicing for HFNC mode."""

    FLOW_MIN = 0
    FLOW_MAX = 80

    def mode_active(self, response: mcu_pb.Parameters) -> bool:
        """Implement ParametersService.mode_active."""
        return response.mode == mcu_pb.VentilationMode.hfnc

    def transform(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Implement ParametersService.transform."""
        service_mode(request, response, simulated_log, log_receiver)
        if not self.mode_active(response):
            return

        service_ventilating(request, response, simulated_log, log_receiver)
        self.service_flow(request, response, simulated_log, log_receiver)
        self.service_fio2(request, response, simulated_log, log_receiver)

    def service_flow(
            self, request: mcu_pb.ParametersRequest,
            response: mcu_pb.Parameters, simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Handle the request's flow rate."""
        old_response = response.flow
        response.flow = transform_parameter(
            self.FLOW_MIN, self.FLOW_MAX, request.flow, response.flow)
        if old_response == response.flow:
            return

        simulated_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
            code=mcu_pb.LogEventCode.flow_setting_changed,
            type=mcu_pb.LogEventType.control,
            old_float=old_response, new_float=response.flow
        )))
        log_receiver.input(mcu_pb.NextLogEvents(
            elements=simulated_log.output().new_events
        ))


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
            self, current_time: float, store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ], simulated_log: log.LocalLogSource,
            log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the parameters for the requested mode."""
        request = typing.cast(
            mcu_pb.ParametersRequest,
            store[states.StateSegment.PARAMETERS_REQUEST]
        )
        response = typing.cast(
            mcu_pb.Parameters, store[states.StateSegment.PARAMETERS]
        )
        self._active_service = self._services.get(request.mode, None)

        if self._active_service is None:
            return

        simulated_log.input(log.LocalLogInputEvent(current_time=current_time))
        self._active_service.transform(
            request, response, simulated_log, log_receiver
        )
