"""Trio I/O with sans-I/O protocol, running a simulation.

Instead of connecting to the microcontroller and relying on it to update the
state variables and measurements related to the breathing circuit, this backend
program simulates the evolution of those variables. This allows this backend
server to act as a mock in place of the real backend server.
"""

import logging
import time
import functools
from typing import Mapping, Optional, Type

import betterproto
import trio

from ventserver.io.trio import rotaryencoder
from ventserver.integration import _trio
from ventserver.io.trio import channels
from ventserver.io.trio import websocket
from ventserver.io.trio import fileio
from ventserver.io.subprocess import frozen_frontend
from ventserver.protocols import server
from ventserver.protocols import exceptions
from ventserver.protocols.application import lists
from ventserver.protocols.protobuf import mcu_pb
from ventserver.simulation import alarm_limits, alarms, parameters, simulators
from ventserver import application


# Simulators


REQUEST_SERVICE_INTERVAL = 20


async def service_requests(
        all_states: Mapping[
            Type[betterproto.Message], Optional[betterproto.Message]
        ]
) -> None:
    """Simulate evolution of all states."""
    parameters_services = parameters.Services()
    alarm_limits_services = alarm_limits.Services()

    while True:
        parameters_services.transform(all_states)
        alarm_limits_services.transform(all_states)
        await trio.sleep(REQUEST_SERVICE_INTERVAL / 1000)


async def simulate_states(
        all_states: Mapping[
            Type[betterproto.Message], Optional[betterproto.Message]
        ],
        log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Simulate evolution of all states."""
    simulation_services = simulators.Services()
    alarms_services = alarms.Services()

    while True:
        simulation_services.transform(time.time(), all_states)
        alarms_services.transform(time.time(), all_states, log_events_sender)
        await trio.sleep(simulators.SENSOR_UPDATE_INTERVAL / 1000)


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
    websocket_endpoint = websocket.Driver()

    rotary_encoder = None
    try:
        rotary_encoder = rotaryencoder.Driver()
        await rotary_encoder.open()
    except exceptions.ProtocolError as err:
        exception = (
            "Unable to connect the rotary encoder, please check the "
            "serial connection. Check if the pigpiod service is running: %s"
        )
        rotary_encoder = None
        logger.error(exception, err)

    # I/O File
    filehandler = fileio.Handler()

    # Server Receive Outputs
    channel: channels.TrioChannel[
        server.ReceiveOutputEvent
    ] = channels.TrioChannel()

    # Initialize states with defaults
    all_states = protocol.receive.backend.all_states
    for state in all_states:
        if state is mcu_pb.ParametersRequest:
            all_states[state] = mcu_pb.ParametersRequest(
                mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
                fio2=60, flow=6
            )
        else:
            all_states[state] = state()
    await application.initialize_states_from_file(
        all_states, protocol, filehandler
    )

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    functools.partial(
                        _trio.process_all, channel=channel,
                        push_endpoint=channel.push_endpoint
                    ),
                    protocol, None, websocket_endpoint, rotary_encoder
                )
                nursery.start_soon(service_requests, all_states)
                nursery.start_soon(
                    simulate_states, all_states,
                    protocol.receive.backend.log_events_sender
                )

                while True:
                    receive_output = await channel.output()
                    await _trio.process_protocol_send(
                        receive_output.server_send, protocol,
                        None, websocket_endpoint, filehandler
                    )

                    if receive_output.frontend_delayed:
                        print("calling")
                        nursery.start_soon(
                            frozen_frontend.kill_frozen_frontend
                        )

                nursery.cancel_scope.cancel()
    except trio.EndOfChannel:
        logger.info('Finished, quitting!')
    except KeyboardInterrupt:
        logger.info('Quitting!')
    except trio.MultiError as exc:
        application.filter_multierror(exc)
        logger.info('Finished, quitting!')


if __name__ == '__main__':
    trio.run(main)
