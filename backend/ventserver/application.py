"""Trio I/O with sans-I/O protocol, running application."""

import logging
from typing import List, Type
import functools
import time

import trio
import betterproto

from ventserver.integration import _trio
from ventserver.io.trio import (
    _serial, channels, fileio, rotaryencoder, websocket
)
from ventserver.io.subprocess import frozen_frontend
from ventserver.protocols import exceptions
from ventserver.protocols.backend import server, states
from ventserver.protocols.protobuf import mcu_pb


async def initialize_states_from_file(
    store: states.Store, protocol: server.Protocol, filehandler: fileio.Handler
) -> None:
    """Initialize states from filesystem and turn off ventilation."""

    # Load state from file
    load_states: List[Type[betterproto.Message]] = [
        mcu_pb.Parameters, mcu_pb.CycleMeasurements,
        mcu_pb.SensorMeasurements, mcu_pb.ParametersRequest
    ]
    await _trio.load_file_states(load_states, protocol, filehandler)

    # Turn off ventilation
    parameters_request = store[states.StateSegment.PARAMETERS_REQUEST]
    if parameters_request is not None:
        parameters_request.ventilating = False


def filter_multierror(exc: trio.MultiError) -> None:
    """Filter out trio.MultiErrors from KeyboardInterrupts."""
    if len(exc.exceptions) != 2:
        raise exc
    if (
            not isinstance(exc.exceptions[0], KeyboardInterrupt) or
            not isinstance(exc.exceptions[1], trio.EndOfChannel)
    ):
        raise exc


async def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    # Configure logging
    logger = logging.getLogger()
    handler = logging.StreamHandler()
    formatter = logging.Formatter(
        '%(asctime)s %(name)-12s %(levelname)-8s %(message)s'
    )
    handler.setFormatter(formatter)
    logger.addHandler(handler)
    logger.setLevel(logging.INFO)

    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    serial_endpoint = _serial.Driver()
    websocket_endpoint = websocket.Driver()
    filehandler = fileio.Handler()

    rotary_encoder = None
    try:
        rotary_encoder = rotaryencoder.Driver()
        try:
            await rotary_encoder.open()
        except exceptions.ProtocolError as err:
            exception = (
                "Unable to connect the rotary encoder, please check the "
                "serial connection. Check if the pigpiod service is running: %s"
            )
            rotary_encoder = None
            logger.error(exception, err)
    except NameError:
        rotary_encoder = None
        logger.warning('Running without rotary encoder support!')

    # Server Receive Outputs
    channel: channels.TrioChannel[
        server.ReceiveOutputEvent
    ] = channels.TrioChannel()

    # Initialize states
    store = protocol.receive.backend.store
    await initialize_states_from_file(store, protocol, filehandler)

    # Initialize time
    protocol.receive.input(server.ReceiveDataEvent(time=time.time()))

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

                    if receive_output.kill_frontend:
                        nursery.start_soon(frozen_frontend.kill_frozen_frontend)
                nursery.cancel_scope.cancel()
    except trio.EndOfChannel:
        logger.info('Finished, quitting!')
    except KeyboardInterrupt:
        logger.info('Quitting!')
    except trio.MultiError as exc:
        filter_multierror(exc)
        logger.info('Finished, quitting!')


if __name__ == '__main__':
    trio.run(main)
