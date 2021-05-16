"""Sans-I/O backend connection loss handling protocol."""

import enum
from typing import Optional, Tuple
import logging

import attr

from ventserver.protocols import events
from ventserver.protocols.application import connections
from ventserver.sansio import protocols


# Events


@enum.unique
class Update(enum.Enum):
    """Enum for specifying the type of connection update event."""
    MCU_CONNECTED = enum.auto()
    MCU_DISCONNECTED = enum.auto()
    MCU_RECEIVED = enum.auto()
    FRONTEND_CONNECTED = enum.auto()
    FRONTEND_DISCONNECTED = enum.auto()
    FRONTEND_RECEIVED = enum.auto()
    CLOCK = enum.auto()


@attr.s
class UpdateEvent(events.Event):
    """Generic connection status event."""

    current_time: float = attr.ib()
    type: Update = attr.ib(default=Update.CLOCK)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class ActionsEvent(events.Event):
    """Actions execution event."""

    alarm_mcu: bool = attr.ib(default=False)
    alarm_frontend: bool = attr.ib(default=False)
    kill_frontend: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class TimeoutHandler(protocols.Filter[UpdateEvent, ActionsEvent]):
    """Filter which handles connection timeouts for MCU and frontend."""
    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    current_time: float = attr.ib(default=0)

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
        return connections.TimeoutDetector(event_timeout=0.5)

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

    def input(self, event: Optional[UpdateEvent]) -> None:
        """Handle input events."""
        if event is None:
            return

        self.current_time = event.current_time
        self._update_clocks()
        if event.type == Update.MCU_CONNECTED:
            self._handle_mcu_connection(True)
        elif event.type == Update.MCU_DISCONNECTED:
            self._handle_mcu_connection(False)
        elif event.type == Update.MCU_RECEIVED:
            self._mcu_status.input(connections.UpdateEvent(
                current_time=self.current_time, event_received=True
            ))
        elif event.type == Update.FRONTEND_CONNECTED:
            self._handle_frontend_connection(True)
        elif event.type == Update.FRONTEND_DISCONNECTED:
            self._handle_frontend_connection(False)
        elif event.type == Update.FRONTEND_RECEIVED:
            self._frontend_status.input(connections.UpdateEvent(
                current_time=self.current_time, event_received=True
            ))

    def output(self) -> ActionsEvent:
        """Emit the next output event."""
        output = ActionsEvent()
        output.alarm_mcu = self._process_mcu()
        (output.alarm_frontend, output.kill_frontend) = self._process_frontend()
        return output

    def _update_clocks(self) -> None:
        """Update all clocks."""
        status_clock_update = connections.UpdateEvent(
            current_time=self.current_time
        )
        self._mcu_status.input(status_clock_update)
        self._frontend_status.input(status_clock_update)
        debouncer_clock_update = connections.ActionStatus(
            current_time=self.current_time
        )
        self._mcu_alarm_debouncer.input(debouncer_clock_update)
        self._frontend_alarm_debouncer.input(debouncer_clock_update)
        self._frontend_kill_debouncer.input(debouncer_clock_update)

    def _process_mcu(self) -> bool:
        """Decide if the MCU connection is unresponsive and what to do."""
        mcu_status = self._mcu_status.output()
        # Update action debouncer
        if mcu_status is not None:
            self._mcu_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, trigger=mcu_status.timed_out
            ))
        # Decide whether to run action
        alarm_mcu = self._mcu_alarm_debouncer.output()
        if alarm_mcu:
            self._mcu_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))
        return alarm_mcu

    def _process_frontend(self) -> Tuple[bool, bool]:
        """Decide if the frontend connection is unresponsive and what to do."""
        frontend_status = self._frontend_status.output()
        # Update action debouncers
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
        # Decide whether to run actions
        alarm_frontend = self._frontend_alarm_debouncer.output()
        if alarm_frontend:
            self._frontend_alarm_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))
        kill_frontend = self._frontend_kill_debouncer.output()
        if kill_frontend:
            self._frontend_kill_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, execute=True
            ))
        return (alarm_frontend, kill_frontend)

    def _handle_mcu_connection(self, connected: bool) -> None:
        """Handle a connection event for the MCU."""
        if connected:
            self._logger.debug('Received connection to the MCU.')
        else:
            self._logger.debug('Lost connection to the MCU!')
        self._mcu_status.input(connections.UpdateEvent(
            connected=connected, current_time=self.current_time
        ))

    def _handle_frontend_connection(self, connected: bool) -> None:
        """Handle a connection event for the Frontend."""
        if connected:
            self._logger.debug('Received connection to the frontend.')
            # Stop repeatedly trying to kill the frontend, to give the frontend
            # connection some time to start producing events
            self._frontend_kill_debouncer.input(connections.ActionStatus(
                current_time=self.current_time, trigger=False
            ))
        else:
            self._logger.debug('Lost connection to the frontend!')
        self._frontend_status.input(connections.UpdateEvent(
            connected=connected, current_time=self.current_time
        ))
