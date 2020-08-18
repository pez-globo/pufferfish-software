"""Defines a generic sequential serial driver interface."""

import logging
from typing import Optional

import attr

import serial  # type: ignore
from serial import serialutil

from ventserver.io import _serial
from ventserver.io.sequential import endpoints


@attr.s
class Driver(endpoints.IOEndpoint[bytes, bytes]):
    """Serial communication driver."""

    _logger = logging.getLogger('.'.join((__name__, 'Driver')))

    props: _serial.SerialProps = attr.ib(factory=_serial.SerialProps)
    _connection: Optional[serial.Serial] = attr.ib(default=None)

    def open(self) -> None:
        """Open connection to the Serial device."""
        if self._connection is not None:
            raise RuntimeError(
                'Cannot open a serial connection which is already open!'
            )
        if not self.props.port:
            self.props.port = _serial.find_port(self.props.port_filter)
        self._connection = serial.Serial(
            port=self.props.port,
            baudrate=self.props.baud_rate,
            timeout=self.props.read_timeout or 0.1,
            write_timeout=self.props.write_timeout or 0.1
        )

    @property
    def is_open(self) -> bool:
        """Return whether or not the Serial device connection is open."""
        return bool(self._connection)

    def close(self) -> None:
        """Close the connection to the Serial device."""
        if self._connection is None:
            return
        self._connection.close()
        self._connection = None

    def send(self, data: Optional[bytes]) -> None:
        """Send data over the serial connection."""
        if self._connection is None:
            raise RuntimeError(
                'Cannot send data without an open serial connection!'
            )
        if data is None:
            return

        try:
            self._connection.write(data)
        except serialutil.SerialException:
            self.close()
            raise BrokenPipeError(
                'Serial connection lost in write!', self.props.port
            )
        if data:
            self._logger.debug('Wrote: %s', data)

    def receive(self) -> bytes:
        """Receive data from the serial connection."""
        if self._connection is None:
            raise RuntimeError(
                'Cannot receive data without an open serial connection!'
            )

        read = b''
        try:
            if self.props.message_delimiter is None:
                read = self._connection.read(
                    size=self.props.message_length
                )
            else:
                read = self._connection.read_until(
                    self.props.message_delimiter, size=self.props.message_length
                )
        except serialutil.SerialException:
            self.close()
            raise BrokenPipeError(
                'Serial connection lost in read!', self.props.port
            )
        if read:
            self._logger.debug('Read: %s', read)
        return read
