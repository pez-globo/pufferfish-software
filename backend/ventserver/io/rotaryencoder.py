"""" Support for Rotary Encoder communication """

import attr
import logging
try:
    import RPi.GPIO as GPIO     # type: ignore
except RuntimeError:
    logging.getLogger().warning('Running without RPi.GPIO!')


@attr.s(auto_attribs=True)
class RotaryEncoderProps:
    """Rotary encoder communication properties."""
    mode = GPIO.BCM
    a_quad_pin: int = 18 # clk
    b_quad_pin: int = 17 # dt
    button_pin: int = 27 # sw
