"""Appication sub-layer for state synchronization."""

import collections
import enum
import logging
from typing import Deque, Dict, Generic, Optional, Type, TypeVar

import attr

import betterproto

from ventserver.protocols import events
from ventserver.protocols import exceptions
from ventserver.sansio import protocols


_StateSegment = TypeVar('_StateSegment', bound=enum.Enum)


@attr.s
class UpdateEvent(events.Event, Generic[_StateSegment]):
    """State update event."""

    time: Optional[float] = attr.ib(default=None)
    pb_message: Optional[betterproto.Message] = attr.ib(default=None)
    segment_type: Optional[_StateSegment] = attr.ib(default=None)

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return self.time is not None or self.pb_message is not None


@attr.s
class ScheduleEntry(Generic[_StateSegment]):
    """Output schedule entry."""

    time: float = attr.ib()
    type: _StateSegment = attr.ib()


@attr.s
class Synchronizer(
        protocols.Filter[UpdateEvent[_StateSegment], betterproto.Message]
):
    """State synchronization filter.

    Inputs are clock updates or state updates received from the peer. Outputs
    are state updates for the peer.

    Warning: if all_states is used by other things, this filter is only safe to
    use in synchronous environments, such as part of another Filter which
    completely owns all_states. However, if access to all_states is only done
    through the input and output methods, then this Filter is safe to use in
    concurrent environments.
    """

    _logger = logging.getLogger('.'.join((__name__, 'Synchronizer')))

    segment_types: Type[_StateSegment] = attr.ib()
    current_time: float = attr.ib(default=0)
    all_states: Dict[_StateSegment, Optional[betterproto.Message]] = attr.ib()
    output_schedule: Deque[ScheduleEntry[_StateSegment]] = attr.ib()
    output_deadline: Optional[float] = attr.ib(default=None)

    @all_states.default
    def init_all_states(self) -> Dict[
            _StateSegment, Optional[betterproto.Message]
    ]:  # pylint: disable=no-self-use
        """Initialize the synchronizable states.

        Each pair consists of the type class to specify the states, and an
        actual object to store the state values.
        """
        return {type: None for type in self.segment_types}

    @output_schedule.default
    def init_output_schedule(self) -> Deque[
            ScheduleEntry[_StateSegment]
    ]:  # pylint: disable=no-self-use
        """Initialize the output schedule.

        Each pair consists of the type class to specify the message to output
        for the next output, and the time (in seconds) to wait after outputting
        that message before proceeding to outputting the next message.
        """
        return collections.deque([])

    def input(self, event: Optional[UpdateEvent[_StateSegment]]) -> None:
        """Handle input events."""
        if event is None or not event.has_data():
            return

        if event.time is not None:
            self._logger.debug('Time: %f', event.time)
            self.current_time = event.time
            if self.output_deadline is None:
                self.output_deadline = (
                    self.current_time + self.output_schedule[0].time
                )
        if event.pb_message is None:
            return

        self._logger.debug('Received: %s', event.pb_message)
        message_type = event.segment_type
        if message_type is None:
            raise exceptions.ProtocolDataError(
                'Received message without corresopnding segment type: {}'
                .format(type(event.pb_message))
            )
        try:
            self.all_states[message_type] = event.pb_message
        except KeyError as exc:
            raise exceptions.ProtocolDataError(
                'Received message type is not a synchronizable state: {}'
                .format(message_type)
            ) from exc

    def output(self) -> Optional[betterproto.Message]:
        """Emit the next output event."""
        if self.output_deadline is None:
            return None

        if self.current_time < self.output_deadline:
            return None

        output_type = self.output_schedule[0].type
        try:
            output_event = self.all_states[output_type]
        except KeyError as exc:
            raise exceptions.ProtocolDataError(
                'Scheduled message type is not a synchronizable state: {}'
                .format(output_type)
            ) from exc

        self.output_schedule.rotate(-1)
        self.output_deadline = (
            self.current_time + self.output_schedule[0].time
        )
        if output_event is None:
            return None

        self._logger.debug('Sending: %s', output_event)
        return output_event
