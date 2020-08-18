"""Sans-I/O architecture demo."""

import logging
import time

from ventserver.integration import sequential
from ventserver.io import _serial
from ventserver.io.sequential import _serial as seqserial
from ventserver.protocols import backend
from ventserver.protocols import server


logger = logging.getLogger()
handler = logging.StreamHandler()
formatter = logging.Formatter(
    '%(asctime)s %(name)-12s %(levelname)-8s %(message)s'
)
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.INFO)


def main() -> None:
    """Set up wiring between subsystems and infinitely process."""
    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    serial_endpoint = seqserial.Driver(
        props=_serial.SerialProps(port=_serial.find_port(''))
    )

    serial_endpoint.open()
    announcement_timer = time.time()
    announcement_timeout = 2.0
    with serial_endpoint:
        while True:
            if time.time() - announcement_timer > announcement_timeout:
                protocol.send.input(backend.Announcement(
                    message=b'Global announcement!'
                ))
                announcement_timer = time.time()
            sequential.process_all(serial_endpoint, protocol)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        logger.info('Quitting!')
