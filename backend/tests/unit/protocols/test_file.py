"""Tests for ventserver.protocols.file."""

import pytest as pt  # type: ignore
import betterproto

from ventserver.protocols import exceptions
from ventserver.protocols import file
from ventserver.protocols.protobuf import mcu_pb


examples_good = [
    (mcu_pb.Parameters,),
    (mcu_pb.ParametersRequest,),
    (mcu_pb.CycleMeasurements,),
    (mcu_pb.SensorMeasurements,),
    (mcu_pb.Alarms,)
]

examples_bad_crc = [
    (mcu_pb.Parameters,),
    (mcu_pb.ParametersRequest,),
    (mcu_pb.CycleMeasurements,),
    (mcu_pb.SensorMeasurements,),
    (mcu_pb.Alarms,)
]

examples_bad_data = [
    (mcu_pb.Parameters,),
    (mcu_pb.CycleMeasurements,),
    (mcu_pb.SensorMeasurements,),
]

# Saving File (Sending)

@pt.mark.parametrize('message,crc_message,state_type', examples_good)
def test_file_tx_state_data_valid(
        message: betterproto.Message,
        crc_message: bytes,
        state_type: str
)-> None:
    """"""
    sender = file.SendFilter()
    sender.input(message)
    output = sender.output()
    assert crc_message == output.data
    assert state_type == output.state_type


# Reading File (Receiving)

@pt.mark.parametrize('_,crc_message,state_type', examples_bad_crc)
def test_file_rx_crc_invalid(
        _: betterproto.Message,
        crc_message: bytes,
        state_type: str
) -> None:
    """"""
    receiver = file.ReceiveFilter()
    receiver.input(file.StateData(
        data=crc_message,
        state_type=state_type
    ))
    with pt.raises(exceptions.ProtocolDataError):
        __ = receiver.output()

@pt.mark.parametrize('_,crc_message,state_type', examples_bad_data)
def test_file_rx_data_invalid(
        _: betterproto.Message,
        crc_message: bytes,
        state_type: str
) -> None:
    """"""
    receiver = file.ReceiveFilter()
    receiver.input(file.StateData(
        data=crc_message,
        state_type=state_type
    ))
    with pt.raises(exceptions.ProtocolDataError):
        __ = receiver.output()

@pt.mark.parametrize('message,crc_message,state_type', examples_good)
def test_file_rx_data_valid(
        message: betterproto.Message,
        crc_message: bytes,
        state_type: str
) -> None:
    """"""
    receiver = file.ReceiveFilter()
    receiver.input(file.StateData(
        data=crc_message,
        state_type=state_type
    ))
    output_msg = receiver.output()
    assert output_msg == message


# Protocol

@pt.mark.parametrize('message,crc_message,state_type', examples_good)
def test_file_roundtrip(
        message: betterproto.Message,
        crc_message: bytes,
        state_type: str
) -> None:
    """"""
    sender = file.SendFilter()
    receiver = file.ReceiveFilter()
    sender.input(message)
    output_sd = sender.output()
    assert crc_message == output_sd.data
    assert state_type == output_sd.state_type
    receiver.input(output_sd)
    output_msg = receiver.output()
    assert output_msg == message


# def test_file_initialize_states() -> None: