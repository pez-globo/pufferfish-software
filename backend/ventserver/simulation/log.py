"""Servicing of Parameters and ParametersRequest states."""

import dataclasses
import time

import attr

from ventserver.protocols.application import lists
from ventserver.protocols.protobuf import mcu_pb


# Simulators


@attr.s
class Manager:
    """Shared LogEvent manager."""

    receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent] = attr.ib(
        default=lists.ReceiveSynchronizer()
    )

    current_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time

    next_log_event_id: int = attr.ib(default=0)

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self.current_time = current_time * 1000 - self.initial_time

    # Log Events

    def add_event(self, event_data: mcu_pb.LogEvent) -> int:
        """Create a new Log Event with the provided data.

        The id and time fields are automatically generated, and the id is
        returned; however, event_data is not modified.
        """
        log_event = dataclasses.replace(event_data)
        log_event.id = self.next_log_event_id
        log_event.time = int(self.current_time)
        self.receiver.input(mcu_pb.NextLogEvents(elements=[log_event]))
        self.next_log_event_id += 1
        return log_event.id
