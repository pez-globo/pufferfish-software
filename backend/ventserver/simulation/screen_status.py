"""alarm muting request and response"""

import betterproto
import typing

from typing import Mapping, Optional

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

class Service:
    """Implement Lock screen"""

    def transform(self,
      store: Mapping[states.StateSegment, Optional[betterproto.Message]]
    ) -> None:
      screen_status_request = typing.cast(
          mcu_pb.ScreenStatusRequest,
          store[states.StateSegment.SCREEN_STATUS_REQUEST]
      )
      screen_status = typing.cast(
       mcu_pb.ScreenStatus,
       store[states.StateSegment.SCREEN_STATUS]
      )

      screen_status.lock = screen_status_request.lock

