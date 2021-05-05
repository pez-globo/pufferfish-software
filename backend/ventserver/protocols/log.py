"""Sans-I/O backend event log service protocol."""

import dataclasses
import typing
from typing import Dict, List, Optional

import attr

from ventserver.protocols import events
from ventserver.protocols.application import clocks, lists
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import channels
from ventserver.sansio import protocols


# Events


@attr.s
class ReceiveInputEvent(events.Event):
    """Event log receiver input event."""

    current_time: float = attr.ib()
    next_log_events: Optional[mcu_pb.NextLogEvents] = attr.ib(default=None)
    active_log_events: Optional[mcu_pb.ActiveLogEvents] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.next_log_events is not None
            or self.active_log_events is not None
            # current_time only matters if next_log_events is not None, so it
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


# Synchronizers


@attr.s
class EventLogReceiver(protocols.Filter[ReceiveInputEvent, ReceiveOutputEvent]):
    """Receipt of an event log from a remote peer.

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
    _clock_synchronizer: clocks.ClockSynchronizer = attr.ib(
        factory=clocks.ClockSynchronizer
    )
    _next_log_events_prev: Optional[mcu_pb.NextLogEvents] = attr.ib(
        default=None
    )
    _events_log_next_id: int = attr.ib(default=0)
    _id_mapping: Dict[int, int] = attr.ib(factory=dict)

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
        if event is None:
            return None

        # Check for newly-received events
        if event.next_log_events != self._next_log_events_prev:
            # Don't re-input previously received segments into receiver.
            # This saves a bit of work, but more importantly it prevents the
            # 0th element from flooding the receiver, since the receiver treats
            # any element with id 0 as a reset and thus a non-duplicate event.
            self._log_events_receiver.input(event.next_log_events)
            self._next_log_events_prev = \
                dataclasses.replace(event.next_log_events)
        update_event = self._log_events_receiver.output()
        expected_log_event = None
        new_elements = []
        active_log_events = mcu_pb.ActiveLogEvents()
        if update_event is not None:
            if update_event.next_expected is not None:
                expected_log_event = mcu_pb.ExpectedLogEvent(
                    id=update_event.next_expected
                )
            # Remap remote IDs and times to local ID numbering & clock
            for element in update_event.new_elements:
                new_element = dataclasses.replace(element)
                new_element.id = self._events_log_next_id
                self._id_mapping[element.id] = new_element.id
                self._events_log_next_id += 1
                self._clock_synchronizer.input(clocks.UpdateEvent(
                    current_time=event.current_time,
                    remote_time=new_element.time
                ))
                new_element.time += self._clock_synchronizer.output()
                new_elements.append(new_element)
        if event.active_log_events is not None:
            active_log_events.id = [
                self._id_mapping[id] for id in event.active_log_events.id
                if id in self._id_mapping
            ]
        return ReceiveOutputEvent(
            expected_log_event=expected_log_event, new_elements=new_elements,
            active_log_events=active_log_events
        )


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
