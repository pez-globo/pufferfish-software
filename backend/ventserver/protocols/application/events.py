"""Appication sub-layer for event synchronization."""

import dataclasses
import enum
import logging
from typing import Dict, Generic, Iterable, Iterator, Optional, Set, TypeVar

import attr

import betterproto

from ventserver.protocols.application import states


_Index = TypeVar('_Index', bound=enum.Enum)


@attr.s
class FilteredSender(states.IndexedSender[_Index]):
    """An overlay over IndexedSender to allow suppressing certain indices."""

    sender: states.IndexedSender[_Index] = attr.ib()
    allowed_indices: Set[_Index] = attr.ib()

    def __getitem__(self, key: _Index) -> Optional[betterproto.Message]:
        """Return the next output from the specified sender.

        Warning: this will not raise a KeyError if the index isn't in
        allowed_indices.
        """
        if key not in self.allowed_indices:
            return None

        return self.sender[key]

    def __len__(self) -> int:
        """Return the number of senders."""
        return len(self.allowed_indices)

    def __iter__(self) -> Iterator[_Index]:
        """Iterate over all indices."""
        return self.allowed_indices.__iter__()


@attr.s
class NotificationSender(states.Sender, Generic[_Index]):
    """Event notification sending filter.

    Inputs are lists of indices to mark as sendable. Outputs are state updates
    for the peer.

    If output_idle is set, when no states are sendable the sender will continue
    generating states as if all states are sendable.

    Warning: if indexed_sender is mutated by other code, this filter is only
    safe to use in synchronous environments, such as part of another Filter
    which completely owns indexed_sender.
    """

    _logger = logging.getLogger('.'.join((__name__, 'TimedSequentialSender')))

    output_schedule: Iterable[_Index] = attr.ib()
    indexed_sender: states.IndexedSender[_Index] = attr.ib()
    output_idle: bool = attr.ib(default=False)
    _sendable_indices: Set[_Index] = attr.ib(factory=set)
    _sendable_sender: states.SequentialSender[_Index] = attr.ib()
    _idle_sender: states.SequentialSender[_Index] = attr.ib()

    @_sendable_sender.default
    def init_sequential_sender(self) -> states.SequentialSender[_Index]:
        """Initialize the sequential sender."""
        return states.SequentialSender(
            output_schedule=self.output_schedule,
            indexed_sender=FilteredSender(
                sender=self.indexed_sender,
                allowed_indices=self._sendable_indices
            ),
            skip_unavailable=True
        )

    @_idle_sender.default
    def init_idle_sender(self) -> states.SequentialSender[_Index]:
        """Initialize the idle sender."""
        return states.SequentialSender(
            output_schedule=self.output_schedule,
            indexed_sender=self.indexed_sender,
            skip_unavailable=True
        )

    def input(self, event: Optional[_Index]) -> None:
        """Handle input events."""
        if event is None:
            return

        self._sendable_indices.add(event)

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        if not self._sendable_indices:
            return self._get_next_idle_output() if self.output_idle else None

        output = self._get_next_sendable_output()
        if output is None and self.output_idle:
            output = self._get_next_idle_output()
        return output

    def _get_next_sendable_output(self) -> Optional[betterproto.Message]:
        """Produce the next state from the sequential sender."""
        output = self._sendable_sender.output()
        index = self._sendable_sender.last_index
        if output is not None and index is not None:
            self._sendable_indices.discard(index)
            self._logger.debug('Sending notification: %s', index)
        return output

    def _get_next_idle_output(self) -> Optional[betterproto.Message]:
        """Produce the next state from the idle sender."""
        output = self._idle_sender.output()
        index = self._idle_sender.last_index
        if output is not None and index is not None:
            self._sendable_indices.discard(index)
        return output


@attr.s
class ChangedStateSender(states.Sender, Generic[_Index]):
    """Changed state event notification sending filter.

    Does not take inputs. Outputs are state updates for the peer.
    Outputs are available only when the outputs from all_states change.

    If output_idle is set, when no states have sendable changes, the sender will
    continue outputting all states as if they all had sendable changes. Sendable
    changes are detected by getting outputs from all indices in all_states;
    so all_states's __getitem__ should be an idempotent operation. In other
    words, all_states should be a dict of indices and states, or a store
    object with the same semantics; it should not be a MappedSenders object.

    Warning: because it relies on an all_states mutated by other code, this
    filter is only safe to use in synchronous environments, such as part of
    another Filter which completely owns all_states.
    """

    output_schedule: Iterable[_Index] = attr.ib()
    all_states: states.IndexedSender[_Index] = attr.ib()
    output_idle: bool = attr.ib(default=True)
    _notification_sender: NotificationSender[_Index] = attr.ib()
    _trackable_states: Set[_Index] = attr.ib()
    _prev_states: Dict[_Index, betterproto.Message] = attr.ib(factory=dict)

    @_notification_sender.default
    def init_notification_sender(self) -> NotificationSender[_Index]:
        """Initialize the notification sender."""
        return NotificationSender(
            output_schedule=self.output_schedule,
            indexed_sender=self.all_states, output_idle=self.output_idle
        )

    @_trackable_states.default
    def init_trackable_states(self) -> Set[_Index]:
        """Initialize the set of trackable states."""
        return set(self.output_schedule)

    def input(self, event: None) -> None:
        """Handle input events."""

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        for index in self._trackable_states:
            new_state = self.all_states[index]
            if (new_state is not None and (
                    index not in self._prev_states or
                    new_state != self._prev_states[index]
            )):
                self._notification_sender.input(index)
                self._prev_states[index] = dataclasses.replace(new_state)

        return self._notification_sender.output()
