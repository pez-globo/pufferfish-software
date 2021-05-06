"""Sans-I/O backend event log service protocol."""

import dataclasses
import typing
from typing import Dict, List, Optional, Union

import attr

from ventserver.protocols import events
from ventserver.protocols.application import clocks, lists
from ventserver.protocols.protobuf import mcu_pb
from ventserver.sansio import channels, protocols


# Events


@attr.s
class ReceiveRemoteInputEvent(events.Event):
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
class ReceiveLocalInputEvent(events.Event):
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


ReceiveInputEvent = Union[ReceiveRemoteInputEvent, ReceiveLocalInputEvent]


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
    _clock_synchronizer: clocks.ClockSynchronizer = attr.ib(
        factory=clocks.ClockSynchronizer
    )
    _next_log_events_prev: Optional[mcu_pb.NextLogEvents] = attr.ib(
        default=None
    )
    _events_log_next_id: int = attr.ib(default=0)
    _remote_id_mapping: Dict[int, int] = attr.ib(factory=dict)
    _session_id: int = attr.ib(default=0)

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

        if isinstance(event, ReceiveRemoteInputEvent):
            return self._handle_remote_event(event)

        if isinstance(event, ReceiveLocalInputEvent):
            return self._handle_local_event(event)

        return None

    def _handle_remote_session_reset(
            self, update_event: lists.UpdateEvent[mcu_pb.LogEvent]
    ) -> None:
        """Handle a reset of the remote peer's log session."""
        self._remote_id_mapping.clear()
        self._clock_synchronizer.input(clocks.ResetEvent())
        self._session_id = update_event.session_id

    def _remap_remote_element(
            self, element: mcu_pb.LogEvent, current_time: float
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
            current_time=current_time,
            remote_time=new_element.time
        ))
        new_element.time += self._clock_synchronizer.output()
        return new_element

    def _handle_remote_event(
            self, event: ReceiveRemoteInputEvent
    ) -> ReceiveOutputEvent:
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
            if update_event.session_id != self._session_id:
                # The remote peer reset its event log, so invalidate all id
                # mappings for active log events and reset the clock sychronizer
                self._handle_remote_session_reset(update_event)
            # Remap remote IDs and times to local ID numbering & clock
            output_event.new_elements = [
                self._remap_remote_element(element, event.current_time)
                for element in update_event.new_elements
            ]
            # Generate the next ExpectedLogEvent
            if update_event.next_expected is not None:
                output_event.expected_log_event = mcu_pb.ExpectedLogEvent(
                    id=update_event.next_expected, session_id=self._session_id
                )
        # TODO: we'll update active local events and store it locally, and then
        # in output() we'll concatenate it together with remapped active remote
        # events for each event source.
        if event.active_log_events is not None:
            output_event.active_log_events = mcu_pb.ActiveLogEvents(
                id=[
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
            )
        return output_event

    def _handle_local_event(
            self, event: ReceiveLocalInputEvent
    ) -> Optional[ReceiveOutputEvent]:
        """Process events received from a local source."""
        # new_elements = []
        # active_log_events = mcu_pb.ActiveLogEvents()
        # if update_event is not None:
        #     # Remap local IDs to local ID numbering
        #     for element in update_event.new_elements:
        #         new_element = dataclasses.replace(element)
        #         new_element.id = self._events_log_next_id
        #         self._remote_id_mapping[element.id] = new_element.id
        #         self._events_log_next_id += 1
        #         new_elements.append(new_element)
        # if event.active_log_events is not None:
        #     active_log_events.id = [
        #         self._remote_id_mapping[id]
        #         for id in event.active_log_events.id
        #         if id in self._remote_id_mapping
        #     ]
        # return ReceiveOutputEvent(
        #     new_elements=new_elements, active_log_events=active_log_events
        # )


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
