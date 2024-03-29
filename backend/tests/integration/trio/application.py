"""Trio I/O with sans-I/O protocol demo, running application demo."""

import logging
import time
import functools

import trio

from ventserver.integration import _trio
from ventserver.io.trio import _serial, channels, fileio, websocket
from ventserver.protocols.backend import server, states
from ventserver.protocols.protobuf import mcu_pb as pb


logger = logging.getLogger()
handler = logging.StreamHandler()
formatter = logging.Formatter(
    '%(asctime)s %(name)-12s %(levelname)-8s %(message)s'
)
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.INFO)


async def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    serial_endpoint = _serial.Driver()
    websocket_endpoint = websocket.Driver()
    filehandler = fileio.Handler()

    # Server Receive Outputs
    channel: channels.TrioChannel[
        server.ReceiveOutputEvent
    ] = channels.TrioChannel()

    # Initialize State
    protocol.receive.backend.store[
        states.StateSegment.PARAMETERS
    ] = pb.Parameters(pip=30, peep=10)

    report_interval = 0.03
    last_report_time = time.time()
    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    functools.partial(_trio.process_all,
                                      channel=channel,
                                      push_endpoint=channel.push_endpoint),
                    protocol, serial_endpoint, websocket_endpoint, None,
                )

                while True:
                    receive_output = await channel.output()
                    if time.time() - last_report_time >= report_interval:
                        logger.info(protocol.receive.backend.store[
                            states.StateSegment.SENSOR_MEASUREMENTS
                        ])
                        last_report_time = time.time()
                    await _trio.process_protocol_send(
                        receive_output.states_send, protocol,
                        serial_endpoint, websocket_endpoint, filehandler
                    )
                nursery.cancel_scope.cancel()
    except trio.EndOfChannel:
        logger.info('Finished, quitting!')


if __name__ == '__main__':
    try:
        trio.run(main)
    except KeyboardInterrupt:
        logger.info('Quitting!')
