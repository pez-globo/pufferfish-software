"""Appication sub-layer for state synchronization."""

import enum
import logging
from typing import Deque, Generic, Mapping, Optional, TypeVar

import attr

import betterproto

from ventserver.protocols import exceptions
from ventserver.sansio import protocols


_Index = TypeVar('_Index', bound=enum.Enum)


class Sender(protocols.Filter[None, betterproto.Message]):
    """Interface class for state senders."""


IndexedSender = Mapping[_Index, Optional[betterproto.Message]]


@attr.s
class SequentialSender(Sender, Generic[_Index]):
    """State sending filter on a fixed sequence.

    Does not take inputs. Outputs are state updates for the peer.

    Warning: if indexed_sender is mutated by other code, this filter is only
    safe to use in synchronous environments, such as part of another Filter
    which completely owns indexed_sender.
    """

    _logger = logging.getLogger('.'.join((__name__, 'SequentialSender')))

    output_schedule: Deque[_Index] = attr.ib()
    indexed_sender: IndexedSender[_Index] = attr.ib()

    def input(self, event: None) -> None:
        """Handle input events."""

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        output_type = self.output_schedule[0]
        try:
            output_event = self.indexed_sender[output_type]
        except KeyError as exc:
            raise exceptions.ProtocolDataError(
                'Scheduled message type is not a sendable state: {}'
                .format(output_type)
            ) from exc

        self.output_schedule.rotate(-1)
        if output_event is None:
            return None

        self._logger.debug('Sending: %s', output_event)
        return output_event


@attr.s
class TimedSequentialSender(
        protocols.Filter[float, betterproto.Message], Generic[_Index]
):
    """State sending filter on a fixed sequence at a fixed time interval.

    Inputs are clock updates. Outputs are state updates for the peer.

    Warning: if indexed_sender is mutated by other code, this filter is only
    safe to use in synchronous environments, such as part of another Filter
    which completely owns indexed_sender.
    """

    _logger = logging.getLogger('.'.join((__name__, 'TimedSequentialSender')))

    output_schedule: Deque[_Index] = attr.ib()
    indexed_sender: IndexedSender[_Index] = attr.ib()
    output_interval: float = attr.ib(default=0)
    _sender: SequentialSender[_Index] = attr.ib()
    _current_time: Optional[float] = attr.ib(default=None)
    _last_output_time: Optional[float] = attr.ib(default=None)

    @_sender.default
    def init_sender(self) -> SequentialSender[_Index]:
        """Initialize the sequential sender."""
        return SequentialSender(
            output_schedule=self.output_schedule,
            indexed_sender=self.indexed_sender
        )

    def input(self, event: Optional[float]) -> None:
        """Handle input events."""
        if event is None:
            return

        self._logger.debug('Time: %f', event)
        self._current_time = event

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        if self.output_interval != 0 and self._last_output_time is not None:
            if self._current_time is None:
                return None

            if (
                    self._current_time - self._last_output_time <
                    self.output_interval
            ):
                return None

        output = self._sender.output()
        self._last_output_time = self._current_time
        return output
