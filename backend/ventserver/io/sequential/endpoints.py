"""Sequential I/O endpoint interfaces."""

import abc
import types
from typing import Generic, Optional, Type, TypeVar


# Typing


_SendData = TypeVar('_SendData')
_ReceiveData = TypeVar('_ReceiveData')


# I/O Endpoints


class IOEndpoint(Generic[_SendData, _ReceiveData], abc.ABC):
    """Base class for sending data to and receiving data from I/O endpoints."""

    @abc.abstractmethod
    def open(self) -> None:
        """Open the I/O connection."""

    @property
    @abc.abstractmethod
    def is_open(self) -> bool:
        """Return whether or not the I/O connection is open."""

    @abc.abstractmethod
    def close(self) -> None:
        """Close the I/O connection."""

    def __enter__(self) -> 'IOEndpoint[_SendData, _ReceiveData]':
        """Create a context manager resource."""
        if not self.is_open:
            self.open()
        return self

    def __exit__(
            self, exc_type: Optional[Type[BaseException]],
            exc: Optional[BaseException], exc_tb: Optional[types.TracebackType]
    ) -> None:
        """Tear down the connection when the context manager exits."""
        self.close()

    @abc.abstractmethod
    def send(self, data: Optional[_SendData]) -> None:
        """Send data over the endpoint.

        If this is called with None, nothing should happen.
        """

    @abc.abstractmethod
    def receive(self) -> _ReceiveData:
        """Receive data from the endpoint."""
