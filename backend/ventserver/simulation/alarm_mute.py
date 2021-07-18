"""alarm muting request and response"""

import random
import time
import typing
from typing import Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb


@attr.s
class Service:
    """Implement Alarm Mute Service"""
    MUTE_MAX_DURATION = 120000  # ms

    active: bool = attr.ib(default=False)
    mute_start_time: Optional[float] = attr.ib(default=None)
    seq_num: int = attr.ib(default=random.getrandbits(32))

    # TODO: also allow canceling alarm mute upon loss of connection, maybe with
    # an AlarmMute client (rather than a service)
    def transform(
            self, current_time: float,
            store: Mapping[states.StateSegment, Optional[betterproto.Message]]
    ) -> None:
        """Update the parameters for alarm mute service"""
        alarm_mute_request = typing.cast(
            mcu_pb.AlarmMuteRequest,
            store[states.StateSegment.ALARM_MUTE_REQUEST]
        )
        alarm_mute = typing.cast(
            mcu_pb.AlarmMute, store[states.StateSegment.ALARM_MUTE]
        )
        self.transform_mute(current_time, alarm_mute_request, alarm_mute)

    def transform_mute(
            self, current_time: float,
            request: mcu_pb.AlarmMuteRequest, response: mcu_pb.AlarmMute
    ) -> None:
        """Implement alarm muting."""
        if request.seq_num == self.seq_num + 1:
            self._update_internal_state(request.active, current_time)
        self._update_response(current_time, response)

    def _update_internal_state(self, active: bool, current_time: float) -> None:
        """Update internal state."""
        if self.active == active:
            return

        self.seq_num += 1
        self.active = active
        if active:
            self.mute_start_time = current_time * 1000
            # TODO: make log event for starting mute
        else:
            self.mute_start_time = None
            # TODO: make log event for ending mute

    def _update_response(
            self, current_time: float, response: mcu_pb.AlarmMute
    ) -> None:
        """Update response based on internal state."""
        response.active = self.active
        response.seq_num = self.seq_num
        self._update_remaining(current_time, response)
        if response.remaining > 0:
            return

        self._update_internal_state(False, current_time)
        response.active = self.active
        response.seq_num = self.seq_num
        self._update_remaining(current_time, response)

    def _update_remaining(
            self, current_time: float, response: mcu_pb.AlarmMute
    ) -> None:
        """Update remaining field of response based on internal state."""
        if not self.active:
            response.remaining = int(self.MUTE_MAX_DURATION / 1000)
            return

        if self.mute_start_time is None:
            self.mute_start_time = current_time * 1000
        mute_duration = current_time * 1000 - self.mute_start_time
        remaining = int((self.MUTE_MAX_DURATION - mute_duration) / 1000)
        response.remaining = max(
            0, min(remaining, int(self.MUTE_MAX_DURATION / 1000))
        )
