"""Appication protocol for connection status handling."""

from typing import Optional

import attr

from ventserver.protocols import events
from ventserver.sansio import protocols


@attr.s
class UpdateEvent(events.Event):
    """Connection update event."""

    current_time: float = attr.ib()
    event_received: Optional[bool] = attr.ib(default=None)
    connected: Optional[bool] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class StatusEvent(events.Event):
    """Connection status event."""

    uptime: float = attr.ib(default=0)
    timed_out: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class TimeoutDetector(protocols.Filter[UpdateEvent, StatusEvent]):
    """Filter for detecting timeouts in connections.

    ConnectionEvents represent a known change in change in connection status,
    or an event received over a connection. They reset the stopwatch for
    detecting whether the connection has timed out (i.e. appears to be
    unresponsive, independently of whether the connection is up).
    """

    event_timeout: float = attr.ib()
    _current_time: Optional[float] = attr.ib(default=None)
    _last_event: Optional[float] = attr.ib(default=None)
    _last_connection: Optional[float] = attr.ib(default=None)

    def input(self, event: Optional[UpdateEvent]) -> None:
        """Handle input events."""
        if event is None:
            return

        self._current_time = event.current_time
        if event.event_received is not None:
            self._last_event = self._current_time
        if event.connected is not None:
            self._last_event = self._current_time
            self._last_connection = (
                self._current_time if event.connected else None
            )

    def output(self) -> Optional[StatusEvent]:
        """Emit the next output event."""
        if self._current_time is None:
            return None

        output = StatusEvent()
        if self._last_event is not None:
            output.timed_out = (
                int(self._current_time - self._last_event) > self.event_timeout
            )
        if self._last_connection is not None:
            output.uptime = self._current_time - self._last_connection
        return output
