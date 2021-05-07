"""Appication protocol for connection status handling."""

from typing import Optional

import attr

from ventserver.protocols import events
from ventserver.sansio import protocols


# Timeout Detection


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


# Timeout Handling


@attr.s
class ActionStatus(events.Event):
    """Connection update event."""

    current_time: float = attr.ib()
    trigger: Optional[bool] = attr.ib(default=None)
    execute: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class ActionDebouncer(protocols.Filter[ActionStatus, bool]):
    """Filter for debouncing actions responding to connection timeouts.

    If the interval is 0, the action will not be debounced - it will always be
    specified as ready to run. If the interval is None, the action will not be
    repeated until after the trigger is cleared. Otherwise, the action will
    be specified as ready to run with the specified repetition interval.
    When available, output specifies whether to run the action.
    """

    repeat_interval: Optional[float] = attr.ib(default=None)
    _current_time: Optional[float] = attr.ib(default=None)
    _triggered: bool = attr.ib(default=False)
    _last_execution: Optional[float] = attr.ib(default=None)

    def input(self, event: Optional[ActionStatus]) -> None:
        """Handle input events."""
        if event is None:
            return

        self._current_time = event.current_time
        if event.execute:
            self._last_execution = self._current_time
        if event.trigger is not None:
            self._triggered = event.trigger
            if not event.trigger:
                self._last_execution = None

    def output(self) -> Optional[bool]:
        """Emit the next output event."""
        if self._current_time is None:
            return None

        if not self._triggered:
            return False

        # Action has been triggered; then:
        if self._last_execution is None:
            return True

        # Action has been run at least once since the trigger; then:
        if self.repeat_interval is None:
            return False

        # Action should be repeated; then:
        return (
            int(self._current_time - self._last_execution) >
            self.repeat_interval
        )

    def run_action(self, current_time: float) -> None:
        """Reset the stopwatch on repeating the action."""
        self._last_execution = current_time
