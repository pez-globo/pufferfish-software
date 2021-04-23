"""Appication protocol for clock synchronization."""

from typing import Optional

import attr

from ventserver.protocols import events
from ventserver.protocols import exceptions
from ventserver.sansio import protocols


@attr.s
class UpdateEvent(events.Event):
    """State update event."""

    current_time: float = attr.ib()
    remote_time: int = attr.ib()

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class ClockSynchronizer(protocols.Filter[UpdateEvent, int]):
    """Clock synchronization filter for clocks on different reference points.

    This is specifically designed for synchronizing between a remote clock on a
    microcontroller (which has times relative to when the microcontroller
    started, and when the clock rolled over) and the local wall clock.
    Inputs are current times at the local clock and the remote clock. The local
    clock time is a float in units of seconds, while the remote clock time is a
    int in units of milliseconds.
    Outputs are the offset to add to a remote clock time to translate it into
    the local clock. Currently, they become invalid if the remote clock rolls
    over, because we need to figue out a robust heuristic to differentiate
    between clock rollover and input of an older remote timestamp when remote
    time hasn't rolled over.
    """

    REMOTE_CLOCK_ROLLOVER = 2 ** 32

    _current_time: Optional[float] = attr.ib(default=None)
    _remote_sync_time: Optional[int] = attr.ib(default=None)  # ms
    _local_sync_time: Optional[float] = attr.ib(default=None)  # sec

    def input(self, event: Optional[UpdateEvent]) -> None:
        """Handle input events."""
        if event is None:
            return

        self._current_time = event.current_time
        if self._remote_sync_time is None:
            self._remote_sync_time = event.remote_time
            self._local_sync_time = self._current_time

    def output(self) -> int:
        """Emit the next output event."""
        if self._remote_sync_time is None or self._local_sync_time is None:
            raise exceptions.ProtocolDataError(
                'Clock synchronization was not initialized!'
            )

        return int(-self._remote_sync_time + self._local_sync_time * 1000)
