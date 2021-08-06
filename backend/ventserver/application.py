"""Trio I/O with sans-I/O protocol, running application."""

import logging
import functools
import random
import time
from typing import Any, Mapping, MutableMapping, Optional, Type

import better_exceptions  # type: ignore
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


INITIAL_VALUES = {
    # AlarmMute isn't actually stored on the filesystem, because any mute
    # should be cancelled upon any disconnection or shutdown. We provide it here
    # just to initialize the store on program startup.
    states.StateSegment.ALARM_MUTE: mcu_pb.AlarmMute(
        active=False, remaining=120
    ),
}


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


# Logging

class ConsoleColors():
    """ASCII color codes for the console."""
    RESET = "\x1b[0m"
    GREY = "\x1b[0;37m"
    GREEN = "\x1b[1;32m"
    YELLOW = "\x1b[1;33m"
    RED = "\x1b[1;31m"
    PURPLE = "\x1b[1;35m"
    BLUE = "\x1b[1;34m"
    LIGHT_BLUE = "\x1b[1;36m"
    BLINK_RED = "\x1b[5m\x1b[1;31m"


class CustomLogRecord(logging.LogRecord):
    """LogRecord with custom attributes."""

    def __init__(self, *args: Any, **kwargs: Any):
        """Initialize the LogRecord."""
        super().__init__(*args, **kwargs)
        self.origin = f'{self.name}:{self.lineno}'


class ConsoleLogFormatter(logging.Formatter):
    """Logging formatter to add colors."""

    ORIGIN_WIDTH = 35

    # Colors

    # Abbreviations
    ORIGIN_ABBREVIATIONS = {
        'ventserver': '(vs)',
        'integration': '(int)',
        'asynchronous': '(async)',
        '_serial': '(ser)',
        'rotaryencoder': '(rot)',
        'websocket': '(ws)',
        'processes': '(proc)',
        'protocols': '(proto)',
        'application': '(app)',
        'connections': '(conn)',
        'backend': '(be)',
        'devices': '(dev)',
        'frontend': '(fe)',
        'rotary_encoder': '(rot)',
        'protobuf': '(pb)',
        'transport': '(tp)',
        'crcelements': '(crc)',
        'datagrams': '(dg)',
        'messages': '(msg)',
        'simulation': '(sim)',
        'simulator': '(sim)',
        'SingleConnectionService': '(Conn)',
        'Synchronizers': '(Sync)',
        'Synchronizer': '(Sync)',
        'TimeoutHandler': '(Time)',
        'Receiver': '(RX)',
        'Sender': '(TX)',
    }

    # Format strings
    FORMAT_PREFIX = (
        ConsoleColors.PURPLE + '{asctime:<23}' + ConsoleColors.RESET
        + ' ' + ConsoleColors.BLUE + (
            '{origin:<' + str(ORIGIN_WIDTH) + '.' + str(ORIGIN_WIDTH) + '}'
        ) + ConsoleColors.RESET
        + ' '
    )
    FORMAT_SUFFIX = (
        '[{levelname:<1}] {message}'
    )
    FORMATS = {
        logging.DEBUG: (
            FORMAT_PREFIX + ConsoleColors.GREEN
            + FORMAT_SUFFIX + ConsoleColors.RESET
        ),
        logging.INFO: (
            FORMAT_PREFIX + ConsoleColors.GREY
            + FORMAT_SUFFIX + ConsoleColors.RESET
        ),
        logging.WARNING: (
            FORMAT_PREFIX + ConsoleColors.YELLOW
            + FORMAT_SUFFIX + ConsoleColors.RESET
        ),
        logging.ERROR: (
            FORMAT_PREFIX + ConsoleColors.RED
            + FORMAT_SUFFIX + ConsoleColors.RESET
        ),
        logging.CRITICAL: (
            FORMAT_PREFIX + ConsoleColors.BLINK_RED
            + FORMAT_SUFFIX + ConsoleColors.RESET
        ),
    }

    def format(self, record: CustomLogRecord) -> str:  # type: ignore
        # Color the record
        log_format = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_format, style='{')

        # Shorten the origin
        if record.levelno == logging.INFO:
            record.origin = record.name
        record.origin = self._abbreviate_origin(record.origin)
        if len(record.origin) > self.ORIGIN_WIDTH:
            record.origin = '*' + record.origin[-self.ORIGIN_WIDTH + 1:]

        # Improve the exception traceback
        if record.exc_info:
            record.exc_text = \
                ''.join(better_exceptions.format_exception(*record.exc_info))

        return formatter.format(record)

    def _abbreviate_origin(self, origin: str) -> str:
        """Abbreviate long words in the origin."""
        for (key, value) in self.ORIGIN_ABBREVIATIONS.items():
            origin = origin.replace(key, value)
        return origin


def configure_logging() -> None:
    """Set up the root logger."""
    logging.setLogRecordFactory(CustomLogRecord)
    root_logger = logging.getLogger()

    # Use shorter log level names
    logging.addLevelName(logging.CRITICAL, 'C')
    logging.addLevelName(logging.ERROR, 'E')
    logging.addLevelName(logging.WARNING, 'W')
    logging.addLevelName(logging.INFO, 'I')
    logging.addLevelName(logging.DEBUG, 'D')

    # Configure stdout output
    handler = logging.StreamHandler()
    handler.setFormatter(ConsoleLogFormatter())
    root_logger.addHandler(handler)
    root_logger.setLevel(logging.INFO)


# Initialization


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


def initialize_hardcoded_states(
        store: MutableMapping[
            states.StateSegment, Optional[betterproto.Message]
        ],
        initial_values: Mapping[
            states.StateSegment, Optional[betterproto.Message]
        ],
        logger: logging.Logger
) -> None:
    """Set initial values for the states."""
    for segment_type in store:
        if segment_type in initial_values:
            logger.info('Initializing hard-coded state: %s', segment_type)
            store[segment_type] = initial_values[segment_type]


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


# Event handling


async def handle_receive_outputs(
    receive_output: server.ReceiveOutputEvent,
    protocol: server.Protocol,
    serial_endpoint: Optional[_serial.Driver],
    websocket_endpoint: Optional[websocket.Driver],
    fileio_endpoint: fileio.Handler,
    nursery: trio.Nursery
) -> None:
    """Handle actions indicated by server protocol receive output events."""
    await _trio.process_protocol_send(
        receive_output.states_send, protocol,
        serial_endpoint, websocket_endpoint, fileio_endpoint
    )

    if receive_output.sysclock_setting is not None:
        nursery.start_soon(
            processes.set_system_time_and_rtc, receive_output.sysclock_setting
        )

    if receive_output.kill_frontend:
        nursery.start_soon(
            processes.kill_process, frontend.FrontendProps().process_name
        )


# Error handling


def filter_multierror(exc: trio.MultiError) -> None:
    """Filter out trio.MultiErrors from KeyboardInterrupts."""
    if len(exc.exceptions) != 2:
        raise exc
    if (
            not isinstance(exc.exceptions[0], KeyboardInterrupt) or
            not isinstance(exc.exceptions[1], trio.EndOfChannel)
    ):
        raise exc


# Main application


async def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    # Set up logging
    configure_logging()
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
    initialize_hardcoded_states(store, INITIAL_VALUES, logger)
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
    better_exceptions.MAX_LENGTH = 150
    better_exceptions.hook()
    trio.run(main)
