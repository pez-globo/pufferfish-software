"""Sans-I/O backend event log service protocol."""

import dataclasses
from typing import List, Mapping, Optional

import attr

from ventserver.protocols import events
from ventserver.protocols.application import clocks, lists
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import channels
from ventserver.sansio import protocols


# Events


@attr.s
class ReceiveInputEvent(events.Event):
    """Event log synchronizer input event."""

    next_log_events: mcu_pb.NextLogEvents = attr.ib()
    current_time: float = attr.ib()

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True


@attr.s
class ReceiveOutputEvent(events.Event):
    """Event log synchronizer input event."""

    expected_log_event: Optional[mcu_pb.ExpectedLogEvent] = attr.ib(
        default=None
    )
    new_elements: List[mcu_pb.LogEvent] = attr.ib(factory=list)
    id_mapping: Mapping[int, int] = attr.ib(factory=dict)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return (
            self.expected_log_event is not None or len(self.new_elements) > 0
            or len(self.id_mapping) > 0
        )


# Synchronizers


@attr.s
class EventLogReceiver(protocols.Filter[ReceiveInputEvent, ReceiveOutputEvent]):
    """Receipt of an event log from a remote peer.

    Receives events from the pee, remaps IDs and times, and outputs newly
    received events.
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
        if update_event is None:
            return None

        expected_log_event = None
        if update_event.next_expected is not None:
            expected_log_event = mcu_pb.ExpectedLogEvent(
                id=update_event.next_expected
            )
        # Remap remote IDs and times to local ID numbering & clock
        new_elements = []
        id_mapping = {}
        for element in update_event.new_elements:
            new_element = dataclasses.replace(element)
            new_element.id = self._events_log_next_id
            id_mapping[element.id] = new_element.id
            self._events_log_next_id += 1
            self._clock_synchronizer.input(clocks.UpdateEvent(
                current_time=event.current_time, remote_time=new_element.time
            ))
            new_element.time += self._clock_synchronizer.output()
            new_elements.append(new_element)
        return ReceiveOutputEvent(
            expected_log_event=expected_log_event, new_elements=new_elements,
            id_mapping=id_mapping
        )
