"""alarm muting request and response"""

import time
import typing
from typing import Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

@attr.s
class AlarmMuteService:
    """Implement Alarm Mute Service"""

    current_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time
    deadline: int = 120000 # ms

    def transform(
            self, current_time: float,
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
        self.update_clock(current_time, alarm_mute)
        self.transform_mute(
            alarm_mute_request, alarm_mute
        )

    def update_clock(
        self, current_time: float,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Update the internal state for timing."""
        if not response.active:
            return
        self.current_time = current_time * 1000 - self.initial_time

    def transform_mute(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Implement alarm muting."""
        if response.active:
            self.continue_countdown(response)
        else:
            self.initial_time = time.time() * 1000
            response.remaining = 120

        response.active = request.active

    def continue_countdown(
        self, response: mcu_pb.AlarmMute
    ) -> None:
        """countdown for two minutes."""
        response.remaining = int((self.deadline - self.current_time) / 1000)
        max(0, min(response.remaining, 120))
