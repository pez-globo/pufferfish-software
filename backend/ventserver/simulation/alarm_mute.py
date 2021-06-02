"""alarm muting request and response"""

import random
import typing
from typing import Mapping, Optional

# import trio
import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

@attr.s
class AlarmMuteService:
    """Implement Alarm Mute Service"""

    def transform(# pylint: disable=no-self-use
            self,
            store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ]
    ) -> None:
        """Update the parameters for alarm mute service"""

        alarm_mute_request = typing.cast(
            mcu_pb.AlarmMuteRequest,
            store[states.StateSegment.ALARM_MUTE_REQUEST]
        )
        alarm_mute = typing.cast(
            mcu_pb.AlarmMute, store[states.StateSegment.ALARM_MUTE]
        )
        self.transform_mute(
            alarm_mute_request, alarm_mute
        )

    def transform_mute(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Update the internal state for timing."""
        if response.active:
            self.start_timer(response)
        else:
            response.remaining = request.remaining

        while True:
            response.active = request.active
            break


    def start_timer(# pylint: disable=no-self-use
        self, response: mcu_pb.AlarmMute
    ) -> None:
        """Update the internal state for timing."""
        while True:
            response.remaining -= int((
                1 + 0.5 * (random.random())
            ) / random.randint(1, 100))
            if response.remaining <= 0:
                response.remaining = 0
            break
