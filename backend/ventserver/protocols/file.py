"""Sans-I/O protobuf file handling protocol."""

import logging
from typing import Optional

import attr

from ventserver.protocols import application
from ventserver.protocols import exceptions
from ventserver.protocols import messages
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
class ReceiveFilter(protocols.Filter[LowerEvent, UpperEvent]):
    """Filter which wraps raw data from file to message."""
    _logger = logging.getLogger('.'.join((__name__, 'ReceiveFilter')))

    _buffer: channels.DequeChannel[LowerEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    _message_receiver: messages.MessageReceiver = attr.ib()

    @_message_receiver.default
    def init_message_receiver(self) -> messages.MessageReceiver:  # pylint: disable=no-self-use
        """Initialize the mcu message receiver."""
        return messages.MessageReceiver(
            message_classes=application.MCU_MESSAGE_CLASSES
        )

    def input(self, event: Optional[LowerEvent]) -> None:
        """Handle input events."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[UpperEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if not event:
            return None

        self._message_receiver.input(event.data)
        message = None
        try:
            message = self._message_receiver.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('MessageSender:')

        # check whether file name and data type are same
        assert (event.state_type == type(message).__name__), ("The state type",
                f"{type(message).__name__} data in the file does not match the",
                f"filename {event.state_type}.")

        return message

@attr.s
class SendFilter(protocols.Filter[UpperEvent, LowerEvent]):
    """Filter which unwraps output data from message to raw data."""
    _logger = logging.getLogger('.'.join((__name__, 'SendFilter')))

    _buffer: channels.DequeChannel[UpperEvent] = attr.ib(
        factory=channels.DequeChannel
    )

    _message_sender: messages.MessageSender = attr.ib()

    @_message_sender.default
    def init_message_sender(self) -> messages.MessageSender:  # pylint: disable=no-self-use
        """Initialize the message sender."""
        return messages.MessageSender(
            message_types=application.MCU_MESSAGE_TYPES
        )

    def input(self, event: Optional[UpperEvent]) -> None:
        """Handle input events."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[LowerEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if not event:
            return None
        self._message_sender.input(event)
        message_body = None
        try:
            message_body = self._message_sender.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('MessageSender:')

        if not message_body:
            print("empty message")

        state_type = type(event).__name__
        payload = message_body
        
        message = StateData(state_type=state_type, data=payload)
        return message
