"""screen lock request and response"""

import typing
from typing import Mapping, Optional

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

class Service:
    """Screen Status Service"""

    def transform(self, #pylint: disable=no-self-use
      store: Mapping[states.StateSegment, Optional[betterproto.Message]]
    ) -> None:
        """Implement screen lock"""
        screen_status_request = typing.cast(
            mcu_pb.ScreenStatusRequest,
            store[states.StateSegment.SCREEN_STATUS_REQUEST]
        )
        screen_status = typing.cast(
        mcu_pb.ScreenStatus,
        store[states.StateSegment.SCREEN_STATUS]
        )

        screen_status.lock = screen_status_request.lock
