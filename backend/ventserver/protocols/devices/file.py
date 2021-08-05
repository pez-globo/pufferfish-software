"""Sans-I/O protobuf file handling protocol."""

import logging
from typing import Optional, Union

import attr
import betterproto

from ventserver.protocols.devices import frontend
from ventserver.protocols import events, exceptions
from ventserver.protocols.transport import crcelements, messages
from ventserver.sansio import channels, protocols


# Classes


@attr.s
class StateData(events.Event):
    """Data info payload details"""
    state_type: str = attr.ib()
    data: bytes = attr.ib()

    def has_data(self) -> bool:
        """Return whether the event has data."""
        return True

# Events


LowerReceiveEvent = Union[StateData, betterproto.Message]
LowerSendEvent = StateData
UpperEvent = betterproto.Message


# Filters


@attr.s
class Receiver(protocols.Filter[LowerReceiveEvent, UpperEvent]):
    """Filter which wraps raw data from file to message."""
    _logger = logging.getLogger('.'.join((__name__, 'Receiver')))

    _buffer: channels.DequeChannel[LowerReceiveEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    _crc_receiver: crcelements.Receiver = attr.ib(factory=crcelements.Receiver)
    _message_receiver: messages.Receiver = attr.ib()

    @_message_receiver.default
    def init_message_receiver(self) -> messages.Receiver:  # pylint: disable=no-self-use
        """Initialize the mcu message receiver."""
        return messages.Receiver(message_classes=frontend.MESSAGE_CLASSES)

    def input(self, event: Optional[LowerReceiveEvent]) -> None:
        """Handle input events."""
        if not event:
            return

        self._buffer.input(event)

    def output(self) -> Optional[UpperEvent]:
        """Emit the next output event."""
        event = self._buffer.output()
        if not event:
            return None

        if isinstance(event, betterproto.Message):
            return event

        # Add data integrity to the state
        crc_message = None
        self._crc_receiver.input(event.data)
        try:
            crc_message = self._crc_receiver.output()
        except exceptions.ProtocolDataError as err:
            raise exceptions.ProtocolDataError(
                'CRCSender({}):{}'.format(event.state_type, str(err))
            )

        if not crc_message:
            return None
        message = None
        self._message_receiver.input(crc_message)
        try:
            message = self._message_receiver.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('MessageSender:')

        # check whether file name and data type are same
        if event.state_type != type(message).__name__:
            raise exceptions.ProtocolDataError(
                f"The state type {type(message).__name__} data in the "
                f"file does not match the filename {event.state_type}."
            )
        return message


@attr.s
class Sender(protocols.Filter[UpperEvent, LowerSendEvent]):
    """Filter which unwraps output data from message to raw data."""
    _logger = logging.getLogger('.'.join((__name__, 'Sender')))

    _buffer: channels.DequeChannel[UpperEvent] = attr.ib(
        factory=channels.DequeChannel
    )
    _crc_sender: crcelements.Sender = attr.ib(factory=crcelements.Sender)

    _message_sender: messages.Sender = attr.ib()

    @_message_sender.default
    def init_message_sender(self) -> messages.Sender:  # pylint: disable=no-self-use
        """Initialize the message sender."""
        return messages.Sender(message_types=frontend.MESSAGE_TYPES)

    def input(self, event: Optional[UpperEvent]) -> None:
        """Handle input events."""
        if event:
            self._buffer.input(event)

    def output(self) -> Optional[LowerSendEvent]:
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
            return None

        self._crc_sender.input(message_body)
        try:
            crc_message = self._crc_sender.output()
        except exceptions.ProtocolDataError:
            self._logger.exception('CRCSender:')

        if not crc_message:
            return None

        state_type = type(event).__name__
        return StateData(state_type=state_type, data=crc_message)
