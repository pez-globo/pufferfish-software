"""battery power and charging"""

import random
import typing
from typing import  Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import states
from ventserver.protocols.protobuf import mcu_pb

@attr.s
class Service:
    """power management service"""

    current_time: float = attr.ib(default=0)
    cycle_start_time: float = attr.ib(default=0)  # ms
    cycle_period: float = attr.ib(default=30000.0)  # ms
    initial_charge: int = attr.ib(default=0)
    max_charge: int = attr.ib(default=100)
    upper_int: int = attr.ib(default=50)
    lower_int: int = attr.ib(default=0)

    def transform(
            self, current_time: float,store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ]
    ) -> None:
        """Update the parameters for power management service"""
        self.cycle_start_time = current_time

        power_management = typing.cast(
         mcu_pb.PowerManagement,
         store[states.StateSegment.POWER_MANAGEMENT]
        )

        power_management.power_left = \
            self.max_charge - (random.randint(self.lower_int, self.upper_int))
        power_management.power_left = min(
            power_management.power_left, 100
        )
        power_management.power_left = max(
            power_management.power_left, 0
        )
        power_management.charging = True
