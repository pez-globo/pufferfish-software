"""Filters for handling data framing over a byte stream.

Provides Filters which demarcate chunks as bytestrings with a byte delimiter
(such as the null byte) and which encode arbitrary data payloads (limited by
payload length) so that they don't contain the null byte. Together, these
Filters can be used to send arbitrary data payloads (up to a certain length)
through a bytestream over the wire while tolerating and being able to recover
from frame synchronization errors which may occur due to dropping or corruption
of bytes on the wire.

A Frame is defined as the data unit of the framing layer, consisting of a
COBS-encoded payload followed by a null byte trailer which indicates the end of
the frame.

    Typical usage example:

    cobs_encoder = COBSEncoder()
    chunk_merger = ChunkMerger()
    chunk_splitter = ChunkSplitter()
    cobs_decoder = COBSDecoder()

    # Send direction
    cobs_encoder.input(bytes([0x00])
    cobs_encoder.input(bytes([0x78]))
    for encode_output in cobs_encoder.output_all():
        print(encode_output)  # bytes([0x01, 0x01]), bytes([0x02, 0x78])
        chunk_merger.input(encode_output)
    merged = chunk_merger.output()
    print(merged)  # bytes([0x01, 0x01, 0x00, 0x02, 0x78, 0x00])

    # Send direction exception
    cobs_encoder.input(bytes([0xff for i in range(300)]))
    try:
        cobs_encoder.output()
    except ventserver.exceptions.ProtocolDataError:
        print('Invalid payload!')  # 'Invalid payload!'

    # Receive direction
    chunk_splitter.input(merged)
    for split_output in chunk_splitter.output_all()
        print(split_output)  # bytes([0x01, 0x01]), bytes([0x02, 0x78])
        cobs_decoder.input(split_output)
    for decode_output in cobs_decoder.output_all():
        print(decode_output)  # bytes([0x00]), bytes([0x78])

    # Receive direction exception
    cobs_decoder.input(bytes([0x02]))
    try:
        payload = cobs_decoder.output()
    except ventserver.exceptions.ProtocolDataError:
        print('Invalid body!')  # 'Invalid body!'

"""

import logging
from typing import Optional

import attr

from cobs import cobs  # type: ignore

from ventserver.protocols import exceptions
from ventserver.sansio import channels
from ventserver.sansio import protocols
from ventserver.sansio import streams


# Constants


CHUNK_MAX_SIZE = 256
FRAME_PAYLOAD_MAX_SIZE = CHUNK_MAX_SIZE - 2


# Chunk Delimiting


@attr.s
class ChunkSplitter(protocols.Filter[bytes, bytes]):
    """A Filter to split delimited chunks of bytes from a bytestream.

    This Filter buffers input bytes and processes them to return zero or one
    delimited chunks of bytes with every call of the output method. A chunk is
    defined as a bytestring which does not contain the delimiter byte but is
    surrounded by delimiter bytes in the input bytestream.

    Attributes:
        delimiter: A bytestring containing a single byte which will be treated
            as the delimiter between chunks in the bytestream for the input.
        current_chunk: A bytearray containing the current buffer of bytes being
            collected for the next chunk to be output once a delimiter byte is
            seen in a future call of the output method. This attribute should
            not be modified by calling code!

    Logging Statements:
        DEBUG: Every not-None bytestring returned by the output method.

    """

    _logger = logging.getLogger('.'.join((__name__, 'ChunkSplitter')))

    delimiter: bytes = attr.ib(default=b'\0')
    current_chunk: bytearray = attr.ib(factory=bytearray, init=False)
    _buffer: streams.BytearrayStream = attr.ib(factory=streams.BytearrayStream)

    @delimiter.validator
    def _check_delimiter(  # pylint: disable=no-self-use
            self, _: 'attr.Attribute[bytes]', value: bytes
    ) -> None:
        """Validate the attr input for delimiter.

        Raises:
            ValueError: attr init value does not consist of exactly one byte.

        """
        if len(value) != 1:
            raise ValueError(
                'Delimiter must consist of exactly one byte: {!r}'.format(value)
            )

    def input(self, event: Optional[bytes]) -> None:
        """Store input bytes in the internal buffer.

        Args:
            event: An optional bytestring of bytes from a bytestream. If it is
            None, it will be ignored and the buffer will not be modified.

        Raises:
            IndexError: The input data item can't be stored on the buffer
                because the buffer is full.

        """
        self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Produce the next complete chunk from bytes in the internal buffer.

        Returns:
            An optional bytestring of the next chunk, excluding any delimiters.
            Returns None if there is not yet a complete next chunk available.

        """
        while True:
            next_byte = self._buffer.output()
            if next_byte is None:
                return None

            if next_byte == self.delimiter:
                chunk = bytes(self.current_chunk)
                self.current_chunk[:] = b''
                self._logger.debug(chunk)
                return chunk

            self.current_chunk.extend(next_byte)


@attr.s
class ChunkMerger(protocols.Filter[bytes, bytes]):
    """A Filter to merge chunks of bytes into a delimited bytestream.

    This Filter buffers input chunks and delimits them to return zero
    or one bytestream strings with every call of the output method. A chunk is
    defined as a bytestring which does not contain the delimiter byte but is
    surrounded by delimiter bytes in the input bytestream.

    Logging Statements:
        DEBUG: Every not-None bytestring returned by the output method.

    """

    _logger = logging.getLogger('.'.join((__name__, 'ChunkMerger')))

    delimiter: bytes = attr.ib(default=b'\0')
    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[bytes]) -> None:
        """Store input chunks in the internal buffer.

        Args:
            event: An optional chunk represented as a bytestring. If it is None,
            it will be ignored and the buffer will not be modified.

        Raises:
            IndexError: The input data item can't be stored on the buffer
                because the buffer is full.

        """
        self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Produce the next chunk with a delimiter added at the end.

        Returns:
            An optional bytestring of the next available chunk in the internal
            buffer, with a delimiter added to mark the end of the chunk. Returns
            None if there are no chunks available in the internal buffer.

        """
        chunk = self._buffer.output()
        if chunk is None:
            return None

        output = chunk + self.delimiter
        self._logger.debug(output)
        return output


# COBS Encoding


@attr.s
class COBSDecoder(protocols.Filter[bytes, bytes]):
    """A Filter to decode a COBS-encoded bytestring into a payload.

    This Filter buffers input bytestrings and performs COBS decoding on them to
    return zero or one payload bytestrings with every call of the output method.

    Logging Statements:
        DEBUG: Every not-None bytestring returned by the output method.

    """

    _logger = logging.getLogger('.'.join((__name__, 'COBSDecoder')))

    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[bytes]) -> None:
        """Store input COBS-encoded bytestrings in the internal buffer.

        Args:
            event: An optional COBS-encoded body represented as a bytestring. If
            it is None, it will be ignored and the buffer will not be modified.

        Raises:
            IndexError: The input data item can't be stored on the buffer
                because the buffer is full.

        """
        self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Produce the next COBS-decoded payload.

        Returns:
            An optional bytestring of the COBS-decoded payload from the next
            available COBS-encoded body in the internal buffer. Returns None if
            there are no bodies available in the internal buffer.

        Raises:
            exceptions.ProtocolDataError: the body is obviously incorrect for
                COBS decoding; note that it is possible that a body may have
                have accumulated byte errors which would silently cause an
                incorrect COBS decoding but would not be detected by the decoder
                and thus would not raise an exception.

        """
        frame = self._buffer.output()
        if frame is None:
            return None

        try:
            decoded: bytes = cobs.decode(frame)
        except cobs.DecodeError as exc:
            raise exceptions.ProtocolDataError(
                'Could not COBS-decode body: {!r}'.format(frame)
            ) from exc
        except TypeError as err:
            raise exceptions.ProtocolDataError(
                'Could not COBS-encode body: required type bytes found type {}'.
                format(type(frame))
            ) from err
        self._logger.debug(decoded)
        return decoded


@attr.s
class COBSEncoder(protocols.Filter[bytes, bytes]):
    """A Filter to encode a bytestring payload into a COBS-encoded body.

    This Filter buffers input bytestrings and performs COBS encoding on them to
    return zero or one body bytestrings with every call of the output method.

    Logging Statements:
        DEBUG: Every not-None bytestring returned by the output method.

    """

    _logger = logging.getLogger('.'.join((__name__, 'COBSEncoder')))

    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[bytes]) -> None:
        """Store input events in the internal buffer.

        Args:
            event: An optional payload represented as a bytestring. If it is
            None, it will be ignored and the buffer will not be modified.

        Raises:
            IndexError: The input data item can't be stored on the buffer
                because the buffer is full.

        """
        self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Produce the next COBS-encoded frame body.

        Returns:
            An optional bytestring of the COBS-encoded body from the next
            available payload in the internal buffer. Returns None if there are
            no payloads available in the internal buffer.

        Raises:
            exceptions.ProtocolDataError: the payload is too long for COBS
                encoding.

        """
        payload = self._buffer.output()
        if payload is None:
            return None

        if len(payload) > FRAME_PAYLOAD_MAX_SIZE:
            raise exceptions.ProtocolDataError(
                'Frame payload for COBS encoding is too long ({} bytes)!'
                .format(len(payload))
            )

        try:
            encoded: bytes = cobs.encode(payload)
        except TypeError as err:
            raise exceptions.ProtocolDataError(
                'Frames payload for COBS encoding should be bytes not {}'
                .format(type(payload))
            ) from err
        self._logger.debug(encoded)
        return encoded
