"""Sans-I/O protobuf file handling protocol."""

import logging
from typing import Optional

import attr

from ventserver.protocols import application
from ventserver.protocols import exceptions
from ventserver.sansio import protocols
from ventserver.sansio import channels

#Classes

@attr.s(auto_attribs=True)
class StateData:
    """Data info payload details"""
    state_type: Optional[str] = None
    data: Optional[bytes] = None

# Events

LowerEvent = StateData
UpperEvent = application.PBMessage

# Filters


@attr.s
class SendFilter(protocols.Filter[UpperEvent, LowerEvent]):
    """Filter which unwraps output data from an event class."""

    _logger = logging.getLogger('.'.join((__name__, 'SendFilter')))

    _buffer: channels.DequeChannel[UpperEvent] = attr.ib(
        factory=channels.DequeChannel)

    def input(self, event: Optional[UpperEvent]) -> None:
        """Handle input events."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[LowerEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if not event:
            return
        state_type = type(event).__name__
        message_body = bytes(event)
        
        message = StateData(state_type=state_type, data=message_body)
        return message
