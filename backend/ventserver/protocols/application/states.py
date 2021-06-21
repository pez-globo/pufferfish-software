"""Appication sub-layer for state synchronization."""

import collections
import enum
import logging
from typing import (
    Any, Deque, Generic, Iterable, Iterator, Mapping, Optional, TypeVar
)

import attr

import betterproto

from ventserver.protocols import exceptions
from ventserver.sansio import protocols


_Index = TypeVar('_Index', bound=enum.Enum)


class Sender(protocols.Filter[Any, betterproto.Message]):
    """Interface class for state senders.

    Classes which implement this interface may either take a narrower input type
    or take no inputs.
    """


IndexedSender = Mapping[_Index, Optional[betterproto.Message]]


@attr.s
class MappedSenders(IndexedSender[_Index]):
    """A collection of Sender filters which can be indexed into."""

    senders: Mapping[_Index, Sender] = attr.ib()

    def __getitem__(self, key: _Index) -> Optional[betterproto.Message]:
        """Return the next output from the specified sender."""
        return self.senders[key].output()

    def __len__(self) -> int:
        """Return the number of senders."""
        return len(self.senders)

    def __iter__(self) -> Iterator[_Index]:
        """Iterate over all indices."""
        return self.senders.__iter__()


@attr.s
class SequentialSender(Sender, Generic[_Index]):
    """State sending filter on a fixed sequence.

    Does not take inputs. Outputs are state updates for the peer.
    If skip_unavailable is set to True, when an index is reached which
    causes indexed_sender to return None, the sequential sender will keep
    advancing the output schedule until it reaches an index for which
    indexed_sender doesn't return None, until it has gone through the entire
    schedule.

    Warning: if indexed_sender is mutated by other code, this filter is only
    safe to use in synchronous environments, such as part of another Filter
    which completely owns indexed_sender.
    """

    _logger = logging.getLogger('.'.join((__name__, 'SequentialSender')))

    output_schedule: Iterable[_Index] = attr.ib()
    indexed_sender: IndexedSender[_Index] = attr.ib()
    skip_unavailable: bool = attr.ib(default=False)
    _schedule: Deque[_Index] = attr.ib()

    @_schedule.default
    def init_schedule(self) -> Deque[_Index]:
        """Initialize the internal output schedule."""
        return collections.deque(self.output_schedule)

    def input(self, event: None) -> None:
        """Handle input events."""

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        for _ in range(len(self._schedule)):
            output = self._get_next_output()
            if output is not None or not self.skip_unavailable:
                return output
        return None

    def _get_next_output(self) -> Optional[betterproto.Message]:
        """Produce the next state in the schedule."""
        try:
            output_type = self._schedule[0]
        except IndexError:
            return None

        try:
            output_event = self.indexed_sender[output_type]
        except KeyError as exc:
            raise exceptions.ProtocolDataError(
                'Scheduled message type is not a sendable state: {}'
                .format(output_type)
            ) from exc

        self._schedule.rotate(-1)
        if output_event is None:
            return None

        self._logger.debug('Sending: %s', output_event)
        return output_event


@attr.s
class TimedSequentialSender(Sender, Generic[_Index]):
    """State sending filter on a fixed sequence at a fixed time interval.

    Inputs are clock updates. Outputs are state updates for the peer.
    If skip_unavailable is set to True, when an index is reached which
    causes indexed_sender to return None, the sequential sender will keep
    advancing the output schedule until it reaches an index for which
    indexed_sender doesn't return None, until it has gone through the entire
    schedule.

    Warning: if indexed_sender is mutated by other code, this filter is only
    safe to use in synchronous environments, such as part of another Filter
    which completely owns indexed_sender.
    """

    _logger = logging.getLogger('.'.join((__name__, 'TimedSequentialSender')))

    output_schedule: Iterable[_Index] = attr.ib()
    indexed_sender: IndexedSender[_Index] = attr.ib()
    output_interval: float = attr.ib(default=0)
    skip_unavailable: bool = attr.ib(default=False)
    _sender: SequentialSender[_Index] = attr.ib()
    _current_time: Optional[float] = attr.ib(default=None)
    _last_output_time: Optional[float] = attr.ib(default=None)

    @_sender.default
    def init_sender(self) -> SequentialSender[_Index]:
        """Initialize the sequential sender."""
        return SequentialSender(
            output_schedule=self.output_schedule,
            indexed_sender=self.indexed_sender,
            skip_unavailable=self.skip_unavailable
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
