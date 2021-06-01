"""battery power and charging"""

import random
import typing
from typing import  Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import alarms, states
from ventserver.protocols.protobuf import mcu_pb

@attr.s
class Service:
    """power management service"""

    power_noise: float = 0.5

    def transform(
            self, store: Mapping[
                states.StateSegment, Optional[betterproto.Message]
            ], events_log: alarms.Manager
    ) -> None:
        """Update the parameters for power management service"""
        power_management = typing.cast(
         mcu_pb.PowerManagement,
         store[states.StateSegment.POWER_MANAGEMENT]
        )

        if power_management.charging:
            self._transform_charge(power_management, events_log)
        else:
            self._transform_discharge(power_management, events_log)

    def _transform_charge(
        self, power_management: mcu_pb.PowerManagement,
        events_log: alarms.Manager
    ) -> None:
        """Simulate battery charging."""
        events_log.input(
            alarms.AlarmDeactivationEvent(
                codes=[mcu_pb.LogEventCode.charger_disconnected]
        ))
        power_management.power_left += int((
            1 + self.power_noise * (random.random())
        ) / random.randint(1, 100))

        power_management.power_left = \
            min(100, max(power_management.power_left, 0))

        if power_management.power_left == 100:
            power_management.charging = False

    def _transform_discharge(
        self, power_management: mcu_pb.PowerManagement,
        events_log: alarms.Manager
    ) -> None:
        """Simulate battery discharging."""
        events_log.input(alarms.AlarmActivationEvent(
            code=mcu_pb.LogEventCode.charger_disconnected,
            event_type=mcu_pb.LogEventType.system
        ))
        power_management.power_left -= int((
            1 + self.power_noise * (random.random())
        ) / random.randint(1, 100))

        power_management.power_left = \
            min(100, max(power_management.power_left, 0))

        if power_management.power_left == 0:
            power_management.charging = True
