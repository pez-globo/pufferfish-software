"""Appication sub-layer for list synchronization."""

import collections
import itertools
import logging
from typing import Deque, Generic, List, Optional, Type, TypeVar

import attr

from typing_extensions import Protocol

from ventserver.protocols import events
from ventserver.protocols import exceptions
from ventserver.sansio import channels
from ventserver.sansio import protocols


# Structural Subtypes


class Indexed(Protocol):
    """Structural subtype for protobuf messages with an id field."""

    id: int  # pylint: disable=invalid-name


_ListElement = TypeVar('_ListElement', bound=Indexed)


class ListSegment(Protocol[_ListElement]):
    """Structural subtype for segments of lists.

    Any protobuf message should conform to this interface.
    """

    next_expected: int
    total: int
    remaining: int
    elements: List[_ListElement]


class EphemeralListSegment(Protocol[_ListElement]):
    """Structural subtype for segments of ephemeral lists.

    Any protobuf message should conform to this interface.
    """

    next_expected: int
    total: int
    remaining: int
    session_id: int
    elements: List[_ListElement]


# List Sending


@attr.s
class UpdateEvent(events.Event, Generic[_ListElement]):
    """State update event."""

    next_expected: Optional[int] = attr.ib(default=None)
    session_id: int = attr.ib(default=0)
    new_elements: List[_ListElement] = attr.ib(default=attr.Factory(list))

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.next_expected is not None or len(self.new_elements) > 0


@attr.s
class SendSynchronizer(
        protocols.Filter[UpdateEvent[_ListElement], ListSegment[_ListElement]]
):
    """List synchronization filter for sending lists.

    Inputs are descriptions received from the receiving peer about the id of the
    next list element it expects as well as any new element to add to the list.
    The id of the next expected list element should never decrease, unless it
    is reset to 0 (which is equivalent to requesting all list elements).
    The id of the next element to add to the list should never decrease (and it
    should preferably increment by 1).
    Outputs are the next elements to send to the peer. An output is only
    generated in response to an UpdateEvent with a next_expected!
    """

    ID_TYPE_SIZE = 32
    MAX_ID = 2 ** ID_TYPE_SIZE - 1  # max for uint32

    _logger = logging.getLogger('.'.join((__name__, 'Sender')))

    segment_type: Type[ListSegment[_ListElement]] = attr.ib()
    max_len: int = attr.ib(default=MAX_ID + 1)
    max_segment_len: int = attr.ib(default=256)
    _elements: Deque[_ListElement] = attr.ib(factory=collections.deque)
    _next_expected: int = attr.ib(default=0)
    _next_expected_index: int = attr.ib(default=0)
    _buffer: channels.DequeChannel[UpdateEvent[_ListElement]] = attr.ib(
        factory=channels.DequeChannel
    )

    @max_len.validator
    def _validate_max_len(self, _: 'attr.Attribute[int]', value: int) -> None:
        """Ensure that the max len field has an allowed value."""
        if value < 0 or value > self.MAX_ID + 1:
            raise ValueError(
                'max_len must be between 0 and {}'.format(self.MAX_ID + 1)
            )

    def input(self, event: Optional[UpdateEvent[_ListElement]]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        for element in event.new_elements:
            self._add_element(element)

        if event.next_expected is None:
            return

        self._buffer.input(event)

    def output(self) -> Optional[ListSegment[_ListElement]]:
        """Emit the next output event."""
        event = self._buffer.output()
        if event is None:
            return None

        if event.next_expected is None:
            return None

        if (
                event.next_expected != 0
                and event.next_expected < self._next_expected
        ):
            raise exceptions.ProtocolDataError(
                'Next expected event id cannot decrease from {} to {}!'
                .format(self._next_expected, event.next_expected)
            )

        self._next_expected = event.next_expected
        self._advance_next_expected_index()

        output_event = self.segment_type()
        output_event.next_expected = self._next_expected
        output_event.total = len(self._elements)
        output_event.elements = list(itertools.islice(
            self._elements,
            self._next_expected_index,
            self._next_expected_index + self.max_segment_len
        ))
        output_event.remaining = len(self._elements) - self._next_expected_index
        self._logger.debug('Sending: %s', output_event)
        return output_event

    def _advance_next_expected_index(self) -> None:
        """Advance the next_expected index to the next expected id."""
        if self._next_expected == 0:
            self._next_expected_index = 0
            return

        while True:
            if self._next_expected_index >= len(self._elements):
                return

            current_element = self._elements[self._next_expected_index]
            if current_element.id >= self._next_expected:
                return

            self._next_expected_index += 1

    def _add_element(self, element: _ListElement) -> None:
        """Attempt to add the element to the end of the list."""
        while len(self._elements) >= self.max_len:
            oldest_element = self._elements.popleft()
            if self._next_expected_index > 0:
                self._next_expected_index -= 1
            else:
                self._logger.warning(
                    'Discarding without sending: %s', oldest_element
                )
        self._elements.append(element)
        self._logger.debug('Added: %s', element)


@attr.s
class ReceiveSynchronizer(protocols.Filter[
        EphemeralListSegment[_ListElement], UpdateEvent[_ListElement]
]):
    """List synchronization filter for receiving ephemeral lists.

    Inputs are the next elements received from the sending peer.
    Outputs are descriptions for the sending peer about the id of the next
    list element it expects as well as any new elements received. The id of the
    next expected list element should always be one greater than the id of the
    last received list element. A received element is only considered new if
    its id is greater than or equal to the id of the next expected list element,
    or if it was delivered with a fresh session ID.

    Ephemeral lists are generated by senders which lack a persistent store for
    the list they are sending, and thus will reset their event ID counter back
    to 0 when the sending peer is reset (or when the event ID rolls over back to
    0). These senders provide a session ID, and they only discard their events
    when the expected log event ID matches their current session.
    """

    _logger = logging.getLogger('.'.join((__name__, 'Receiver')))

    _next_expected: int = attr.ib(default=0)
    _session_id: int = attr.ib(default=0)
    _buffer: channels.DequeChannel[
        EphemeralListSegment[_ListElement]
    ] = attr.ib(factory=channels.DequeChannel)

    def input(
            self, event: Optional[EphemeralListSegment[_ListElement]]
    ) -> None:
        """Handle input events."""
        if event is None:
            return

        self._buffer.input(event)

    def output(self) -> Optional[UpdateEvent[_ListElement]]:
        """Emit the next output event."""
        event = self._buffer.output()
        if event is None:
            return None

        if event.session_id != self._session_id:
            # Reset the receiver
            if self._session_id != 0:
                self._logger.warning(
                    'Reset session ID from %s to %s',
                    self._session_id, event.session_id
                )
            self._next_expected = 0
            self._session_id = event.session_id

        new_elements = []
        for element in event.elements:
            if element.id >= self._next_expected:
                self._next_expected = element.id + 1
                new_elements.append(element)

        return UpdateEvent(
            next_expected=self._next_expected, new_elements=new_elements,
            session_id=self._session_id
        )
