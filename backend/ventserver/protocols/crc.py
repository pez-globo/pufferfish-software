"""Filter to generate protected bytes using crc function."""
# TODO: rename certain variables derived from datagram filter

import logging
import struct
from typing import Optional, Callable, Any
from crcmod import predefined  # type: ignore

import attr

from ventserver.protocols import exceptions
from ventserver.sansio import channels
from ventserver.sansio import protocols

# CRC generating

@attr.s
class CRCSender(protocols.Filter[bytes, bytes]):
    """Generates the crc for incoming bytes data and attaches 
    the generated crc key to the message and returns it back."""

    _logger = logging.getLogger(".".join((__name__, "CRCSender")))

    _HEADER_FORMAT = '> L'
    _HEADER_PARSER = struct.Struct(_HEADER_FORMAT)
    HEADER_SIZE = struct.calcsize(_HEADER_FORMAT)
    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )
    _crc_func: Callable[[bytes], Any] = attr.ib(
        default=predefined.mkCrcFun('crc-32c')
    )

    def input(self, event: Optional[bytes]) -> None:
        """Stores input data in internal buffer."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[bytes]:
        """Extracts the next event in the buffer and processes it.""" 

        event = self._buffer.output()
        assert isinstance(event, bytes), (
            "{event} is expected to be a bytes object."
        )
        try:
            crc_key = int(self._crc_func(event)) # raise error
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

@attr.s
class CRCReceiver(protocols.Filter[bytes, bytes]):
    """Generates the crc for incoming bytes data and attaches 
    the generated crc key to the message and returns it back."""

    _logger = logging.getLogger(".".join((__name__, "CRCReceiver")))

    _HEADER_FORMAT = '> L'
    _HEADER_PARSER = struct.Struct(_HEADER_FORMAT)
    HEADER_SIZE = struct.calcsize(_HEADER_FORMAT)
    _buffer: channels.DequeChannel[bytes] = attr.ib(
        factory=channels.DequeChannel
    )
    _crc_func: Callable[[bytes], Any] = attr.ib(
        default=predefined.mkCrcFun('crc-32c')
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
        
        crc_key = None
        try:
            crc_key = self._HEADER_PARSER.unpack(event[:self.HEADER_SIZE])[0]
        except struct.error as exc:
            raise exceptions.ProtocolDataError(
                'Unparseable header: {!r}'.format(event[:self.HEADER_SIZE])
            ) from exc

        calculated_crc = self._crc_func(event[self.HEADER_SIZE:])
        if not crc_key != calculated_crc:
            raise exceptions.ProtocolDataError(
                'The specified CRC of the datagram\'s protected section, '
                '0x{:08x}, is inconsistent with the actual computed CRC '
                'of the received protected section, 0x{:08x}'
                .format(crc_key, calculated_crc)
            )

        message = event[self.HEADER_SIZE:]

        return message 
