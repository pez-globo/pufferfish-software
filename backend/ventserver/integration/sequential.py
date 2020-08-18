"""Sequential integration of I/O drivers with Sans-I/O protocols."""

import logging

from ventserver.io.sequential import endpoints
from ventserver.protocols import server


logger = logging.getLogger(__name__)


def process_send(
        protocol: server.Protocol, serial: endpoints.IOEndpoint[bytes, bytes]
) -> None:
    """Process all send outputs from the server."""
    for send_output in protocol.send.output_all():
        serial.send(send_output.serial_send)
        logger.info(
            'If there were a WebSocket Driver, it would write: %s',
            send_output.websocket_send
        )


def process_all(
        serial: endpoints.IOEndpoint[bytes, bytes], protocol: server.Protocol
) -> None:
    """Process all data sequentially."""
    process_send(protocol, serial)
    protocol.receive.input_serial(serial.receive())
    for receive_output in protocol.receive.output_all():
        protocol.send.input(receive_output.server_send)
        process_send(protocol, serial)
