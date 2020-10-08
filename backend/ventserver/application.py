"""Trio I/O with sans-I/O protocol, running application."""

import logging
from typing import Type, List
import functools

import trio
import betterproto

try:
    import RPi.GPIO as GPIO  # type:ignore
    from ventserver.io.trio import rotaryencoder
except RuntimeError:
    logging.getLogger().warning('Running without RPi.GPIO!')

from ventserver.integration import _trio
from ventserver.io.trio import _serial
from ventserver.io.trio import channels
from ventserver.io.trio import websocket
from ventserver.io.trio import fileio
from ventserver.io.trio import rotaryencoder
from ventserver.protocols import server
from ventserver.protocols.protobuf import mcu_pb


try:
    GPIO.setmode(GPIO.BCM)
except NameError:
    logging.getLogger().warning('Running without RPi.GPIO!')


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
    # pylint: disable=duplicate-code
    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    serial_endpoint = _serial.Driver()
    websocket_endpoint = websocket.Driver()
    filehandler = fileio.Handler()

    rotary_encoder = None
    try:
        rotary_encoder = rotaryencoder.Driver()
        await rotary_encoder.open()
    except NameError:
        rotary_encoder = None
        logger.warning('Running without rotary encoder support!')

    # Server Receive Outputs
    channel: channels.TrioChannel[
        server.ReceiveOutputEvent
    ] = channels.TrioChannel()

    # Initialize State
    states: List[Type[betterproto.Message]] = [
        mcu_pb.Parameters, mcu_pb.CycleMeasurements,
        mcu_pb.SensorMeasurements, mcu_pb.ParametersRequest
    ]
    all_states = protocol.receive.backend.all_states
    await _trio.initialize_states(
        states, protocol, filehandler, all_states
    )

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    # mypy only supports <= 5 args with trio-typing
                    functools.partial(_trio.process_all,
                                      channel=channel,
                                      push_endpoint=channel.push_endpoint),
                    protocol, serial_endpoint,
                    websocket_endpoint, rotary_encoder
                )

                while True:
                    receive_output = await channel.output()
                    await _trio.process_protocol_send(
                        receive_output.server_send, protocol,
                        serial_endpoint, websocket_endpoint,
                        filehandler
                    )
                nursery.cancel_scope.cancel()
    except trio.EndOfChannel:
        logger.info('Finished, quitting!')


if __name__ == '__main__':
    try:
        trio.run(main)
    except KeyboardInterrupt:
        logger.info('Quitting!')
