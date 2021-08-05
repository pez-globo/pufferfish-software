"""Sans-I/O ventilator backend server protocol."""

from typing import Optional, Union, Tuple
import logging

import attr

from ventserver.protocols import events, exceptions
from ventserver.protocols.backend import backend, connections
from ventserver.protocols.devices import file, frontend, mcu, rotary_encoder
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import protocols


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

    # This is the system's "wall clock" time, which may jump backwards or
    # forwards when the system time is adjusted. It should only be used for
    # recording timestamps intended to be read by people.
    wall_time: Optional[float] = attr.ib(default=None)
    # This is the system's monotonic time, which always increases at the same
    # rate regardless of adjustments to system time. Anything which needs to
    # compute the duration of elapsed time since some instant should use the
    # monotonic time.
    monotonic_time: Optional[float] = attr.ib(default=None)
    serial_receive: Optional[bytes] = attr.ib(default=None)
    websocket_receive: Optional[bytes] = attr.ib(default=None)
    rotary_encoder_receive: Tuple[int, bool] = attr.ib(default=None)
    file_receive: Optional[file.LowerReceiveEvent] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.wall_time is not None
            or self.monotonic_time is not None
            or bool(self.serial_receive)
            or self.websocket_receive is not None
            or self.rotary_encoder_receive is not None
            or self.file_receive is not None
        )


@attr.s
class ReceiveOutputEvent(events.Event):
    """Server receive output/send event."""

    states_send: Optional[backend.SendEvent] = attr.ib(default=None)
    sysclock_setting: Optional[float] = attr.ib(default=None)
    kill_frontend: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        if (self.states_send is not None) and self.states_send.has_data():
            return True

        if (self.sysclock_setting is not None) or self.kill_frontend:
            return True

        return False


ReceiveEvent = Union[ReceiveDataEvent, ConnectionEvent]
SendEvent = backend.SendEvent


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
        wall_time: float, monotonic_time: Optional[float],
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from serial receive data."""
    return ReceiveDataEvent(
        serial_receive=serial_receive,
        wall_time=wall_time, monotonic_time=monotonic_time
    )


def make_websocket_receive(
        ws_receive: bytes,
        wall_time: float, monotonic_time: Optional[float]
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from websocket receive data."""
    return ReceiveDataEvent(
        websocket_receive=ws_receive,
        wall_time=wall_time, monotonic_time=monotonic_time
    )


def make_rotary_encoder_receive(
        re_receive: Tuple[int, bool],
        wall_time: float, monotonic_time: Optional[float]
) -> ReceiveDataEvent:
    """Make a ReceiveDataEvent from rotary encoder receive data."""
    return ReceiveDataEvent(
        rotary_encoder_receive=re_receive,
        wall_time=wall_time, monotonic_time=monotonic_time
    )


# Filters


@attr.s
class Receiver(protocols.Filter[ReceiveEvent, ReceiveOutputEvent]):
    """Filter which transforms receive bytes into high-level events.

    Because this filter immediately handles inputs instead of taking inputs
    through an internal buffer and then processing the buffer in the output()
    method, the input method is not thread-safe! It should only be used in
    sequential or async/await code.
    """
    _logger = logging.getLogger('.'.join((__name__, 'Receiver')))

    wall_time: float = attr.ib(default=0)
    monotonic_time: float = attr.ib(default=0)
    _backend: backend.Receiver = attr.ib(factory=backend.Receiver)

    # Devices
    _mcu: mcu.Receiver = attr.ib(factory=mcu.Receiver)
    _frontend: frontend.Receiver = attr.ib(factory=frontend.Receiver)
    _file: file.Receiver = attr.ib(factory=file.Receiver)
    _rotary_encoder: rotary_encoder.Receiver = attr.ib(
        factory=rotary_encoder.Receiver
    )

    _connections: connections.TimeoutHandler = \
        attr.ib(factory=connections.TimeoutHandler)
    _connection_states: mcu_pb.BackendConnections = \
        attr.ib(factory=mcu_pb.BackendConnections)

    def input(self, event: Optional[ReceiveEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        if isinstance(event, MCUConnectionEvent):
            update_type = (
                connections.Update.MCU_CONNECTED if event.connected
                else connections.Update.MCU_DISCONNECTED
            )
            self._connections.input(connections.UpdateEvent(
                monotonic_time=self.monotonic_time, type=update_type
            ))
            return

        if isinstance(event, FrontendConnectionEvent):
            update_type = (
                connections.Update.FRONTEND_CONNECTED if event.connected
                else connections.Update.FRONTEND_DISCONNECTED
            )
            self._connections.input(connections.UpdateEvent(
                monotonic_time=self.monotonic_time, type=update_type
            ))
            return

        if event.wall_time is not None:
            self.wall_time = event.wall_time
        if event.monotonic_time is not None:
            self.monotonic_time = event.monotonic_time
            self._connections.input(connections.UpdateEvent(
                monotonic_time=self.monotonic_time
            ))
        self._mcu.input(event.serial_receive)
        self._frontend.input(event.websocket_receive)
        self._rotary_encoder.input(rotary_encoder.ReceiveEvent(
            wall_time=self.wall_time, re_data=event.rotary_encoder_receive
        ))
        self._file.input(event.file_receive)

    def output(self) -> Optional[ReceiveOutputEvent]:
        """Emit the next output event."""
        devices_processed = self._process_devices()
        if not devices_processed:
            self._backend.input(backend.ReceiveDataEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time
            ))
        # Process backend output until the backend has data to output or it
        # indicates that it has no more receive data to process.
        output_event = ReceiveOutputEvent()
        while True:
            backend_send = self._backend.output()
            if backend_send is None or backend_send.has_data():
                break
        if backend_send is not None:
            output_event.states_send = backend_send.states_send
        output_event.kill_frontend = self._process_connection_statuses()
        if backend_send is not None:
            output_event.sysclock_setting = backend_send.sysclock_setting
        if not (devices_processed or output_event.has_data()):
            return None

        return output_event

    def _process_mcu(self) -> bool:
        """Process the next event from the mcu protocol."""
        mcu_output = self._mcu.output()
        if mcu_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            wall_time=self.wall_time, monotonic_time=self.monotonic_time,
            mcu_receive=mcu_output, frontend_receive=None
        ))
        self._connections.input(connections.UpdateEvent(
            monotonic_time=self.monotonic_time,
            type=connections.Update.MCU_RECEIVED
        ))
        if not self._connection_states.has_mcu:
            self._logger.info('Receiving data from the MCU')
            self._backend.input(backend.ExternalLogEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                active=False,
                code=mcu_pb.LogEventCode.backend_mcu_connection_down
            ))
            self._connection_states.has_mcu = True
            self._backend.input(backend.ReceiveDataEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                server_receive=self._connection_states
            ))
        return True

    def _process_frontend(self) -> bool:
        """Process the next event from the frontend protocol."""
        frontend_output = self._frontend.output()
        if frontend_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            wall_time=self.wall_time, monotonic_time=self.monotonic_time,
            frontend_receive=frontend_output
        ))
        self._connections.input(connections.UpdateEvent(
            monotonic_time=self.monotonic_time,
            type=connections.Update.FRONTEND_RECEIVED
        ))
        if not self._connection_states.has_frontend:
            self._logger.info('Receiving data from the frontend')
            self._backend.input(backend.ExternalLogEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                active=False,
                code=mcu_pb.LogEventCode.backend_frontend_connection_down
            ))
            self._backend.input(backend.ExternalLogEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                code=mcu_pb.LogEventCode.backend_frontend_connection_up
            ))
            self._connection_states.has_frontend = True
            self._backend.input(backend.ReceiveDataEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                server_receive=self._connection_states
            ))
        return True

    def _process_file(self) -> bool:
        """Process the next event from the file."""
        file_output = self._file.output()  # throws ProtocolDataError
        if file_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            wall_time=self.wall_time, monotonic_time=self.monotonic_time,
            file_receive=file_output
        ))
        return True

    def _process_rotary_encoder(self) -> bool:
        """Process the next event from the rotary encoder."""
        rotary_encoder_output = self._rotary_encoder.output()
        if rotary_encoder_output is None:
            return False

        self._backend.input(backend.ReceiveDataEvent(
            wall_time=self.wall_time, monotonic_time=self.monotonic_time,
            frontend_receive=rotary_encoder_output
        ))
        return True

    def _process_devices(self) -> bool:
        """Process the next event from every device."""
        any_updated = False

        any_updated = self._process_mcu() or any_updated
        any_updated = self._process_frontend() or any_updated
        # TODO: why do we need an exception handler here?
        try:
            any_updated = self._process_file() or any_updated
        except exceptions.ProtocolDataError as err:
            self._logger.error(err)
        any_updated = self._process_rotary_encoder() or any_updated

        return any_updated

    def _process_connection_statuses(self) -> bool:
        """Handle any connection timeouts."""
        actions = self._connections.output()
        if actions.alarm_mcu:
            self._backend.input(backend.ExternalLogEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                active=True,
                code=mcu_pb.LogEventCode.backend_mcu_connection_down
            ))
            self._connection_states.has_mcu = False
            self._backend.input(backend.ReceiveDataEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                server_receive=self._connection_states
            ))
            # The backend should temporarily override any active alarm mute by
            # cancelling it in case local alarm sounds need to be audible in the
            # backend, e.g. for loss of the MCU. It will also propagate this
            # override to the frontend, if the frontend is connected; the
            # frontend will also independently cancel the alarm mute, so when
            # the backend overrides the frontend (or vice versa) the alarm mute
            # is still cancelled. AlarmMute will be overridden again by the MCU
            # when it reconnects, but the MCU will also cancel any active alarm
            # mute in the MCU, so the alarm mute is still cancelled.
            self._backend.input(backend.AlarmMuteCancellationEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                source=mcu_pb.AlarmMuteSource.backend_mcu_loss
            ))
        if actions.alarm_frontend:
            self._backend.input(backend.ExternalLogEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                active=True,
                code=mcu_pb.LogEventCode.backend_frontend_connection_down
            ))
            self._connection_states.has_frontend = False
            self._backend.input(backend.ReceiveDataEvent(
                wall_time=self.wall_time, monotonic_time=self.monotonic_time,
                server_receive=self._connection_states
            ))
            # We don't need to request an alarm mute cancellation from the
            # MCU, because the MCU already cancels the alarm mute when the
            # backend tells it that the frontend has been lost. If the MCU is
            # also lost, that will be handled in the previous `if` statement.
        return actions.kill_frontend

    def input_serial(self, serial_receive: bytes) -> None:
        """Input a ReceiveDataEvent corresponding to serial data.

        This is just a convenience function intended for writing unit tests
        more concisely.
        """
        self.input(make_serial_receive(
            serial_receive, self.wall_time, self.monotonic_time
        ))

    def input_websocket(self, websocket: bytes) -> None:
        """Input a ReceiveDataEvent corresponding to websocket data.

        This is just a convenience function intended for writing unit tests
        more concisely.
        """
        self.input(make_websocket_receive(
            websocket, self.wall_time, self.monotonic_time
        ))

    @property
    def backend(self) -> backend.Receiver:
        """Return the backend receiver."""
        return self._backend

    @property
    def file(self) -> file.Receiver:
        """Return the file receiver"""
        return self._file


@attr.s
class Sender(protocols.Filter[SendEvent, SendOutputEvent]):
    """Filter which transforms high-level events into send bytes."""

    # Devices
    _mcu: mcu.Sender = attr.ib(factory=mcu.Sender)
    _frontend: frontend.Sender = attr.ib(factory=frontend.Sender)
    _file: file.Sender = attr.ib(factory=file.Sender)

    def input(self, event: Optional[SendEvent]) -> None:
        """Handle input events."""
        self._mcu.input(backend.get_mcu_send(event))
        self._frontend.input(backend.get_frontend_send(event))
        self._file.input(backend.get_file_send(event))

    def output(self) -> Optional[SendOutputEvent]:
        """Emit the next output event."""
        send_event = SendOutputEvent(
            serial_send=self._mcu.output(),
            websocket_send=self._frontend.output(),
            file_send=self._file.output()
        )
        if not send_event.has_data():
            return None

        return send_event

    @property
    def file(self) -> file.Sender:
        """Return file sendfilter"""
        return self._file


# Protocols


@attr.s
class Protocol(protocols.Protocol[
        ReceiveEvent, ReceiveOutputEvent, SendEvent, SendOutputEvent
]):
    """Backend communication protocol."""

    _receive: Receiver = attr.ib(factory=Receiver)
    _send: Sender = attr.ib(factory=Sender)

    @property
    def receive(self) -> Receiver:
        """Return a Filter interface for receive events."""
        return self._receive

    @property
    def send(self) -> Sender:
        """Return a Filter interface for send events."""
        return self._send
