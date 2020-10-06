"""Trio I/O Rotary Encoder driver."""

import logging
from typing import Optional, Tuple
import attr

import trio
import pigpio

from ventserver.io import rotaryencoder
from ventserver.io.trio import endpoints
from ventserver.protocols import exceptions


# States
dt_gpio1 = 'D' # dt_gpio is high
dt_gpio0 = 'd' # dt_gpio is low
clk_gpio1 = 'C' # clk_gpio is high
clk_gpio0 = 'c' # clk_gpio is low

# State sequences
SEQUENCE_UP = dt_gpio1 + clk_gpio1 + dt_gpio0 + clk_gpio0
SEQUENCE_DOWN = clk_gpio1 + dt_gpio1 + clk_gpio0 + dt_gpio0

@attr.s
class RotaryEncoderState:
    """Holds state values for communication and computation."""

    a_quad_state: int = attr.ib(default=None, repr=False)
    a_quad_last_state: int = attr.ib(default=None, repr=False)
    b_quad_state: int = attr.ib(default=None, repr=False)
    counter: int = attr.ib(default=0, repr=False)
    button_pressed: bool = attr.ib(default=False, repr=False)
    last_pressed: int = attr.ib(default=None, repr=False)
    debounce_time: int = attr.ib(default=300) # debounce time in us


@attr.s
class Driver(endpoints.IOEndpoint[bytes, Tuple[int, bool]]):
    """Implements driver for reading rotary encoder inputs."""

    _logger = logging.getLogger('.'.join((__name__, 'Driver')))

    _props: rotaryencoder.RotaryEncoderProps = attr.ib(
        factory=rotaryencoder.RotaryEncoderProps
    )
    pi = attr.ib(factory=pigpio.pi)
    _connected: bool = attr.ib(default=False)
    _data_available: trio.Event = attr.ib(factory=trio.Event)
    _state: RotaryEncoderState = attr.ib(factory=RotaryEncoderState)
    trio_token: trio.lowlevel.TrioToken = attr.ib(default=None, repr=False)
    sequence: str = attr.ib(default='')



    def a_quad_rise(self, gpio, level, tick) -> None:
        self.sequence += clk_gpio0
        if self.sequence == SEQUENCE_UP:
            self._state.counter += 1
            self.sequence = ''
            trio.from_thread.run_sync(
                self._data_available.set,
                trio_token=self.trio_token
            )

    def a_quad_fall(self, gpio, level, tick) -> None:
        if len(self.sequence) > 2:
            self.sequence = ''
        self.sequence += clk_gpio1

    def b_quad_rise(self, gpio, level, tick) -> None:
        self.sequence += dt_gpio0
        if self.sequence == SEQUENCE_DOWN:
            self._state.counter -= 1
            self.sequence = ''
            trio.from_thread.run_sync(
                self._data_available.set,
                trio_token=self.trio_token
            )

    def b_quad_fall(self, gpio, level, tick) -> None:
        if len(self.sequence) > 2:
            self.sequence = ''
        self.sequence += dt_gpio1


    @property
    def is_data_available(self) -> bool:
        """Return whether or not new state is available or not."""
        return self._data_available.is_set()

    @property
    def is_open(self) -> bool:
        """Return whether or not the rotary encoder is connected."""
        return self._connected


    async def open(self, nursery: Optional[trio.Nursery] = None) -> None:
        """Opens the connection with the rotary encoder.
        Raises:
            ProtocolError:"Rotary Encoder is already connected at A:a_quad,
                B:b_quad GPIO pins."

            IOError
        """

        if self.is_open:
            exception = ("Rotary Encoder is already connected at "
                         f"A:{self._props.a_quad_pin}, "
                         "B:{self._props.b_quad_pin} GPIO pins.")
            raise exceptions.ProtocolError(exception)

        try:

            self.pi.set_glitch_filter(
                self._props.a_quad_pin,
                self._state.debounce_time
            )
            self.pi.callback(
                self._props.a_quad_pin,
                pigpio.FALLING_EDGE,
                self.a_quad_fall
            )
            self.pi.callback(
                self._props.a_quad_pin,
                pigpio.RISING_EDGE,
                self.a_quad_rise
            )
            self.pi.set_glitch_filter(
                self._props.b_quad_pin,
                self._state.debounce_time
            )
            self.pi.callback(
                self._props.b_quad_pin,
                pigpio.FALLING_EDGE,
                self.b_quad_fall
            )
            self.pi.callback(
                self._props.b_quad_pin,
                pigpio.RISING_EDGE, 
                self.b_quad_rise
            )            
        except Exception as err:
            raise IOError(err)

        self._connected = True
        self.trio_token = trio.lowlevel.current_trio_token()


    async def close(self) -> None:
        """Closes the connection with the rotary encoder.

         Closes the connection by cleaning up the GPIO pins on
         the Raspberry Pi board.

        """
        self._connected = False


    async def receive(self) -> Tuple[int, bool]:
        """Shares current rotation counts and button
        pressed states with the caller.

        Waits for the the rotary encoder to change state and the callback
        function to set the trio.Event.

        Raises:
            ProtocolError: No Rotary Encoder is connected.Try checking
                your RPi Connections.
        """

        if not self.is_open:
            exception = ("No Rotary Encoder is connected. "
                         "Try checking your RPi Connections.")
            raise exceptions.ProtocolError(exception)

        await self._data_available.wait()
        self._data_available = trio.Event()
        return (self._state.counter, self._state.button_pressed)


    async def send(self, data: Optional[bytes]) -> None:
        """Defined just to fulfill requirements of the
        IOEndpoint's abstract class."""
        return
