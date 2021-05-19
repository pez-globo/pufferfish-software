"""alarm muting request and response"""

from typing import Dict

import attr
from ventserver.protocols.protobuf import mcu_pb

@attr.s
class Service:
    """Alarm mute service"""

    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def transform(
     self, request: mcu_pb.AlarmMuteRequest,
     response: mcu_pb.AlarmMute
    ) -> None:
        """Implement alarm mute service"""
        if self.active_alarm_ids is None:
            return

        response.active = request.active