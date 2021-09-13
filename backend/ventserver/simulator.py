"""Trio I/O with sans-I/O protocol, running a simulation.

Instead of connecting to the microcontroller and relying on it to update the
state variables and measurements related to the breathing circuit, this backend
program simulates the evolution of those variables. This allows this backend
server to act as a mock in place of the real backend server.
"""

import functools
import logging
import time
import random
import typing
from typing import Mapping, Optional

import better_exceptions  # type: ignore
import betterproto
import trio

from ventserver.integration import _trio
from ventserver.io.trio import channels, fileio, processes, websocket
from ventserver.protocols.application import debouncing, lists
from ventserver.protocols.backend import alarms, log, server, states
from ventserver.protocols.protobuf import frontend_pb, mcu_pb
from ventserver.simulation import (
    alarm_limits, alarm_mute, alarms as sim_alarms,
    parameters, power_management, simulators, screen_status
)
from ventserver import application


# Simulators


REQUEST_SERVICE_INTERVAL = 20

INITIAL_SEQ_NUM = random.getrandbits(32)

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
        active=False
    ),
    states.StateSegment.MCU_POWER_STATUS: mcu_pb.MCUPowerStatus(
        power_left=0, charging=True
    ),
    states.StateSegment.SCREEN_STATUS: mcu_pb.ScreenStatus(lock=False),
    states.StateSegment.SCREEN_STATUS_REQUEST: mcu_pb.ScreenStatusRequest(lock=False),
    states.StateSegment.SYSTEM_SETTINGS: frontend_pb.SystemSettings(
        display_brightness=100, date=time.time(), seq_num=INITIAL_SEQ_NUM
    ),
    states.StateSegment.SYSTEM_SETTINGS_REQUEST:
        frontend_pb.SystemSettingsRequest(
            display_brightness=100, date=time.time(), seq_num=INITIAL_SEQ_NUM
        ),
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
        simulated_log.input(log.LocalLogInputEvent(wall_time=time.time()))
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
    logger = logging.getLogger('ventserver.simulator')

    simulation_services = simulators.Services()
    power_service = power_management.Service()
    alarms_services = sim_alarms.Services()
    alarm_mute_service = alarm_mute.Service()
    screen_status_service = screen_status.Service()
    active_log_events = typing.cast(
        mcu_pb.ActiveLogEvents,
        store[states.StateSegment.ACTIVE_LOG_EVENTS_MCU]
    )
    alarm_mute_ = typing.cast(
        mcu_pb.AlarmMute,
        store[states.StateSegment.ALARM_MUTE]
    )
    prev_audible_alarms = False

    while True:
        simulated_log.input(log.LocalLogInputEvent(wall_time=time.time()))
        simulation_services.transform(time.time(), time.monotonic(), store)
        power_service.transform(store, simulated_log)
        alarms_services.transform(store, simulated_log)
        alarm_mute_service.transform(time.monotonic(), store, simulated_log)
        screen_status_service.transform(store)
        audible_alarms = (
            len(active_log_events.id) > 0 and not alarm_mute_.active
        )
        if audible_alarms and not prev_audible_alarms:
            logger.info('There are now audible alarms!')
        elif prev_audible_alarms and not audible_alarms:
            logger.info('There are no longer audible alarms!')
        prev_audible_alarms = audible_alarms
        service_event_log(
            simulated_log, active_log_events, simulated_log_receiver
        )
        await trio.sleep(simulators.SENSOR_UPDATE_INTERVAL / 1000)


async def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    # Set up logging
    application.configure_logging()
    logger = logging.getLogger('ventserver.simulator')

    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    serial_endpoint = None
    websocket_endpoint = websocket.Driver()
    fileio_endpoint = fileio.Handler()
    rotary_encoder_endpoint = \
        await application.open_rotary_encoder_endpoint(logger)

    # Server Receive Outputs
    channel: channels.TrioChannel[server.ReceiveOutputEvent] = \
        channels.TrioChannel()

    # Initialize states with defaults
    store = protocol.receive.backend.store
    application.initialize_hardcoded_states(store, INITIAL_VALUES, logger)
    await application.initialize_states_from_file(
        store, protocol, fileio_endpoint
    )

    # Initialize events log manager
    simulated_log = alarms.Manager(debouncers={
        mcu_pb.LogEventCode.fio2_too_low: debouncing.Debouncer(),
        mcu_pb.LogEventCode.fio2_too_high: debouncing.Debouncer(),
        mcu_pb.LogEventCode.flow_too_low: debouncing.Debouncer(),
        mcu_pb.LogEventCode.flow_too_high: debouncing.Debouncer(),
        mcu_pb.LogEventCode.spo2_too_low: debouncing.Debouncer(),
        mcu_pb.LogEventCode.spo2_too_high: debouncing.Debouncer(),
        mcu_pb.LogEventCode.hr_too_low: debouncing.Debouncer(),
        mcu_pb.LogEventCode.hr_too_high: debouncing.Debouncer(),
    })
    simulated_log_receiver = (
        protocol.receive.backend.  # pylint: disable=protected-access
        _event_log_receiver.  # pylint: disable=protected-access
        _log_events_receiver
    )

    # Initialize time
    protocol.receive.input(server.ReceiveDataEvent(
        wall_time=time.time(), monotonic_time=time.monotonic()
    ))
    simulated_log.input(log.LocalLogInputEvent(wall_time=time.time()))

    # Make server protocol think the MCU is connnected
    protocol.receive._connection_states.has_mcu = True

    await processes.make_dialog(
        'Running simulator backend. If this is running on a real ventilator, '
        'the software is not configured correctly and will not be able to '
        'talk to the hardware inside the ventilator!',
        'warning'
    )

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    functools.partial(
                        _trio.process_all, channel=channel,
                        push_endpoint=channel.push_endpoint
                    ),
                    protocol,
                    serial_endpoint, websocket_endpoint, rotary_encoder_endpoint
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
                    await application.handle_receive_outputs(
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
        application.filter_multierror(exc)
        logger.info('Finished, quitting!')


if __name__ == '__main__':
    better_exceptions.MAX_LENGTH = None
    better_exceptions.hook()
    trio.run(main)
