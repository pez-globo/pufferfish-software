"""Sans-I/O ventilator backend server protocol."""

from typing import Optional, Union, Tuple
import logging

import attr

from ventserver.protocols import events, exceptions
from ventserver.protocols.backend import backend, connections
from ventserver.protocols.devices import file, frontend, mcu, rotary_encoder
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.sansio import channels, protocols


# Events


@attr.s
class GenericConnectionEvent(events.Event):
    """Generic connection status event."""

    connected: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


class FrontendConnectionEvent(GenericConnectionEvent):
    """Frontend connection status event."""


class MCUConnectionEvent(GenericConnectionEvent):
    """MCU connection status event."""


ConnectionEvent = Union[MCUConnectionEvent, FrontendConnectionEvent]


@attr.s
class ReceiveDataEvent(events.Event):
    """Server receive input event."""

    time: Optional[float] = attr.ib(default=None)
    serial_receive: Optional[bytes] = attr.ib(default=None)
    websocket_receive: Optional[bytes] = attr.ib(default=None)
    rotary_encoder_receive: Tuple[int, bool] = attr.ib(default=None)
    file_receive: Optional[file.LowerReceiveEvent] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.time is not None
            or bool(self.serial_receive)
            or self.websocket_receive is not None
            or self.rotary_encoder_receive is not None
            or self.file_receive is not None
        )


@attr.s
class ReceiveOutputEvent(events.Event):
    """Server receive output/send event."""

    server_send: Optional[backend.OutputEvent] = attr.ib(default=None)
    kill_frontend: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.server_send is not None and self.server_send.has_data()


ReceiveEvent = Union[ReceiveDataEvent, ConnectionEvent]
SendEvent = backend.OutputEvent


@attr.s
class SendOutputEvent(events.Event):
    """Server send output/send event."""

    serial_send: Optional[bytes] = attr.ib(default=None)
    websocket_send: Optional[bytes] = attr.ib(default=None)
    file_send: Optional[file.StateData] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            bool(self.serial_send) or
            self.websocket_send is not None or
            self.file_send is not None
        )


def make_serial_receive(
        serial_receive: bytes,
        time: float
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from serial receive data."""
    return ReceiveDataEvent(serial_receive=serial_receive, time=time)


def make_websocket_receive(
        ws_receive: bytes,
        time: float
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from websocket receive data."""
    return ReceiveDataEvent(websocket_receive=ws_receive, time=time)


def make_rotary_encoder_receive(
        re_receive: Tuple[int, bool],
        time: float
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from rotary encoder receive data."""
    return ReceiveDataEvent(rotary_encoder_receive=re_receive, time=time)


# Filters


@attr.s
class ReceiveFilter(protocols.Filter[ReceiveEvent, ReceiveOutputEvent]):
    """Filter which transforms receive bytes into high-level events."""
    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    # TODO: can we remove _buffer?
    _buffer: channels.DequeChannel[ReceiveEvent] =\
        attr.ib(factory=channels.DequeChannel)

    current_time: float = attr.ib(default=0)
    _backend: backend.ReceiveFilter = attr.ib(factory=backend.ReceiveFilter)

    # Devices
    _mcu: mcu.ReceiveFilter = attr.ib(factory=mcu.ReceiveFilter)
    _frontend: frontend.ReceiveFilter = attr.ib(factory=frontend.ReceiveFilter)
    _file: file.ReceiveFilter = attr.ib(factory=file.ReceiveFilter)
    _rotary_encoder: rotary_encoder.ReceiveFilter = attr.ib(
        factory=rotary_encoder.ReceiveFilter
    )

    _connections: connections.TimeoutHandler = \
        attr.ib(factory=connections.TimeoutHandler)
    _connection_states: frontend_pb.BackendConnections = \
        attr.ib(factory=frontend_pb.BackendConnections)

    def input(self, event: Optional[ReceiveEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[ReceiveOutputEvent]:
        """Emit the next output event."""
        self._process_buffer()
        any_updated = self._process_devices()
        if not any_updated:
            self._backend.input(backend.ReceiveDataEvent(
                time=self.current_time
            ))
        # Process backend output until the backend has data to output or it
        # indicates that it has no more receive data to process.
        output = ReceiveOutputEvent()
        while True:
            output.server_send = self._backend.output()
            if output.server_send is None:
                break
            if output.server_send.has_data():
                break
        any_updated = any_updated or output.server_send is not None
        output.kill_frontend = self._process_connection_statuses()
        any_updated = any_updated or output.kill_frontend
        if not any_updated:
            return None

        return output

    def _process_buffer(self) -> None:
        """Process the next event in the input buffer."""
        event = self._buffer.output()
        if event is None:
            return

        if isinstance(event, MCUConnectionEvent):
            update_type = (
                connections.Update.MCU_CONNECTED if event.connected
                else connections.Update.MCU_DISCONNECTED
            )
            self._connections.input(connections.UpdateEvent(
                current_time=self.current_time, type=update_type
            ))
            return

        if isinstance(event, FrontendConnectionEvent):
            update_type = (
                connections.Update.FRONTEND_CONNECTED if event.connected
                else connections.Update.FRONTEND_DISCONNECTED
            )
            self._connections.input(connections.UpdateEvent(
                current_time=self.current_time, type=update_type
            ))
            return

        if event.time is not None:
            self.current_time = event.time
            self._connections.input(connections.UpdateEvent(
                current_time=self.current_time
            ))
        self._mcu.input(event.serial_receive)
        self._frontend.input(event.websocket_receive)
        self._rotary_encoder.input(rotary_encoder.ReceiveEvent(
            time=self.current_time, re_data=event.rotary_encoder_receive
        ))
        self._file.input(event.file_receive)

    def _process_mcu(self) -> bool:
        """Process the next event from the mcu protocol."""
        mcu_output = self._mcu.output()
        if mcu_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            time=self.current_time, mcu_receive=mcu_output,
            frontend_receive=None
        ))
        self._connections.input(connections.UpdateEvent(
            current_time=self.current_time, type=connections.Update.MCU_RECEIVED
        ))
        if not self._connection_states.has_mcu:
            self._backend.input(backend.ExternalLogEvent(
                time=self.current_time, active=False,
                code=mcu_pb.LogEventCode.backend_mcu_connection_down
            ))
            self._connection_states.has_mcu = True
            self._backend.input(backend.ReceiveDataEvent(
                time=self.current_time, server_receive=self._connection_states
            ))
        return True

    def _process_frontend(self) -> bool:
        """Process the next event from the frontend protocol."""
        frontend_output = self._frontend.output()
        if frontend_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            time=self.current_time, frontend_receive=frontend_output
        ))
        self._connections.input(connections.UpdateEvent(
            current_time=self.current_time,
            type=connections.Update.FRONTEND_RECEIVED
        ))
        if not self._connection_states.has_frontend:
            self._backend.input(backend.ExternalLogEvent(
                time=self.current_time, active=False,
                code=mcu_pb.LogEventCode.backend_frontend_connection_down
            ))
            self._backend.input(backend.ExternalLogEvent(
                time=self.current_time,
                code=mcu_pb.LogEventCode.backend_frontend_connection_up
            ))
            self._connection_states.has_frontend = True
            self._backend.input(backend.ReceiveDataEvent(
                time=self.current_time, server_receive=self._connection_states
            ))
        return True

    def _process_rotary_encoder(self) -> bool:
        """Process the next event from the rotary encoder."""
        rotary_encoder_output = self._rotary_encoder.output()
        if rotary_encoder_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            time=self.current_time, frontend_receive=rotary_encoder_output
        ))
        return True

    def _process_file(self) -> bool:
        """Process the next event from the file."""
        file_output = self._file.output()  # throws ProtocolDataError
        if file_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            time=self.current_time, file_receive=file_output
        ))
        return True

    def _process_devices(self) -> bool:
        """Process the next event from every device."""
        any_updated = False

        any_updated = self._process_mcu() or any_updated
        any_updated = self._process_frontend() or any_updated
        any_updated = self._process_rotary_encoder() or any_updated
        # TODO: why do we need an exception handler here?
        try:
            any_updated = self._process_file() or any_updated
        except exceptions.ProtocolDataError as err:
            self._logger.error(err)

        return any_updated

    def _process_connection_statuses(self) -> bool:
        """Handle any connection timeouts."""
        actions = self._connections.output()
        if actions.alarm_mcu:
            self._backend.input(backend.ExternalLogEvent(
                time=self.current_time, active=True,
                code=mcu_pb.LogEventCode.backend_mcu_connection_down
            ))
            self._connection_states.has_mcu = False
            self._backend.input(backend.ReceiveDataEvent(
                time=self.current_time, server_receive=self._connection_states
            ))
            self._backend.input(backend.AlarmMuteCancellationEvent(
                time=self.current_time, request=False
            ))
        if actions.alarm_frontend:
            self._backend.input(backend.ExternalLogEvent(
                time=self.current_time, active=True,
                code=mcu_pb.LogEventCode.backend_frontend_connection_down
            ))
            self._connection_states.has_frontend = False
            self._backend.input(backend.ReceiveDataEvent(
                time=self.current_time, server_receive=self._connection_states
            ))
            self._backend.input(backend.AlarmMuteCancellationEvent(
                time=self.current_time, request=True
            ))
        return actions.kill_frontend

    def input_serial(self, serial_receive: bytes) -> None:
        """Input a ReceiveDataEvent corresponding to serial data.

        This is just a convenience function intended for writing unit tests
        more concisely.
        """
        self.input(make_serial_receive(serial_receive, self.current_time))

    def input_websocket(self, websocket: bytes) -> None:
        """Input a ReceiveDataEvent corresponding to websocket data.

        This is just a convenience function intended for writing unit tests
        more concisely.
        """
        self.input(make_websocket_receive(websocket, self.current_time))

    @property
    def backend(self) -> backend.ReceiveFilter:
        """Return the backend receiver."""
        return self._backend

    @property
    def file(self) -> file.ReceiveFilter:
        """Return the file receiver"""
        return self._file


@attr.s
class SendFilter(protocols.Filter[SendEvent, SendOutputEvent]):
    """Filter which transforms high-level events into send bytes."""

    # TODO: can we remove _buffer?
    _buffer: channels.DequeChannel[SendEvent] = attr.ib(
        factory=channels.DequeChannel
    )

    _backend: backend.SendFilter = attr.ib(factory=backend.SendFilter)

    # Devices
    _mcu: mcu.SendFilter = attr.ib(factory=mcu.SendFilter)
    _frontend: frontend.SendFilter = attr.ib(factory=frontend.SendFilter)
    _file: file.SendFilter = attr.ib(factory=file.SendFilter)

    def input(self, event: Optional[SendEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[SendOutputEvent]:
        """Emit the next output event."""
        any_updated = False
        self._process_buffer()
        backend_output = self._backend.output()
        any_updated = (backend_output is not None) or any_updated

        self._mcu.input(backend.get_mcu_send(backend_output))
        mcu_output = self._mcu.output()
        any_updated = (mcu_output is not None) or any_updated

        self._frontend.input(backend.get_frontend_send(backend_output))
        frontend_output = self._frontend.output()
        any_updated = (frontend_output is not None) or any_updated

        self._file.input(backend.get_file_send(backend_output))
        file_output = self._file.output()
        any_updated = (file_output is not None) or any_updated

        if not any_updated:
            return None

        output = SendOutputEvent(
            serial_send=mcu_output, websocket_send=frontend_output,
            file_send=file_output
        )
        return output

    def _process_buffer(self) -> None:
        """Process the next event in the input buffer."""
        try:
            event = self._buffer.output()
            self._backend.input(event)
        except IndexError:
            pass

    @property
    def file(self) -> file.SendFilter:
        """Return file sendfilter"""
        return self._file


# Protocols


@attr.s
class Protocol(protocols.Protocol[
        ReceiveEvent, ReceiveOutputEvent, SendEvent, SendOutputEvent
]):
    """Backend communication protocol."""

    _receive: ReceiveFilter = attr.ib(factory=ReceiveFilter)
    _send: SendFilter = attr.ib(factory=SendFilter)

    @property
    def receive(self) -> ReceiveFilter:
        """Return a Filter interface for receive events."""
        return self._receive

    @property
    def send(self) -> SendFilter:
        """Return a Filter interface for send events."""
        return self._send
