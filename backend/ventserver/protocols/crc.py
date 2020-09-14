"""Filter to generate protected bytes using crc function.

# TODO: Make changes to the docstrings
"""

import logging
import struct
from typing import Optional, Any, Callable
from crcmod import predefined  # type: ignore

import attr

from ventserver.protocols import exceptions
from ventserver.sansio import channels
from ventserver.sansio import protocols


CRCFUNC = predefined.mkCrcFun('crc-32c')

def long_attr(
        _: Any, __: 'attr.Attribute[bytes]', value: bytes
) -> None:
    """Validate the attr input as a byte re.

    Raises:
        ValueError: attr init value cannot be represented as a single byte.

    """
    # TODO: need a better way for this validator
    if len(bytes(value)) != 4:
        raise ValueError(
            'Attr must be a bytes representation of unsigned long: {!r}'
            .format(value)
        )


@attr.s
class CRCElement:
    """CRC element class defines crc properties of and crc
    operations on data."""

    _HEADER_FORMAT = '> L'
    _HEADER_PARSER = struct.Struct(_HEADER_FORMAT)
    HEADER_SIZE = struct.calcsize(_HEADER_FORMAT)
    
    payload: bytes = attr.ib(default=b'')

    crc: bytes = attr.ib(
        default=b'\x00\x00\x00\x00',
        validator=[long_attr], repr=(
            lambda value: '0x{:08x}'.format(value)  # pylint: disable=unnecessary-lambda
        )
    )


    def parse(self, body: bytes) -> None:
        """Exxtracts CRC and payload from incoming message."""
        self.crc = body[:self.HEADER_SIZE]
        self.payload = body[self.HEADER_SIZE:]

    def _compute_crc(self) -> bytes:
        """Computes CRC for the payload."""
        try:
            crc_key = int(CRCFUNC(self.payload)) # raise error
        except TypeError as terr:
            raise exceptions.ProtocolDataError(
                'Could not compute crc: {0}'
                .format(terr))
        parsed_key = None
        try:
            parsed_key = self._HEADER_PARSER.pack(crc_key)
        except struct.error as exc:
            raise exceptions.ProtocolDataError(
                'Could not pack header fields: crc=0x{:x}'
                .format(crc_key)
            ) from exc

        return parsed_key

    def check_integrity(self) -> None:
        """Matches computed CRC with that extracted from message."""

        computed_crc = self._compute_crc()
        if self.crc != computed_crc:
            raise exceptions.ProtocolDataError(
                'The specified CRC of the datagram\'s protected section, '
                '{!r}, is inconsistent with the actual computed CRC '
                'of the received protected section, {!r}'
                .format(self.crc, computed_crc)
            )

    def get_body(self) -> bytes:
        """Returns the CRC combined with the payload."""
        self.crc = self._compute_crc() # raises ProtocolDataError
        return self.crc + self.payload

# CRC Filters

@attr.s
class CRCSender(protocols.Filter[bytes, bytes]):
    """Generates the crc for incoming bytes data and attaches
    the generated crc key to the message and returns it back."""
    _logger = logging.getLogger(".".join((__name__, "CRCSender")))

    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[bytes]) -> None:
        """Stores input data in internal buffer."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Extracts the next event in the buffer and processes it."""

        event = self._buffer.output()
        if not event:
            return None
        assert isinstance(event, bytes), (
            "{!r} is expected to be a bytes object.".format(event)
        )
        crc_element = CRCElement(payload=event)
        message = crc_element.get_body() # raises ProtocolDataError
        return message

@attr.s
class CRCReceiver(protocols.Filter[bytes, bytes]):
    """Generates the crc for incoming bytes data and attaches
    the generated crc key to the message and returns it back."""

    _logger = logging.getLogger(".".join((__name__, "CRCReceiver")))

    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )

    def input(self, event: Optional[bytes]) -> None:
        """Stores input data in internal buffer."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Extracts the next event in the buffer and processes it."""

        event = self._buffer.output()
        if not event:
            return None
        crc_element = CRCElement()
        crc_element.parse(event)
        crc_element.check_integrity() # raises ProtocolDataError
        return crc_element.payload
