"""Sans-I/O ventilator backend server protocol."""

from typing import Optional, Union, Tuple
import logging

import attr

from ventserver.protocols import events, exceptions
from ventserver.protocols.application import connections
from ventserver.protocols.backend import backend
from ventserver.protocols.devices import file, frontend, mcu, rotary_encoder
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
    file_receive: Optional[file.StateData] = attr.ib(default=None)

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
    frontend_unresponsive: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.server_send is not None and self.server_send.has_data()


ReceiveEvent = Union[
    ReceiveDataEvent, FrontendConnectionEvent, MCUConnectionEvent
]
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

    _buffer: channels.DequeChannel[ReceiveEvent] =\
        attr.ib(factory=channels.DequeChannel)

    current_time: float = attr.ib(default=0)

    _mcu: mcu.ReceiveFilter = attr.ib(factory=mcu.ReceiveFilter)
    _frontend: frontend.ReceiveFilter = attr.ib(factory=frontend.ReceiveFilter)
    _file: file.ReceiveFilter = attr.ib(factory=file.ReceiveFilter)
    _rotary_encoder: rotary_encoder.ReceiveFilter = attr.ib(
        factory=rotary_encoder.ReceiveFilter
    )
    _backend: backend.ReceiveFilter = attr.ib(factory=backend.ReceiveFilter)

    _mcu_status: connections.TimeoutDetector = attr.ib()
    _mcu_alarm_debouncer: connections.ActionDebouncer = \
        attr.ib(factory=connections.ActionDebouncer)
    _frontend_status: connections.TimeoutDetector = attr.ib()
    _frontend_kill_debouncer: connections.ActionDebouncer = attr.ib()
    _frontend_alarm_debouncer: connections.ActionDebouncer = \
        attr.ib(factory=connections.ActionDebouncer)

    @_mcu_status.default
    def init_mcu_status(self) -> \
            connections.TimeoutDetector:  # pylint: disable=no-self-use
        """Initialize the MCU connection status tracker."""
        return connections.TimeoutDetector(event_timeout=2)

    @_frontend_status.default
    def init_frontend_status(self) -> \
            connections.TimeoutDetector:  # pylint: disable=no-self-use
        """Initialize the frontend connection status tracker."""
        return connections.TimeoutDetector(event_timeout=2)

    @_frontend_kill_debouncer.default
    def init_frontend_kill_debouncer(self) -> \
            connections.ActionDebouncer:  # pylint: disable=no-self-use
        """Initialize the frontend kill debouncer."""
        return connections.ActionDebouncer(repeat_interval=5)

    def input(self, event: Optional[ReceiveEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[ReceiveOutputEvent]:
        """Emit the next output event."""
        self._process_buffer()
        any_updated = False

        # Process mcu output
        any_updated = self._process_mcu() or any_updated
        # Process frontend output
        any_updated = self._process_frontend() or any_updated
        # Process rotary encoder output
        any_updated = self._process_rotary_encoder() or any_updated
        # Process file output
        # TODO: why do we need an exception handler here?
        try:
            any_updated = self._process_file() or any_updated
        except exceptions.ProtocolDataError as err:
            self._logger.error(err)
        # Process time
        if not any_updated:
            self._backend.input(backend.ReceiveEvent(time=self.current_time))

        # Process backend output
        # Consume any outputs as long as the backend is indicating that it still
        # has receive data to process, even if it has no data to output
        output = ReceiveOutputEvent()
        while True:
            output.server_send = self._backend.output()
            if output.server_send is None:
                break
            if output.server_send.has_data():
                break
        any_updated = any_updated or output.server_send is not None

        # TODO: decompose the output method into smaller chunks!
        # TODO: make a filter or helper class for dealing with unresponsive
        # connections!

        # Handle unresponsive mcu
        self._mcu_status.input(connections.UpdateEvent(
            current_time=self.current_time
        ))
        mcu_status = self._mcu_status.output()
        if mcu_status is not None:
            self._mcu_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, trigger=mcu_status.timed_out
            ))
        else:
            self._mcu_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time
            ))
        if self._mcu_alarm_debouncer.output():
            self._logger.info('TODO: create an active alarm for lost mcu!')
            self._mcu_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))

        # Handle unresponsive frontend
        self._frontend_status.input(connections.UpdateEvent(
            current_time=self.current_time
        ))
        frontend_status = self._frontend_status.output()
        if frontend_status is not None:
            self._frontend_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time,
                trigger=frontend_status.timed_out
            ))
            self._frontend_kill_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, trigger=(
                    frontend_status.timed_out and frontend_status.uptime > 2
                )
            ))
        else:
            self._frontend_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time
            ))
            self._frontend_kill_debouncer.input(connections.ActionStatus(
                current_time=self.current_time
            ))
        if self._frontend_kill_debouncer.output():
            output.frontend_unresponsive = True
            self._frontend_kill_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))
        if self._frontend_alarm_debouncer.output():
            self._logger.info('TODO: create an active alarm for lost frontend!')
            self._frontend_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))
        any_updated = any_updated or output.frontend_unresponsive

        if not any_updated:
            return None

        return output

    def _process_buffer(self) -> None:
        """Process the next event in the input buffer."""
        event = self._buffer.output()
        if event is None:
            return

        if isinstance(event, MCUConnectionEvent):
            if event.connected:
                self._logger.debug('Received connection to the MCU.')
            else:
                self._logger.debug('Lost connection to the MCU!')
            self._mcu_status.input(connections.UpdateEvent(
                connected=event.connected, current_time=self.current_time
            ))
            return

        if isinstance(event, FrontendConnectionEvent):
            if event.connected:
                self._logger.debug('Received connection to the frontend.')
                # Stop repeatedly killing the frontend to give the frontend
                # connection some time to start producing events
                self._frontend_kill_debouncer.input(connections.ActionStatus(
                    current_time=self.current_time, trigger=False
                ))
            else:
                self._logger.debug('Lost connection to the frontend!')
            self._frontend_status.input(connections.UpdateEvent(
                connected=event.connected, current_time=self.current_time
            ))
            return

        if event.time is not None:
            self.current_time = event.time
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

        self._backend.input(backend.ReceiveEvent(
            time=self.current_time, mcu_receive=mcu_output,
            frontend_receive=None
        ))
        self._mcu_status.input(connections.UpdateEvent(
            current_time=self.current_time, event_received=True
        ))
        # TODO: deactivate any alarm for unresponsive/lost MCU
        return True

    def _process_frontend(self) -> bool:
        """Process the next event from the frontend protocol."""
        frontend_output = self._frontend.output()
        if frontend_output is None:
            return False

        self._backend.input(backend.ReceiveEvent(
            time=self.current_time, frontend_receive=frontend_output
        ))
        self._frontend_status.input(connections.UpdateEvent(
            current_time=self.current_time, event_received=True
        ))
        # TODO: deactivate any alarm for unresponsive/lost frontend
        return True

    def _process_rotary_encoder(self) -> bool:
        """Process the next event from the rotary encoder."""
        rotary_encoder_output = self._rotary_encoder.output()
        if rotary_encoder_output is None:
            return False

        self._backend.input(backend.ReceiveEvent(
            time=self.current_time, frontend_receive=rotary_encoder_output
        ))
        return True

    def _process_file(self) -> bool:
        """Process the next event from the file."""
        file_output = self._file.output()  # throws ProtocolDataError
        if file_output is None:
            return False

        self._backend.input(backend.ReceiveEvent(
            time=self.current_time, file_receive=file_output
        ))
        return True

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

    _buffer: channels.DequeChannel[SendEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    _backend: backend.SendFilter = attr.ib(factory=backend.SendFilter)
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
