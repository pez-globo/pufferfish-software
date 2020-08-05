"""Filters for buffering bytes.

Provides Filters whose behavior is only to buffer input bytes and make them
available as individual output bytes without any transformation. Unlike the
Filters from the ventserver.sansio.channels module, streams provide an interface
oriented towards inputting bytes from a continuous sequence of bytes, rather
than treating every byte as a discrete event.

    Typical usage example:

    bounded_stream = BytearrayStream(maxlen=10)
    bounded_stream.input(b'1234')
    print(bounded_stream.output())  # b'1'
    for byte in bounded_stream.output_all():
        print(byte)  # b'2', b'3', b'4'

    unbounded_stream = BytearrayStream()
"""

from typing import Optional

import attr

from ventserver.sansio import protocols


# Streams


@attr.s
class BytearrayStream(protocols.Filter[bytes, bytes]):
    """A Filer to buffer input bytes.

    This Filter uses a bytearray to merge and buffer provided input bytestrings
    and make them available as byte-by-byte outputs, without any transformation
    of bytes.

    Attributes:
        maxlen: An optional integer indicating the maximum length of the
            bytearray buffer. If provided, any input data provided which would
            overfill the buffer will be rejected.

    """

    maxlen: Optional[int] = attr.ib(default=None)
    _buffer: bytearray = attr.ib(factory=bytearray)

    def input(self, event: Optional[bytes]) -> None:
        """Push bytes on the stream.

        Args:
            event: An optional bytestring to store on the buffer. If it is None,
                it will be ignored and the buffer will not be modified. Either
                all bytes will be stored on the buffer, or (if the input is too
                long for the buffer) none of them will be stored on the buffer.

        Raises:
            IndexError: The input bytes can't be stored on the buffer because
                the buffer would be overfilled if all of them were stored.

        """
        if event is None:
            return

        if self.maxlen is None:
            allowed = len(event)
        else:
            allowed = max(0, self.maxlen - len(self._buffer))
        if allowed < len(event):
            raise IndexError('BytearrayStream is full!')
        self._buffer.extend(event[:allowed])

    def output(self) -> Optional[bytes]:
        """Remove the next byte from the buffer and return it.

        Returns:
            The next byte on the buffer, or None if the buffer is empty.

        """
        if not self._buffer:
            return None

        data = self._buffer[0:1]
        self._buffer[0:1] = b''  # Fast buffer deletion
        return data
