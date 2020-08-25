"""Sans-I/O protocol interfaces with an h11-like design.

Provides an abstract Filter class which defines a transformation between input
events and output events. Provides an abstract Protocol class which associates
two Filters transforming events in opposite directions (receive and send).

    Typical usage example:

    @attr.s
    class ToUpperFilter(Filter[str, str]):
        last_input: Optional[str] = attr.ib(default=None)

        def input(self, event: Optional[str]) -> None:
            if event is None:
                return

            self.last_input = event

        def output(self) -> Optional[str]:
            if self.last_input is None:
                return None

            last_input = self.last_input.upper()
            self.last_input = None
            return last_input

    @attr.s
    class ToLowerFilter(Filter[str, str]):
        last_input: Optional[str] = attr.ib(default=None)

        def input(self, event: Optional[str]) -> None:
            if event is None:
                return

            self.last_input = event

        def output(self) -> Optional[str]:
            if self.last_input is None:
                return None

            last_input = self.last_input.lower()
            self.last_input = None
            return last_input

    to_upper_filter = ToUpperFilter()
    to_upper_filter.input('foo')
    to_upper_filter.input('bar')
    print(to_upper_filter.output()  # 'BAR'
    to_upper_filter.input('foo')
    for output in to_upper_filter.output_all():
        print(output)  # 'FOO'

    @attr.s
    class UpperLowerProtocol(Protocol[str, str, str, str]):
        to_upper_filter: ToUpperFilter = attr.ib(factory=ToUpperFilter)
        to_lower_filter: ToLowerFilter = attr.ib(factory=ToLowerFilter)

        @property
        def receive(self) -> ToUpperFilter:
            return self.to_upper_filter

        @property
        def send(self) -> ToLowerFilter:
            return self.to_lower_filter

    upper_lower_protocol = UpperLowerProtocol()
    upper_lower_protocol.send.input('FOO')
    print(upper_lower_protocol.send.output())  # 'foo'
    upper_lower_protocol.receive.input('bar')
    print(upper_lower_protocol.receive.output())  # 'BAR'
"""

import abc
from typing import Generator, Generic, Optional, TypeVar


# Filters


_InputEvent = TypeVar('_InputEvent')
_OutputEvent = TypeVar('_OutputEvent')


class Filter(Generic[_InputEvent, _OutputEvent], abc.ABC):
    """Interface class for filters in a pipe-and-filter architecture.

    This interface defines a filter in which processing and emission of output
    events is done one step at a time, allowing for querying of any public state
    exposed by the Filter between output calls. The Filter's internal state
    (private or public) may be modified by the input and output methods.

    If the filter is expected to be used by calling code with any form of
    asynchronous or concurrent flow control, the filter should not contain any
    state shared with the outside world, such that it only communicates with
    other software components through its input and output methods.
    """

    @abc.abstractmethod
    def input(self, event: Optional[_InputEvent]) -> None:
        """Handle an input event by modifying internal state accordingly.

        Typically this just will put the input event onto an internal buffer to
        be processed sequentially in repeated calls of the output method.

        Args:
            event: An optional event for the filter to process as input. If it
                is None, it should be ignored and internal state should not be
                modified.

        Raises:
            May raise an exception if the input is rejected.

        """

    @abc.abstractmethod
    def output(self) -> Optional[_OutputEvent]:
        """Process internal state to emit the next output event.

        Returns:
            The next output from the filter's processing of its internal state,
            or None if no output is available.

        Raises:
            May raise an exception if the internal state is invalid.

        """

    def output_all(self) -> Generator[_OutputEvent, None, None]:
        """Yield output events while they are available.

        The calling code is allowed to query any public state exposed by the
        Filter between every output yielded by this method. Note that this
        method will never yield None, as that is the sentinel value to indicate
        that no outputs are available.
        """
        while True:
            output = self.output()
            if output is None:
                return
            yield output


# Protocols


ReceiveInputEvent = TypeVar('ReceiveInputEvent')
ReceiveOutputEvent = TypeVar('ReceiveOutputEvent')
SendInputEvent = TypeVar('SendInputEvent')
SendOutputEvent = TypeVar('SendOutputEvent')


class Protocol(Generic[
        ReceiveInputEvent, ReceiveOutputEvent, SendInputEvent, SendOutputEvent
], abc.ABC):
    """Interface class for send/receive protocols.

    This interface associates a pair of Filters with semantic directions of data
    flow - one filter defines the receive direction for transforming data, while
    the other filter defines the send direction for transforming data.
    """

    @property
    @abc.abstractmethod
    def receive(self) -> Filter[ReceiveInputEvent, ReceiveOutputEvent]:
        """Return a Filter interface for the receive direction."""

    @property
    @abc.abstractmethod
    def send(self) -> Filter[SendInputEvent, SendOutputEvent]:
        """Return a Filter interface for the send direction."""
