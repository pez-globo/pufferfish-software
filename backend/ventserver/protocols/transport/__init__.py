"""Reusable Sans-I/O protocols for transport of data.

These protocols handle serialization and deserialization of data, including
fault tolerance over unreliable communication links. They are intended to be
composed into transport-level communication protocols for specific I/O devices.

Modules:
    messages: protocol logic for data serialization/deserialization.
    crcelements: protocol logic for data integrity checking in datagrams.
    datagrams: protocol logic for data loss detection in frames.
    frames: protocol logic for data framing over a bytestream.
"""
