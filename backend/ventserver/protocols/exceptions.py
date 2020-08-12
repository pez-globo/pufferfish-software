"""Exceptions raised by modules in this sub-package."""


class ProtocolError(Exception):
    """Basic exception for errors raised from communication protocols.

    Only used for errors raised during input and output calls of the protocols,
    not during initialization.
    """


class ProtocolDataError(ProtocolError, ValueError):
    """Basic exception for problems with data to be sent/received."""
