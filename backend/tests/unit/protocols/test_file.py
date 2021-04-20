"""Tests for ventserver.protocols.file."""

import pytest as pt  # type: ignore
import betterproto

from ventserver.protocols import exceptions
from ventserver.protocols import file
from ventserver.protocols.protobuf import mcu_pb


# examples_good = [
#     (
#         mcu_pb.Parameters(
#             time=1601382363357, mode=mcu_pb.VentilationMode(6), pip=6,
#             peep=30,  # 0-50
#             vt=200,  # 100-4000/20-300
#             rr=30,  # 4-150
#             ie=2, fio2=60,  # 0-100
#             flow=34
#         ),
#         b'U\xa3\x13\xae\x04\x08\xdd\xe1\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
#         b'\xc0@%\x00\x00\xf0A-\x00\x00HC5\x00\x00\xf0A=\x00\x00\x00@E\x00' +
#         b'\x00pBM\x00\x00\x08B', 'Parameters'
#     ),
#     (
#         mcu_pb.ParametersRequest(
#             time=1601382365840, mode=mcu_pb.VentilationMode(6), pip=7,
#             peep=20, vt=300, rr=80, ie=3, fio2=50, flow=20
#         ),
#         b'\xff\r\xca<\x05\x08\x90\xf5\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
#         b'\xe0@%\x00\x00\xa0A-\x00\x00\x96C5\x00\x00\xa0B=\x00\x00@@E' +
#         b'\x00\x00HBM\x00\x00\xa0A', 'ParametersRequest'
#     ),
#     (
#         mcu_pb.CycleMeasurements(
#             time=1601382370441, vt=250, rr=25,
#             peep=15, pip=10, ip=6, ve=15
#         ),
#         b'\xacO\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x15\x00\x00zC\x1d' +
#         b'\x00\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
#         'CycleMeasurements'
#     ),
#     (
#         mcu_pb.SensorMeasurements(
#             time=1601382670441, cycle=10, paw=20,
#             flow=25, volume=50, fio2=50, spo2=97  # 90-100
#         ),
#         b'%W\xb29\x02\x08\xe9\xc0\xe1\xce\xcd.\x10\n\x1d\x00\x00\xa0A%' +
#         b'\x00\x00\xc8A-\x00\x00HB5\x00\x00HB=\x00\x00\xc2B',
#         'SensorMeasurements'
#     )
# ]

# examples_bad_crc = [
#     (
#         mcu_pb.ParametersRequest(
#             time=1601382365840, mode=mcu_pb.VentilationMode(6), pip=7,
#             peep=20, vt=300, rr=80, ie=0.55, fio2=50, flow=20
#         ),
#         b'*Z\x1e|\x05\x08\x90\xf5\xce\xce\xcd.\x10\x06\x14\x00\x00\xe0@%' +
#         b'\x00\x00\xa0A-\x00\x00\x96C5\x00\x00\xa0B=\xcd\xcc\x0c?E\x00' +
#         b'\x00HBM\x00\x00\xa0A', 'ParametersRequest'
#     ),
#     (
#         mcu_pb.Parameters(
#             time=1601382363357, mode=mcu_pb.VentilationMode(6), pip=6,
#             peep=30, vt=200, rr=30, ie=0.6, fio2=60, flow=34
#         ),
#         b'C\x82\xfc,\x04\x08\xdd\xe1\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
#         b'\xc0@%\xf0A-\x00\x00HC5\x00\x00\xf0A=\x9a\x99\x19?E\x00' +
#         b'\x00pBM\x00\x00\x08B', 'Parameters'
#     ),
#     (
#         mcu_pb.CycleMeasurements(
#             time=1601382370441, vt=250, rr=25,
#             peep=15, pip=10, ip=6, ve=15
#         ),
#         b'\xac1\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x15\x00\x00zC\x1d' +
#         b'\x00\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
#         'CycleMeasurements'
#     )
# ]

examples_bad_data = [
    (
        mcu_pb.Parameters(
            time=1601382363357, mode=mcu_pb.VentilationMode(6), pip=6,
            peep=30, vt=200, rr=30, ie=0.6, fio2=60, flow=34
        ),
        b'\xacO\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x15\x00\x00zC\x1d\x00' +
        b'\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
        'Parameters'
    ),
    (
        mcu_pb.CycleMeasurements(
            time=1601382370441, vt=250, rr=25,
            peep=15, pip=10, ip=6, ve=15
        ),
        b'%W\xb29\x02\x08\xe9\xc0\xe1\xce\xcd.\x10\n\x1d\x00\x00\xa0A%\x00' +
        b'\x00\xc8A-\x00\x00HB5\x00\x00HB=\x00\x00\xc2B', 'CycleMeasurements'
    ),
    (
        mcu_pb.SensorMeasurements(
            time=1601382670441, cycle=10, paw=20,
            flow=25, volume=50, fio2=50, spo2=97  # 90-100
        ),
        b'C\x82\xfc,\x04\x08\xdd\xe1\xce\xce\xcd.\x10\x06\x1d\x00\x00\xc0@%' +
        b'\x00\x00\xf0A-\x00\x00HC5\x00\x00\xf0A=\x9a\x99\x19?E\x00\x00pBM' +
        b'\x00\x00\x08B', 'SensorMeasurements'
    )
]


# Saving File (Sending)

# @pt.mark.parametrize('message,crc_message,state_type', examples_good)
# def test_file_tx_state_data_valid(
#         message: betterproto.Message,
#         crc_message: bytes,
#         state_type: str
# ) -> None:
#     """"""
#     sender = file.SendFilter()
#     sender.input(message)
#     output = sender.output()
#     assert output is not None
#     assert crc_message == output.data
#     assert state_type == output.state_type


# Reading File (Receiving)

# @pt.mark.parametrize('_,crc_message,state_type', examples_bad_crc)
# def test_file_rx_crc_invalid(
#         _: betterproto.Message,
#         crc_message: bytes,
#         state_type: str
# ) -> None:
#     """"""
#     receiver = file.ReceiveFilter()
#     receiver.input(file.StateData(
#         data=crc_message,
#         state_type=state_type
#     ))
#     with pt.raises(exceptions.ProtocolDataError):
#         receiver.output()


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
        receiver.output()


# @pt.mark.parametrize('message,crc_message,state_type', examples_good)
# def test_file_rx_data_valid(
#         message: betterproto.Message,
#         crc_message: bytes,
#         state_type: str
# ) -> None:
#     """"""
#     receiver = file.ReceiveFilter()
#     receiver.input(file.StateData(
#         data=crc_message,
#         state_type=state_type
#     ))
#     output_msg = receiver.output()
#     assert output_msg == message


# Protocol

# @pt.mark.parametrize('message,crc_message,state_type', examples_good)
# def test_file_roundtrip(
#         message: betterproto.Message,
#         crc_message: bytes,
#         state_type: str
# ) -> None:
#     """"""
#     sender = file.SendFilter()
#     receiver = file.ReceiveFilter()
#     sender.input(message)
#     output_sd = sender.output()
#     assert output_sd is not None
#     assert crc_message == output_sd.data
#     assert state_type == output_sd.state_type
#     receiver.input(output_sd)
#     output_msg = receiver.output()
#     assert output_msg == message


# TODO: add test for initialize_states() from integration._trio.py
