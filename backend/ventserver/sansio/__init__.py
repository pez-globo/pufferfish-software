"""Abstract interfaces and basic components for writing sans-I/O protocols.

Modules:
    protocols: abstract interfaces for writing sans-I/O protocols with an
        h11-like design.
    channels: sans-I/O component for buffering event-oriented input data, to be
        used by other sans-I/O protocols for handling input data.
    streams: sans-I/O component for buffering bytestream-oriented input data, to
        be used by other sans-I/O protocols for handling input data.
"""
