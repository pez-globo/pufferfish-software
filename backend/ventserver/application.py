"""Trio I/O with sans-I/O protocol, running application."""

import logging
import functools
import random
import time
from typing import Mapping, Optional, Type

import betterproto

import trio

from ventserver.integration import _trio
from ventserver.io import frontend
from ventserver.io.trio import (
    _serial, channels, fileio, rotaryencoder, processes, websocket
)
from ventserver.protocols import exceptions
from ventserver.protocols.backend import server, states
from ventserver.protocols.protobuf import frontend_pb, mcu_pb


INITIAL_SEQ_NUM = random.getrandbits(32)


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
    frontend_pb.SystemSettings: frontend_pb.SystemSettings(
        display_brightness=100, date=time.time(), seq_num=INITIAL_SEQ_NUM
    ),
    frontend_pb.SystemSettingsRequest: frontend_pb.SystemSettingsRequest(
        display_brightness=100, date=time.time(), seq_num=INITIAL_SEQ_NUM
    )
    # TODO: add fallback for FrontendDisplayRequest, which isn't defined yet
}


async def open_rotary_encoder_endpoint(
    logger: logging.Logger
) -> Optional[rotaryencoder.Driver]:
    """Try to start the rotary encoder driver, or fall back to None."""
    try:
        rotary_encoder_endpoint = rotaryencoder.Driver()
        await rotary_encoder_endpoint.open()
        return rotary_encoder_endpoint
    except exceptions.ProtocolError:
        logger.error(
            'Unable to connect to the pigpiod service for the rotary encoder. '
            'Is the service running?'
        )
        logger.warning('Running without rotary encoder support!')
        return None


async def initialize_states_from_file(
    store: states.Store, protocol: server.Protocol,
    fileio_endpoint: fileio.Handler
) -> None:
    """Initialize states from filesystem and turn off ventilation."""

    # Load state from file
    load_states = list(states.FILE_INPUT_TYPES.keys())
    await _trio.load_file_states(
        load_states, protocol, fileio_endpoint, FALLBACK_VALUES
    )

    # Turn off ventilation
    parameters_request = store[states.StateSegment.PARAMETERS_REQUEST]
    if parameters_request is not None:
        parameters_request.ventilating = False


async def handle_receive_outputs(
    receive_output: server.ReceiveOutputEvent,
    protocol: server.Protocol,
    serial_endpoint: Optional[_serial.Driver],
    websocket_endpoint: Optional[websocket.Driver],
    fileio_endpoint: fileio.Handler,
    nursery: trio.Nursery
) -> None:
    """Handle actions indicated by server protocol receive output events."""
    logger = logging.getLogger('ventserver.application')

    await _trio.process_protocol_send(
        receive_output.states_send, protocol,
        serial_endpoint, websocket_endpoint, fileio_endpoint
    )

    if receive_output.sysclock_setting is not None:
        logger.info(
            'Changing system clock from %s to %s',
            time.time(), receive_output.sysclock_setting
        )

    if receive_output.kill_frontend:
        nursery.start_soon(
            processes.kill_process,
            frontend.FrontendProps().process_name
        )


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
    fileio_endpoint = fileio.Handler()
    rotary_encoder_endpoint = await open_rotary_encoder_endpoint(logger)

    # Server Receive Outputs
    channel: channels.TrioChannel[server.ReceiveOutputEvent] = \
        channels.TrioChannel()

    # Initialize states
    store = protocol.receive.backend.store
    await initialize_states_from_file(store, protocol, fileio_endpoint)

    # Initialize time
    protocol.receive.input(server.ReceiveDataEvent(
        wall_time=time.time(), monotonic_time=time.monotonic()
    ))

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    # mypy only supports <= 5 args with trio-typing
                    functools.partial(
                        _trio.process_all, channel=channel,
                        push_endpoint=channel.push_endpoint
                    ),
                    protocol,
                    serial_endpoint, websocket_endpoint, rotary_encoder_endpoint
                )

                while True:
                    receive_output = await channel.output()
                    await handle_receive_outputs(
                        receive_output, protocol,
                        serial_endpoint, websocket_endpoint, fileio_endpoint,
                        nursery
                    )

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
