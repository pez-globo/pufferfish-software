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
    previous_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time
    pulse: float = 1000 # s

    def transform(# pylint: disable=no-self-use
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
        self.update_clock(current_time)
        self.transform_mute(
            alarm_mute_request, alarm_mute
        )

    def update_needed(self) -> bool:
        """If duration is within pulse."""
        return not self.current_time - self.previous_time < self.pulse

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        if self.update_needed():
            self.previous_time = self.current_time
        self.current_time = current_time * 1000 - self.initial_time

    def transform_mute(
        self, request: mcu_pb.AlarmMuteRequest,
        response: mcu_pb.AlarmMute
    ) -> None:
        """Implement alarm muting."""
        if response.active:
            self.start_countdown(response)
        else:
            response.remaining = request.remaining

        while True:
            response.active = request.active
            break

    def start_countdown(# pylint: disable=no-self-use
        self, response: mcu_pb.AlarmMute
    ) -> None:
        """countdown for two minutes."""
        if not self.update_needed():
            return

        while True:
            response.remaining -= 1
            if response.remaining <= 0:
                response.remaining = 0
            break
