"""Filters for buffering discrete data items or other events.

Provides Filters whose behavior is only to buffer inputs and make them available
as outputs without any transformation.

    Typical usage example:

    bounded_channel = DequeChannel(maxlen=10)
    bounded_channel.input('foo')
    bounded_channel.input('foo')
    bounded_channel.input('bar')
    bounded_channel.input('foo')
    bounded_channel.input('hello')
    print(bounded_channel.output())  # 'foo'
    for string in bounded_channel.output_all():
        print(string)  # 'foo', 'bar', 'foo', 'hello'

    unbounded_channel = DequeChannel()
"""

import collections
from typing import Deque, Optional, TypeVar

import attr

from ventserver.sansio import protocols


# Typing


_Data = TypeVar('_Data')


# Channels


@attr.s
class DequeChannel(protocols.Filter[_Data, _Data]):
    """A Filter to buffer input events.

    This Filter uses a deque to buffer provided inputs and make them available
    as outputs, without any transformation of events.

    Attributes:
        maxlen: An optional integer indicating the maximum length of the deque
            buffer. If provided, any input data provided when the buffer is full
            will be rejected.

    """

    maxlen: Optional[int] = attr.ib(default=None)
    _buffer: Deque[_Data] = attr.ib(factory=collections.deque)

    def input(self, event: Optional[_Data]) -> None:
        """Push a data item onto the channel.

        Args:
            event: An optional data item to store on the buffer. If it is None,
                it will be ignored and the buffer will not be modified.

        Raises:
            IndexError: The input data item can't be stored on the buffer
                because the buffer is full.

        """
        if event is None:
            return

        if self.maxlen is not None and len(self._buffer) >= self.maxlen:
            raise IndexError('DequeChannel is full!')
        self._buffer.append(event)

    def output(self) -> Optional[_Data]:
        """Remove the next data item from the buffer and return it.

        Returns:
            The next data item on the buffer, or None if the buffer is empty.

        """
        try:
            return self._buffer.popleft()
        except IndexError:
            return None
