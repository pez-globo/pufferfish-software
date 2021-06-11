"""Sans-I/O backend event log service protocol."""

from typing import Collection, Dict, List, Optional, Union

import attr

from ventserver.protocols import events
from ventserver.protocols.application import debouncing
from ventserver.protocols.backend import log
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import protocols


# Events


@attr.s
class AlarmActivationEvent(events.Event):
    """Alarm deactivation event."""

    code: mcu_pb.LogEventCode = attr.ib()
    event_type: mcu_pb.LogEventType = attr.ib()
    lower_limit: int = attr.ib(default=0)
    upper_limit: int = attr.ib(default=0)
    current_time: Optional[float] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class AlarmDeactivationEvent(events.Event):
    """Alarm deactivation event."""

    current_time: Optional[float] = attr.ib(default=None)
    codes: Collection[mcu_pb.LogEventCode] = attr.ib(factory=list)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return len(self.codes) > 0


InputEvent = Union[
    log.LocalLogInputEvent, AlarmActivationEvent, AlarmDeactivationEvent
]


# Managers


@attr.s
class Manager(protocols.Filter[InputEvent, log.ReceiveInputEvent]):
    """Management of alarm and non-alarm events for the local event log."""

    SOURCE = log.EventSource.BACKEND

    debouncers: Dict[mcu_pb.LogEventCode, debouncing.Debouncer] = \
        attr.ib(factory=dict)
    current_time: float = attr.ib(default=0)  # s
    _event_log: log.LocalLogSource = attr.ib(factory=log.LocalLogSource)
    _new_events: List[mcu_pb.LogEvent] = attr.ib(factory=list)
    _active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(factory=dict)

    def input(self, event: Optional[InputEvent]) -> None:
        """Handle input events."""
        if event is None:
            return

        if event.current_time is not None:
            self.current_time = event.current_time
        if not event.has_data():
            return

        if isinstance(event, log.LocalLogInputEvent):
            self._event_log.input(event)
        elif isinstance(event, AlarmActivationEvent):
            self.handle_alarm_activation(event)
        elif isinstance(event, AlarmDeactivationEvent):
            self.handle_alarm_deactivation(event)
        new_events = self._event_log.output()
        self._new_events.extend(new_events.new_events)
        for (event_code, event_id) in new_events.new_active_events.items():
            self._active_alarm_ids[event_code] = event_id

    def handle_alarm_activation(self, event: AlarmActivationEvent) -> None:
        """Process an alarm activation event input."""
        if event.code in self.debouncers:
            self.debouncers[event.code].input(debouncing.Input(
                current_time=self.current_time, signal=True
            ))
            if not self.debouncers[event.code].output():
                return

        if event.code in self._active_alarm_ids:
            return

        self._event_log.input(log.LocalLogInputEvent(
            current_time=self.current_time, active=True,
            new_event=mcu_pb.LogEvent(
                code=event.code, type=event.event_type,
                alarm_limits=mcu_pb.Range(
                    lower=event.lower_limit, upper=event.upper_limit
                )
            )
        ))

    def handle_alarm_deactivation(self, event: AlarmDeactivationEvent) -> None:
        """Process an alarm deactivation event input."""
        for code in event.codes:
            if code in self.debouncers:
                self.debouncers[code].input(debouncing.Input(
                    current_time=self.current_time, signal=False
                ))
                if not self.debouncers[code].output():
                    self._active_alarm_ids.pop(code, None)
            else:
                self._active_alarm_ids.pop(code, None)

    def output(self) -> Optional[log.ReceiveInputEvent]:
        """Emit the next output event."""
        output = log.ReceiveInputEvent(
            source=self.SOURCE, current_time=self.current_time,
            next_log_events=mcu_pb.NextLogEvents(elements=self._new_events),
            active_log_events=mcu_pb.ActiveLogEvents(
                id=list(self._active_alarm_ids.values())
            )
        )
        self._new_events = []
        return output
