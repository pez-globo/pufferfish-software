"""alarm muting request and response"""

import typing
from typing import Dict, Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

def transform(
    request: mcu_pb.AlarmMuteRequest,
    response: mcu_pb.AlarmMute
) -> None:
    """Mute the current active alarm"""

    response.active = request.active

@attr.s
class AlarmMuteService:
    """Implement Alarm Mute Service"""

    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def transform(
            self, store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ]
    ) -> None:
        """Update the parameters for alarm mute service"""
        if self.active_alarm_ids is None:
            return

        alarm_mute_request = typing.cast(
            mcu_pb.AlarmMuteRequest,
            store[states.StateSegment.ALARM_MUTE_REQUEST]
        )
        alarm_mute = typing.cast(
            mcu_pb.AlarmMute, store[states.StateSegment.ALARM_MUTE]
        )
        transform(
            alarm_mute_request, alarm_mute
        )