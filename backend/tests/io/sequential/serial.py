"""Trio I/O with sans-I/O protocol demo, running on real I/O."""

import logging
import time

from ventserver.io.sequential import _serial


logger = logging.getLogger()
handler = logging.StreamHandler()
formatter = logging.Formatter(
    '%(asctime)s %(name)-12s %(levelname)-8s %(message)s'
)
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.INFO)


def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    serial = _serial.Driver()

    with serial:
        while True:
            time.sleep(1.0)
            logger.info('Sending ping...')
            serial.send(b'ping!\0')
            logger.info('Received: %s', serial.receive())


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        logger.info('Quitting!')
