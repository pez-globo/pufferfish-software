"""alarm muting request and response"""

import typing
from typing import Dict, Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb


def transform_mute(
    request: mcu_pb.AlarmMuteRequest,
    response: mcu_pb.AlarmMute
) -> None:
    """Implement alarm mute service"""

    response.active = request.active

@attr.s
class Service:
    """Base class for a Alarm Mute simulator."""

    def transform(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Mute the current active alarm"""

@attr.s
class PCAC(Service):
    """Alarm Mute simulator in PC-AC mode."""

    def transform(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Mute the current active alarm"""

        transform_mute(request, response)

@attr.s
class HFNC(Service):
    """Alarm Mute simulator in HFNC mode."""

    def transform(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Mute the current active alarm"""

        transform_mute(request, response)

@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services: Dict[mcu_pb.VentilationMode, Service] = attr.ib()
    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    @_services.default    # pylint: disable=R0801
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
            ]
    ) -> None:
        """Update the parameters for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, store[states.StateSegment.PARAMETERS]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        if self.active_alarm_ids is None:
            return

        alarm_mute_request = typing.cast(
            mcu_pb.AlarmMuteRequest,
            store[states.StateSegment.ALARM_MUTE_REQUEST]
        )
        alarm_mute = typing.cast(
            mcu_pb.AlarmMute, store[states.StateSegment.ALARM_MUTE]
        )
        self._active_service.transform(
            alarm_mute_request, alarm_mute
        )
