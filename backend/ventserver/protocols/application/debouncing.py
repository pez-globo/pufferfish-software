"""Appication protocol for event debouncing."""

from typing import Optional

import attr

from ventserver.protocols import events
from ventserver.sansio import protocols


@attr.s
class Input(events.Event):
    """Debouncing input event."""

    monotonic_time: float = attr.ib()
    signal: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class Debouncer(protocols.Filter[Input, bool]):
    """Filter for debouncing a binary signal.

    Debouncing works by integrating the input signal to perform a time
    hysteresis. Adaped from Kenneth Kuhn's C implementation of the algorithm:
    https://www.kennethkuhn.com/electronics/debounce.c
    """

    max_integrator_samples: int = attr.ib(default=100)
    sample_interval: float = attr.ib(default=0.01)  # s
    _last_sample: Optional[float] = attr.ib(default=None)
    _integrator: int = attr.ib(default=0)
    _output: bool = attr.ib(default=False)

    def input(self, event: Optional[Input]) -> None:
        """Handle input events."""
        if event is None:
            return

        if (
                self._last_sample is not None and
                event.monotonic_time - self._last_sample < self.sample_interval
        ):
            return

        self._last_sample = event.monotonic_time

        if not event.signal and self._integrator > 0:
            self._integrator -= 1
        elif event.signal and self._integrator < self.max_integrator_samples:
            self._integrator += 1

    def output(self) -> bool:
        """Emit the next output event."""
        if self._integrator == 0:
            self._output = False
        elif self._integrator >= self.max_integrator_samples:
            self._output = True
            # Defensive code if the integrator got corrupted:
            self._integrator = self.max_integrator_samples
        return self._output
