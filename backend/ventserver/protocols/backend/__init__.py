"""Sans-I/O protocols specifically for the ventserver backend application.

These protocols integrate other protocols into a single protocol for the
Pufferfish backend server.

Modules:
    states: protocol integrating state synchronizers and defining state types.
    log: protocol for synchronizing event logs.
    backend: application-level communication protocol between device protocols.
    server: protocol integrating the backend protocol with device protocols.
"""
