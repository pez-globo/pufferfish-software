"""Trio I/O with sans-I/O protocol, running a simulation.

Instead of connecting to the microcontroller and relying on it to update the
state variables and measurements related to the breathing circuit, this backend
program simulates the evolution of those variables. This allows this backend
server to act as a mock in place of the real backend server.
"""

import logging
import time
import functools
from typing import Mapping, MutableMapping, Optional

import betterproto
import trio

from ventserver.integration import _trio
from ventserver.io.trio import channels, fileio, rotaryencoder, websocket
from ventserver.io.subprocess import frozen_frontend
from ventserver.protocols import exceptions
from ventserver.protocols.backend import server, states
from ventserver.protocols.protobuf import mcu_pb
from ventserver.simulation import (
    alarm_limits, alarms, log, parameters, simulators
)
from ventserver import application


# Simulators


REQUEST_SERVICE_INTERVAL = 20


async def service_requests(
        store: Mapping[
            states.StateSegment, Optional[betterproto.Message]
        ], log_manager: log.Manager
) -> None:
    """Simulate evolution of all states."""
    parameters_services = parameters.Services()
    alarm_limits_services = alarm_limits.Services()

    while True:
        parameters_services.transform(time.time(), store, log_manager)
        alarm_limits_services.transform(time.time(), store, log_manager)
        await trio.sleep(REQUEST_SERVICE_INTERVAL / 1000)


async def simulate_states(
        store: Mapping[
            states.StateSegment, Optional[betterproto.Message]
        ], log_manager: log.Manager
) -> None:
    """Simulate evolution of all states."""
    simulation_services = simulators.Services()
    alarms_services = alarms.Services()

    while True:
        simulation_services.transform(time.time(), store)
        alarms_services.transform(time.time(), store, log_manager)
        await trio.sleep(simulators.SENSOR_UPDATE_INTERVAL / 1000)


def initialize_states(store: MutableMapping[
        states.StateSegment, Optional[betterproto.Message]
]) -> None:
    """Set initial values for the states."""
    for segment_type in store:
        if segment_type is states.StateSegment.PARAMETERS_REQUEST:
            store[segment_type] = mcu_pb.ParametersRequest(
                mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
                fio2=21, flow=0
            )
        elif segment_type is states.StateSegment.PARAMETERS:
            store[segment_type] = mcu_pb.Parameters(
                mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
                fio2=21, flow=0
            )
        elif segment_type is states.StateSegment.SENSOR_MEASUREMENTS:
            store[segment_type] = mcu_pb.SensorMeasurements()
        elif segment_type is states.StateSegment.ALARM_LIMITS_REQUEST:
            store[segment_type] = mcu_pb.AlarmLimitsRequest(
                fio2=mcu_pb.Range(lower=21, upper=23),
                flow=mcu_pb.Range(lower=-2, upper=2),
                spo2=mcu_pb.Range(lower=21, upper=100),
                hr=mcu_pb.Range(lower=0, upper=200),
            )
        elif segment_type is states.StateSegment.ALARM_LIMITS:
            store[segment_type] = mcu_pb.AlarmLimits(
                fio2=mcu_pb.Range(lower=21, upper=23),
                flow=mcu_pb.Range(lower=-2, upper=2),
                spo2=mcu_pb.Range(lower=21, upper=100),
                hr=mcu_pb.Range(lower=0, upper=200),
            )
        elif segment_type is states.StateSegment.ACTIVE_LOG_EVENTS_MCU:
            store[segment_type] = mcu_pb.ActiveLogEvents()


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
    store = protocol.receive.backend.store
    initialize_states(store)
    await application.initialize_states_from_file(
        store, protocol, filehandler
    )

    # Initialize events log manager
    log_manager = log.Manager(
        receiver=protocol.receive.backend.  # pylint: disable=protected-access
        _event_log_receiver.  # pylint: disable=protected-access
        _log_events_receiver
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
                nursery.start_soon(service_requests, store, log_manager)
                nursery.start_soon(simulate_states, store, log_manager)

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
