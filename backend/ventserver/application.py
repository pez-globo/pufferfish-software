"""Trio I/O with sans-I/O protocol, running application."""

import logging
import functools
import time
from typing import Mapping, Type

import betterproto

import trio

from ventserver.integration import _trio
from ventserver.io.trio import (
    _serial, channels, fileio, rotaryencoder, websocket
)
from ventserver.io.subprocess import frozen_frontend
from ventserver.protocols import exceptions
from ventserver.protocols.backend import server, states
from ventserver.protocols.protobuf import frontend_pb, mcu_pb


FALLBACK_VALUES: Mapping[Type[betterproto.Message], betterproto.Message] = {
    mcu_pb.ParametersRequest: mcu_pb.ParametersRequest(
        mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
        fio2=80, flow=30
    ),
    mcu_pb.AlarmLimitsRequest: mcu_pb.AlarmLimitsRequest(
        fio2=mcu_pb.Range(lower=78, upper=82),
        flow=mcu_pb.Range(lower=28, upper=32),
        spo2=mcu_pb.Range(lower=90, upper=100),
        hr=mcu_pb.Range(lower=60, upper=100),
    ),
    mcu_pb.AlarmMuteRequest: mcu_pb.AlarmMuteRequest(active=False),
    frontend_pb.SystemSettingRequest:
        frontend_pb.SystemSettingRequest(brightness=100, date=int(time.time()))
    # TODO: add fallback for FrontendDisplayRequest, which isn't defined yet
}


async def initialize_states_from_file(
    store: states.Store, protocol: server.Protocol, filehandler: fileio.Handler
) -> None:
    """Initialize states from filesystem and turn off ventilation."""

    # Load state from file
    load_states = list(states.FILE_INPUT_TYPES.keys())
    await _trio.load_file_states(
        load_states, protocol, filehandler, FALLBACK_VALUES
    )

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
    root_logger = logging.getLogger()
    handler = logging.StreamHandler()
    formatter = logging.Formatter(
        '%(asctime)s %(name)-55s %(levelname)-8s %(message)s'
    )
    handler.setFormatter(formatter)
    root_logger.addHandler(handler)
    root_logger.setLevel(logging.INFO)
    logger = logging.getLogger('ventserver.application')

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
    protocol.receive.input(server.ReceiveDataEvent(
        wall_time=time.time(), monotonic_time=time.monotonic()
    ))

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
