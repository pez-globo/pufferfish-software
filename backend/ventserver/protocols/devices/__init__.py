"""Sans-I/O protocols for I/O devices for the ventserver backend application.

These protocols integrate transport-level protocols into protocols for devices.

Modules:
    mcu: protocol for converting between a bytestream and MCU events.
    frontend: protocol for converting between bytestrings and frontend events.
    file: protocol for converting between file binary data and file I/O events.
    rotary_encoder: protocol for processing rotary encoder events.
"""
