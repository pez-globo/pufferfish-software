"""Mock sequential implementations of I/O endpoints."""

import logging
import time
from typing import Optional, TypeVar

from ventserver.io.sequential import endpoints


# Typing


_SendData = TypeVar('_SendData')
_ReceiveData = TypeVar('_ReceiveData')


# Mocks


class IOEndpoint(endpoints.IOEndpoint[_SendData, _ReceiveData]):
    """Base class for sending data to and receiving data from I/O endpoints."""

    def open(self) -> None:
        """Open the I/O connection."""

    @property
    def is_open(self) -> bool:
        """Return whether or not the I/O connection is open."""
        return True

    def close(self) -> None:
        """Close the I/O connection."""


class SerialDriver(IOEndpoint[bytes, bytes]):
    """Sequential mock for serial I/O subsystem."""

    _logger = logging.getLogger('.'.join((__name__, 'SerialDriver')))

    def send(self, data: Optional[bytes]) -> None:  # pylint: disable=no-self-use
        """Send data over the serial connection."""
        if data is None:
            return
        if data:
            self._logger.debug('Wrote: %s', data)
        time.sleep(0.5)

    def receive(self) -> bytes:  # pylint: disable=no-self-use
        """Receive data from the serial connection."""
        time.sleep(0.5)
        read = b'new serial rx!\0another new serial rx!\0'
        if read:
            self._logger.debug('Read: %s', read)
        return read
