"""Trio I/O with sans-I/O protocol, running a simulation.

Instead of connecting to the microcontroller and relying on it to update the
state variables and measurements related to the breathing circuit, this backend
program simulates the evolution of those variables. This allows this backend
server to act as a mock in place of the real backend server.
"""

import logging
import time
import functools
import typing
from typing import Mapping, MutableMapping, Optional

import betterproto
import trio

from ventserver.integration import _trio
from ventserver.io.trio import channels, fileio, rotaryencoder, websocket
from ventserver.io.subprocess import frozen_frontend
from ventserver.protocols import exceptions
from ventserver.protocols.application import lists
from ventserver.protocols.backend import alarms, log, server, states
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.simulation import (
    alarm_limits, alarm_mute, alarms as sim_alarms,
    parameters, power_management, simulators
)
from ventserver import application


# Simulators


REQUEST_SERVICE_INTERVAL = 20

INITIAL_VALUES = {
    states.StateSegment.SENSOR_MEASUREMENTS: mcu_pb.SensorMeasurements(),
    states.StateSegment.PARAMETERS: mcu_pb.Parameters(
        mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
        fio2=21, flow=0
    ),
    states.StateSegment.PARAMETERS_REQUEST: mcu_pb.ParametersRequest(
        mode=mcu_pb.VentilationMode.hfnc, ventilating=False,
        fio2=80, flow=30
    ),
    states.StateSegment.ALARM_LIMITS: mcu_pb.AlarmLimits(
        fio2=mcu_pb.Range(lower=21, upper=23),
        flow=mcu_pb.Range(lower=-2, upper=2),
        spo2=mcu_pb.Range(lower=21, upper=100),
        hr=mcu_pb.Range(lower=0, upper=200),
    ),
    states.StateSegment.ALARM_LIMITS_REQUEST: mcu_pb.AlarmLimitsRequest(
        fio2=mcu_pb.Range(lower=78, upper=82),
        flow=mcu_pb.Range(lower=28, upper=32),
        spo2=mcu_pb.Range(lower=90, upper=100),
        hr=mcu_pb.Range(lower=60, upper=100),
    ),
    states.StateSegment.ACTIVE_LOG_EVENTS_MCU: mcu_pb.ActiveLogEvents(),
    states.StateSegment.ALARM_MUTE: mcu_pb.AlarmMute(
        active=False, remaining=120
    ),
    states.StateSegment.ALARM_MUTE_REQUEST: mcu_pb.AlarmMuteRequest(
        active=False, remaining=120
    ),
    states.StateSegment.MCU_POWER_STATUS: mcu_pb.MCUPowerStatus(
        power_left=0, charging=True
    ),
    states.StateSegment.SCREEN_STATUS: mcu_pb.ScreenStatus(lock=False),
    states.StateSegment.SYSTEM_SETTING_REQUEST:
        frontend_pb.SystemSettingRequest(brightness=100, date=int(time.time())),
    states.StateSegment.FRONTEND_DISPLAY: frontend_pb.FrontendDisplaySetting(
        theme=frontend_pb.ThemeVariant.dark,
        unit=frontend_pb.Unit.metric,
    )
    # TODO: initial value for FrontendDisplayRequest, which isn't defined yet
}


def service_event_log(
    simulated_log: alarms.Manager, active_log_events: mcu_pb.ActiveLogEvents,
    simulated_log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Output outstanding events."""
    result = simulated_log.output()
    if result is None:
        return

    simulated_log_receiver.input(result.next_log_events)
    if result.active_log_events is not None:
        active_log_events.id = result.active_log_events.id


async def service_requests(
    store: Mapping[states.StateSegment, Optional[betterproto.Message]],
    simulated_log: alarms.Manager,
    simulated_log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Simulate evolution of all states."""
    parameters_services = parameters.Services()
    alarm_limits_services = alarm_limits.Services()
    active_log_events = typing.cast(
        mcu_pb.ActiveLogEvents,
        store[states.StateSegment.ACTIVE_LOG_EVENTS_MCU]
    )

    while True:
        simulated_log.input(log.LocalLogInputEvent(current_time=time.time()))
        parameters_services.transform(store, simulated_log)
        alarm_limits_services.transform(store, simulated_log)
        service_event_log(
            simulated_log, active_log_events, simulated_log_receiver
        )
        await trio.sleep(REQUEST_SERVICE_INTERVAL / 1000)


async def simulate_states(
        store: Mapping[
            states.StateSegment, Optional[betterproto.Message]
        ], simulated_log: alarms.Manager,
        simulated_log_receiver: lists.ReceiveSynchronizer[mcu_pb.LogEvent]
) -> None:
    """Simulate evolution of all states."""
    simulation_services = simulators.Services()
    alarms_services = sim_alarms.Services()
    alarm_mute_service = alarm_mute.AlarmMuteService()
    power_service = power_management.Service()
    active_log_events = typing.cast(
        mcu_pb.ActiveLogEvents,
        store[states.StateSegment.ACTIVE_LOG_EVENTS_MCU]
    )

    while True:
        simulated_log.input(log.LocalLogInputEvent(current_time=time.time()))
        simulation_services.transform(time.time(), store)
        alarms_services.transform(store, simulated_log)
        alarm_mute_service.transform(store)
        power_service.transform(store, simulated_log)
        service_event_log(
            simulated_log, active_log_events, simulated_log_receiver
        )
        await trio.sleep(simulators.SENSOR_UPDATE_INTERVAL / 1000)


def initialize_states(store: MutableMapping[
        states.StateSegment, Optional[betterproto.Message]
]) -> None:
    """Set initial values for the states."""
    for segment_type in store:
        if segment_type in INITIAL_VALUES:
            store[segment_type] = INITIAL_VALUES[segment_type]


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
    simulated_log = alarms.Manager()
    simulated_log_receiver = (
        protocol.receive.backend.  # pylint: disable=protected-access
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
                nursery.start_soon(
                    service_requests, store,
                    simulated_log, simulated_log_receiver
                )
                nursery.start_soon(
                    simulate_states, store,
                    simulated_log, simulated_log_receiver
                )

                while True:
                    receive_output = await channel.output()
                    await _trio.process_protocol_send(
                        receive_output.server_send, protocol,
                        None, websocket_endpoint, filehandler
                    )

                    if receive_output.kill_frontend:
                        nursery.start_soon(frozen_frontend.kill_frozen_frontend)

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
