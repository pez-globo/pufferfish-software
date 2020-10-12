"""
Story: CRC based hashing filter that provides data
    integrity with various I/Os.
"""

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.protocols import crcelements
from ventserver.protocols import exceptions

example_crc_good = [
    (
        b'\xf1aw\xd2', b'\x00\x00', b'\xf1\x61\x77\xd2\x00\x00'
    ),
    (
        b'\x98\xdb\xe3U', b'\x01\x05\x01\x02\x03\x04\x05',
        b'\x98\xdb\xe3\x55\x01\x05\x01\x02\x03\x04\x05'
    )
]
example_crc_bad = [
    (
        b'\x124Vx', b'\x03\x04\x00\xed\x30\x00',
        b'\x12\x34\x56\x78\x03\x04\x00\xed\x30\x00'
    ),
    (
        b'\x03\x12#E', b'\x01\x05\x09\x08\x0a\x02\x0a',
        b'\x03\x12\x23\x45\x01\x05\x09\x08\x0a\x02\x0a'
    )
]
example_crc = example_crc_good + example_crc_bad

# CRCElement

@hp.given(crc=st.binary(min_size=0x00000000, max_size=0xffffffffff))
@hp.example(crc=b'\x00\x00\x00\x00')
@hp.example(crc=b'\xff\xff\xff\xff')
@hp.example(crc=b'\xff\xff\xff\xff' + b'\x00\x00\x00\x01')
def test_crcelement_init_crc(crc: bytes) -> None:
    """
    Scenario: CRC of data is generated/parsed from the input.
    Given: We have CRCElement object.
    And: The CRC generated/parsed is in bytes.
    When: The CRC is between \\x00\\x00\\x00\\x00 and \\xff\\xff\\xff\\xff'
    And: It's length is 4.
    Then: ValueError should not be raised.
    """
    if (b'\x00\x00\x00\x00' <= crc <= b'\xff\xff\xff\xff') and (len(crc) == 4):
        crcelements.CRCElement(crc=crc)
    else:
        with pt.raises(ValueError):
            crcelements.CRCElement(crc=crc)


@pt.mark.parametrize('crc,payload,_', example_crc_good)
def test_crcelement_integrity_pass(
        crc: bytes, payload: bytes, _: bytes
) -> None:
    """
    Scenario: The system reads file/processes message from mcu/frontend.
        The message has been parsed 
    Given: We have CRCElement object.
    And: The parsed CRC and payload, in bytes.
    When: The parsed CRC matches the computed CRC
    Then: ProtocolDataError should not be raised.
    """
    crc_element = crcelements.CRCElement()
    crc_element.crc = crc
    crc_element.payload = payload
    try:
        crc_element.check_integrity()
    except exceptions.ProtocolDataError as err:
        pt.fail("Integrity check failed: {0}".format(err))


@pt.mark.parametrize('crc,payload,body', example_crc)
def test_crcelement_parse(
        crc: bytes, payload: bytes, body: bytes
) -> None:
    """
    Scenario: The system reads file/processes message from mcu/frontend.
    Given: A protobuf message in bytes.
    And: A CRC element.
    When: The protobuf message has correct size
    Then: We should get seperated CRC key and message body.
    """
    crc_element = crcelements.CRCElement()
    crc_element.parse(body)
    assert crc_element.crc == crc
    assert crc_element.payload == payload


@pt.mark.parametrize('_,__,body', example_crc_bad)
def test_crcelements_rx_invalid_crc(
        _: bytes, __: bytes, body: bytes
) -> None:
    """
    Scenario: The system reads corrupted file or
        processes corrupted message.
    Given: A protobuf message in bytes.
    And: A CRC message receiver.
    When: The message bytes are corrupted
    Then: Raise ProtocolDataError for that message.
    """
    receiver = crcelements.CRCReceiver()
    receiver.input(body)
    with pt.raises(exceptions.ProtocolDataError):
        ___ = receiver.output()
