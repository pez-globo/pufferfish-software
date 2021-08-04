"""Sans-I/O backend event log service protocol."""

import dataclasses
import enum
import typing
from typing import Dict, List, Mapping, Optional

import attr

from ventserver.protocols import events
from ventserver.protocols.application import clocks, lists
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import channels, protocols


# Events


@enum.unique
class EventSource(enum.Enum):
    """Enum for specifying the type of connection update event."""
    MCU = enum.auto()
    BACKEND = enum.auto()


@attr.s
class ReceiveInputEvent(events.Event):
    """Event log receiver input event."""

    source: EventSource = attr.ib()
    wall_time: float = attr.ib()
    next_log_events: Optional[mcu_pb.NextLogEvents] = attr.ib(default=None)
    active_log_events: Optional[mcu_pb.ActiveLogEvents] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.next_log_events is not None
            or self.active_log_events is not None
            # wall_time only matters if next_log_events is not None, so it
            # doesn't count for has_data.
        )


@attr.s
class ReceiveOutputEvent(events.Event):
    """Event log receiver input event."""

    expected_log_event: Optional[mcu_pb.ExpectedLogEvent] = attr.ib(
        default=None
    )
    new_elements: List[mcu_pb.LogEvent] = attr.ib(factory=list)
    active_log_events: Optional[mcu_pb.ActiveLogEvents] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.expected_log_event is not None or len(self.new_elements) > 0
            or self.active_log_events is not None
        )


@attr.s
class SendInputEvent(events.Event):
    """Event log sender input event."""

    expected_log_event: Optional[mcu_pb.ExpectedLogEvent] = \
        attr.ib(default=None)
    new_log_events: List[mcu_pb.LogEvent] = attr.ib(factory=list)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.expected_log_event is not None or len(self.new_log_events) > 0
        )


@attr.s
class LocalLogInputEvent(events.Event):
    """Local event log input event.

    next_log_event's id and time fields are ignored.
    """

    wall_time: Optional[float] = attr.ib(default=None)  # s
    new_event: Optional[mcu_pb.LogEvent] = attr.ib(default=None)
    active: bool = attr.ib(default=False)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.new_event is not None


@attr.s
class LocalLogOutputEvent(events.Event):
    """Local event log output event.
    """

    new_events: List[mcu_pb.LogEvent] = attr.ib(factory=list)
    new_active_events: Mapping[mcu_pb.LogEventCode, int] = attr.ib(factory=dict)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return bool(self.new_events) or bool(self.new_active_events)


# Synchronizers


@attr.s
class EventLogReceiver(protocols.Filter[ReceiveInputEvent, ReceiveOutputEvent]):
    """Receipt of an event log from a local or remote source.

    Receives log events and the list of active log events from the, remaps IDs
    and timestamps, and outputs newly received events and ID-remapped active log
    events.
    If an event has not yet been received but it's on the list of active events,
    it will be excluded from the active events list until the event is received
    in the log.
    """

    _buffer: channels.DequeChannel[ReceiveInputEvent] = attr.ib(
        factory=channels.DequeChannel
    )

    _log_events_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent] = attr.ib()
    _clock_synchronizer: clocks.Synchronizer = attr.ib(
        factory=clocks.Synchronizer
    )
    _next_log_events_prev: Optional[mcu_pb.NextLogEvents] = attr.ib(
        default=None
    )
    _events_log_next_id: int = attr.ib(default=0)
    _remote_id_mapping: Dict[int, int] = attr.ib(factory=dict)
    _remote_session_id: int = attr.ib(default=0)
    _active_remote_events: List[int] = attr.ib(factory=list)
    _local_id_mapping: Dict[int, int] = attr.ib(factory=dict)
    _active_local_events: List[int] = attr.ib(factory=list)

    @_log_events_receiver.default
    def init_log_events_list_receiver(self) -> lists.ReceiveSynchronizer[
            mcu_pb.LogEvent
    ]:   # pylint: disable=no-self-use
        """Initialize the frontend log events list sender."""
        return lists.ReceiveSynchronizer()

    def input(self, event: Optional[ReceiveInputEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        self._buffer.input(event)

    def output(self) -> Optional[ReceiveOutputEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if event is None or not event.has_data():
            return None

        if event.source == EventSource.MCU:
            return self._handle_remote_event(event)

        if event.source == EventSource.BACKEND:
            return self._handle_local_event(event)

        return None

    @property
    def _active_log_events(self) -> List[int]:
        """Compute the set union of active remote and local events."""
        return list(set(self._active_local_events + self._active_remote_events))

    def _handle_remote_session_reset(
            self, update_event: lists.UpdateEvent[mcu_pb.LogEvent]
    ) -> None:
        """Handle a reset of the remote peer's log session."""
        self._remote_id_mapping.clear()
        self._clock_synchronizer.input(clocks.ResetEvent())
        self._remote_session_id = update_event.session_id

    def _remap_remote_element(
            self, element: mcu_pb.LogEvent, wall_time: float
    ) -> mcu_pb.LogEvent:
        """Remap a remote log event's ID & time into the local reference."""
        new_element = dataclasses.replace(element)
        # Remap ID
        new_element.id = self._events_log_next_id
        self._events_log_next_id += 1
        # Maintain ID mappings for active log events
        self._remote_id_mapping[element.id] = new_element.id
        # Remap time
        self._clock_synchronizer.input(clocks.UpdateEvent(
            wall_time=wall_time, remote_time=new_element.time
        ))
        new_element.time += self._clock_synchronizer.output()
        return new_element

    def _handle_remote_event(self, event: ReceiveInputEvent) \
            -> ReceiveOutputEvent:
        """Process events received from a remote source."""
        # Check for newly-received events
        if event.next_log_events != self._next_log_events_prev:
            # Don't re-input previously received segments into receiver.
            self._log_events_receiver.input(event.next_log_events)
            self._next_log_events_prev = \
                dataclasses.replace(event.next_log_events)
        update_event = self._log_events_receiver.output()
        output_event = ReceiveOutputEvent()
        if update_event is not None:
            # Reset session-specific state when needed
            if update_event.session_id != self._remote_session_id:
                # The remote peer reset its event log, so invalidate all id
                # mappings for active log events and reset the clock sychronizer
                self._handle_remote_session_reset(update_event)
            # Remap remote IDs and times to local ID numbering & clock
            output_event.new_elements = [
                self._remap_remote_element(element, event.wall_time)
                for element in update_event.new_elements
            ]
            # Generate the next ExpectedLogEvent
            if update_event.next_expected is not None:
                output_event.expected_log_event = mcu_pb.ExpectedLogEvent(
                    id=update_event.next_expected,
                    session_id=self._remote_session_id
                )
        if event.active_log_events is not None:
            self._active_remote_events = [
                self._remote_id_mapping[id]
                for id in event.active_log_events.id
                if id in self._remote_id_mapping
                # TODO: we may need error handling if id isn't in mapping
                # (e.g. because the backend crashed and restarted while
                # the firmware stayed running. But if active_log_events
                # arrived before the corresponding log event arrived,
                # that shouldn't be treated like an error. So maybe we
                # actually just need to map (session ID, event ID) pairs,
                # and we need to persist that mapping to the filesystem,
                # and we can discard any keys with stale sessions.
            ]
        output_event.active_log_events = mcu_pb.ActiveLogEvents(
            id=self._active_log_events
        )
        return output_event

    def _remap_local_element(self, element: mcu_pb.LogEvent) -> mcu_pb.LogEvent:
        """Remap a local log event's ID into the local reference."""
        new_element = dataclasses.replace(element)
        # Remap ID
        new_element.id = self._events_log_next_id
        self._events_log_next_id += 1
        # Maintain ID mappings for active log events
        self._local_id_mapping[element.id] = new_element.id
        return new_element

    def _handle_local_event(self, event: ReceiveInputEvent) \
            -> Optional[ReceiveOutputEvent]:
        """Process events received from a local source."""
        output_event = ReceiveOutputEvent()
        # Remap ID numbering
        output_event.new_elements = []
        if event.next_log_events is not None:
            output_event.new_elements = [
                self._remap_local_element(element)
                for element in event.next_log_events.elements
            ]
        if event.active_log_events is not None:
            self._active_local_events = [
                self._local_id_mapping[id]
                for id in event.active_log_events.id
                if id in self._local_id_mapping
                # TODO: do we need error handling if id isn't in mapping?
            ]
        output_event.active_log_events = mcu_pb.ActiveLogEvents(
            id=self._active_log_events
        )
        return output_event


@attr.s
class EventLogSender(protocols.Filter[SendInputEvent, mcu_pb.NextLogEvents]):
    """Sending of an event log to a remote peer.

    Receives event log list synchronization cursor from the peer, and sends any
    events starting at that cursor.
    """

    _log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent] = attr.ib()
    _expected_log_event_prev: Optional[mcu_pb.ExpectedLogEvent] = attr.ib(
        default=None
    )

    @_log_events_sender.default
    def init_log_events_list_sender(self) -> lists.SendSynchronizer[
            mcu_pb.LogEvent
    ]:   # pylint: disable=no-self-use
        """Initialize the frontend log events list sender."""
        return lists.SendSynchronizer(segment_type=mcu_pb.NextLogEvents)

    def input(self, event: Optional[SendInputEvent]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        if (
                event.expected_log_event != self._expected_log_event_prev or
                len(event.new_log_events) > 0
        ):
            next_expected = None
            if event.expected_log_event is not None:
                next_expected = event.expected_log_event.id
                self._expected_log_event_prev = dataclasses.replace(
                    event.expected_log_event
                )
            self._log_events_sender.input(lists.UpdateEvent(
                next_expected=next_expected, new_elements=event.new_log_events
            ))

    def output(self) -> Optional[mcu_pb.NextLogEvents]:
        """Emit the next output event."""
        return typing.cast(
            mcu_pb.NextLogEvents, self._log_events_sender.output()
        )


# Managers


@attr.s
class LocalLogSource(protocols.Filter[
        LocalLogInputEvent, LocalLogOutputEvent
]):
    """Annotation of new local log events with IDs and timestamps.

    The IDs of all events passed through this filter are numbered consecutively.
    Takes input events, each of which contains a LogEvent whose id and timestamp
    fields are to be overridden (in a copy, so that the input is not modified),
    and a flag specifying whether the event is active.
    Outputs a list of annotated LogEvents to input into an EventLogReceiver, and
    a mapping of LogEventCodes to IDs for active log events.
    """

    wall_time: float = attr.ib(default=0)  # s
    next_log_event_id: int = attr.ib(default=0)
    _new_events: List[mcu_pb.LogEvent] = attr.ib(factory=list)
    _new_active_events: Dict[mcu_pb.LogEventCode, int] = attr.ib(factory=dict)

    def input(self, event: Optional[LocalLogInputEvent]) -> None:
        """Handle input events."""
        if event is None:
            return

        if event.wall_time is not None:
            self.wall_time = event.wall_time
        if event.new_event is None:
            return

        log_event = dataclasses.replace(event.new_event)
        log_event.id = self.next_log_event_id
        log_event.time = int(self.wall_time * 1000)
        self._new_events.append(log_event)
        if event.active:
            self._new_active_events[log_event.code] = log_event.id
        self.next_log_event_id += 1

    def output(self) -> LocalLogOutputEvent:
        """Emit the next output event."""
        output = LocalLogOutputEvent(
            new_events=self._new_events,
            new_active_events=self._new_active_events
        )
        self._new_events = []
        self._new_active_events = {}
        return output
