"""Trio I/O with sans-I/O protocol, running application."""

import logging
from typing import Type, List, Optional, Dict

import trio
import betterproto

from ventserver.integration import _trio
from ventserver.io.trio import _serial
from ventserver.io.trio import channels
from ventserver.io.trio import websocket
from ventserver.io.trio import fileio
from ventserver.protocols import server
from ventserver.protocols import file
from ventserver.protocols import exceptions
from ventserver.protocols.protobuf import mcu_pb 


logger = logging.getLogger()
handler = logging.StreamHandler()
formatter = logging.Formatter(
    '%(asctime)s %(name)-12s %(levelname)-8s %(message)s'
)
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.setLevel(logging.INFO)

async def initialize_states(
        states: List[Type[betterproto.Message]],
        protocol: server.Protocol,
        filehandler: fileio.Handler
) -> Dict[
    Type[betterproto.Message], Optional[betterproto.Message]
]:
    """Initialize state values from state store or default values."""
    default_init = list()
    for state in states:
        try: # Handle fileio errors
            filehandler.set_props(state.__name__, "rb")
            await filehandler.open()
            message = await filehandler.read()
            protocol.receive.file.input(
                file.StateData(state_type=state.__name__, data=message)
                )
        except OSError as err:
            print(err)
        finally:
            await filehandler.close()
    
    all_states = protocol.receive.backend.all_states
    while True:
        try: # Handles data integrity and protocol error
            event = protocol.receive.file.output()
            if not event:
                break

            all_states[type(event)] = event
        except exceptions.ProtocolDataError as err:
            print(err)

    for state in states:
        if not all_states[state]:
            default_init.append(state)  
    
    for state in default_init:
        if state is mcu_pb.ParametersRequest:
            all_states[mcu_pb.ParametersRequest] = mcu_pb.ParametersRequest(
                mode=mcu_pb.VentilationMode.hfnc, rr=30, fio2=60, flow=6
            )
        else:
            all_states[state] = state()
    
    return all_states

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
    all_states = protocol.receive.backend.all_states
    states = [
        mcu_pb.Parameters, mcu_pb.CycleMeasurements,
        mcu_pb.SensorMeasurements, mcu_pb.ParametersRequest
    ]
    all_states = await initialize_states(
        states, protocol, filehandler
    )

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    # mypy only supports <= 4 args with trio-typing
                    _trio.process_all, serial_endpoint,
                    protocol, websocket_endpoint, channel, channel.push_endpoint
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
