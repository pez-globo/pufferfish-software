"""Reusable Sans-I/O protocols for all application-layer protocol logic.

These protocols handle high-level application events. They are intended to be
composed into application-level communication protocols for specific
applications, such as the ventserver backend.

Modules:
    states: protocol logic for state synchronization.
    lists: protocol logic for list synchronization.
    clocks: protocol logic for time synchronization on independent clocks.
"""
