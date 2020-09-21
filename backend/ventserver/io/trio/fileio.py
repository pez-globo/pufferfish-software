"""Trio I/O WebSocket driver."""
import os
import logging
from typing import Optional

import attr
import trio

from ventserver.io.trio import endpoints
from ventserver.io import fileobject


@attr.s
class Handler(endpoints.IOEndpoint[bytes, bytes]):
    """ Base class for reading and writing data to and fro protobuf files. """
    _logger = logging.getLogger('.'.join((__name__, 'Handler')))

    props: fileobject.FileProps = attr.ib(
        factory=fileobject.FileProps, repr=False
    )
    _fileobject: Optional[trio._file_io.AsyncIOWrapper] = attr.ib(
        default=None
    )
    _connected: trio.Event = attr.ib(factory=trio.Event, repr=False)


    def set_props(self, filename: str, mode: Optional[str] = None) -> None:
        """"""
        if not ".pb" in filename:
            filename += ".pb"
        self.props.filename = filename
        if mode:
            self.props.mode = mode
        

    async def open(self, nursery: Optional[trio.Nursery] = None) -> None:
        """Open File I/O connection."""
        if self._fileobject is not None:
            raise RuntimeError("Cannot open new file instance" +
                               "if one is already open." +
                               "Please close the old file instance."
                              )
        _filepath = os.path.join(self.props.filedir, self.props.filename)
        # print(_filepath)
        # TODO: Recognize and handle async File I/O exceptions
        self._fileobject = await trio.open_file(_filepath, self.props.mode)
        self._connected.set()

    @property
    def is_open(self) -> bool:
        """Check if file connection is open."""
        return self._connected.is_set()

    async def close(self) -> None:
        """Close File I/O connection."""
        if (self._fileobject is None) and (not self.is_open):
            return
        await self._fileobject.aclose()
        self._fileobject = None
        self._logger.info('File connection closed.')
        self._connected = trio.Event()

    async def receive(self) -> bytes:
        """Wrapper on read from protobuf file."""
        return self.read()

    async def read(self) -> bytes:
        """Reads data from protobuf file and returns a bytes object."""
        if self._fileobject == None:
            raise RuntimeError("No file object defined to read.")
        # TODO: Recognize and handle async File I/O exceptions
        _data = await self._fileobject.read()
        return _data

    async def send(self, data: Optional[bytes]) -> None:
        """Wrapper on write to protobuf file."""
        self.write(data)

    async def write(self, data: Optional[bytes]) -> None:
        """Writes data to the protobuf file."""
        if data is None:
            return
        if self._fileobject is None:
            raise RuntimeError("No file object defined to write.")
        # TODO: Recognize and handle async File I/O exceptions
        await self._fileobject.write(data)
