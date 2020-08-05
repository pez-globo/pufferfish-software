"""Test the functionality of protocols.datagrams classes."""

from typing import List, Tuple

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.protocols import datagrams
from ventserver.protocols import exceptions


example_datagrams_good = [
    (
        0xf16177d2, 0, 0, b'', b'\xf1\x61\x77\xd2\x00\x00'
    ),
    (
        0x98dbe355, 1, 5, b'\x01\x02\x03\x04\x05',
        b'\x98\xdb\xe3\x55\x01\x05\x01\x02\x03\x04\x05'
    )
]
example_datagrams_bad_crc = [
    (
        0x12345678, 0, 0, b'', b'\x12\x34\x56\x78\x00\x00'
    ),
    (
        0x00000000, 1, 5, b'\x01\x02\x03\x04\x05',
        b'\x00\x00\x00\x00\x01\x05\x01\x02\x03\x04\x05'
    )
]
example_datagrams_bad_length = [
    (
        0x5c1c2483, 0, 255, b'', b'\x5c\x1c\x24\x83\x00\xff'
    ),
    (
        0xc51ebf3b, 1, 0, b'\x01\x02\x03\x04\x05',
        b'\xc5\x1e\xbf\x3b\x01\x00\x01\x02\x03\x04\x05'
    )
]

example_datagrams = (
    example_datagrams_good + example_datagrams_bad_crc
    + example_datagrams_bad_length
)


# Datagrams


@hp.given(crc=st.integers(min_value=-1, max_value=0xffffffffff))
@hp.example(crc=0)
@hp.example(crc=-1)
@hp.example(crc=0xffffffff)
@hp.example(crc=0xffffffff + 1)
def test_datagram_init_crc(crc: int) -> None:
    """Test Datagram crc attr initialization."""
    if 0 <= crc <= 0xffffffff:
        datagrams.Datagram(crc=crc)
    else:
        with pt.raises(ValueError):
            datagrams.Datagram(crc=crc)


@hp.given(seq=st.integers(min_value=-1, max_value=0xffff))
@hp.example(seq=0)
@hp.example(seq=-1)
@hp.example(seq=0xff)
@hp.example(seq=0xff + 1)
def test_datagram_init_seq(seq: int) -> None:
    """Test Datagram seq attr initialization."""
    if 0 <= seq <= 0xff:
        datagrams.Datagram(seq=seq)
    else:
        with pt.raises(ValueError):
            datagrams.Datagram(seq=seq)


@hp.given(length=st.integers(min_value=-1, max_value=0xffff))
@hp.example(length=0)
@hp.example(length=-1)
@hp.example(length=0xff)
@hp.example(length=0xff + 1)
def test_datagram_init_length(length: int) -> None:
    """Test Datagram length attr initialization."""
    if 0 <= length <= 0xff:
        datagrams.Datagram(length=length)
    else:
        with pt.raises(ValueError):
            datagrams.Datagram(length=length)


@pt.mark.parametrize('crc,seq,length,payload,body', example_datagrams)
def test_datagram_parse(
        crc: int, seq: int, length: int, payload: bytes, body: bytes
) -> None:
    """Test Datagram parsing from body."""
    datagram = datagrams.Datagram()
    datagram.parse(body)
    assert datagram.crc == crc
    assert datagram.seq == seq
    assert datagram.length == length
    assert datagram.payload == payload


@hp.given(
    crc=st.binary(min_size=4, max_size=4),
    seq=st.integers(min_value=0, max_value=255),
    length=st.integers(min_value=0, max_value=255),
    payload=st.binary(max_size=255)
)
@hp.example(crc=b'\x12\x34\x56\x78', seq=0, length=0, payload=b'')
@hp.example(
    crc=b'\x12\x34\x56\x78', seq=255, length=255,
    payload=bytes(list(range(255)))
)
def test_datagram_parse_invariants(
        crc: bytes, seq: int, length: int, payload: bytes
) -> None:
    """Test Datagram parse invariants."""
    datagram = datagrams.Datagram()
    datagram.parse(crc + bytes([seq, length]) + payload)
    assert datagram.crc // 256 // 256 // 256 == crc[0]
    assert datagram.crc // 256 // 256 % 256 == crc[1]
    assert datagram.crc // 256 % 256 == crc[2]
    assert datagram.crc % 256 == crc[3]
    assert datagram.seq == seq
    assert datagram.length == length
    assert datagram.payload == payload


@hp.given(body=st.binary(max_size=5))
def test_datagram_parse_invalid(body: bytes) -> None:
    """Test Datagram parse errors."""
    datagram = datagrams.Datagram()
    with pt.raises(exceptions.ProtocolDataError):
        datagram.parse(body)


@pt.mark.parametrize('crc,seq,length,payload,body', example_datagrams)
def test_datagram_generate(
        crc: int, seq: int, length: int, payload: bytes, body: bytes
) -> None:
    """Test Datagram generation of body."""
    datagram = datagrams.Datagram()
    datagram.crc = crc
    datagram.seq = seq
    datagram.length = length
    datagram.payload = payload
    assert datagram.compute_body() == body


@pt.mark.parametrize('crc,seq,length,payload,body', example_datagrams_good)
def test_datagram_update(
        crc: int, seq: int, length: int, payload: bytes, body: bytes
) -> None:
    """Test Datagram updating from payload."""
    datagram = datagrams.Datagram()
    datagram.seq = seq
    datagram.payload = payload
    datagram.update_from_payload()
    assert datagram.crc == crc
    assert datagram.length == length
    assert datagram.compute_body() == body


@pt.mark.parametrize('crc,seq,length,payload,body', example_datagrams)
def test_datagram_roundtrip(
        crc: int, seq: int, length: int, payload: bytes, body: bytes
) -> None:
    """Test Datagram parse/generate roundtrip."""
    generate_datagram = datagrams.Datagram()
    generate_datagram.crc = crc
    generate_datagram.seq = seq
    generate_datagram.length = length
    generate_datagram.payload = payload
    generate_datagram.parse(generate_datagram.compute_body())
    assert generate_datagram.crc == crc
    assert generate_datagram.seq == seq
    assert generate_datagram.length == length
    assert generate_datagram.payload == payload
    parse_datagram = datagrams.Datagram()
    parse_datagram.parse(generate_datagram.compute_body())
    assert parse_datagram.crc == crc
    assert parse_datagram.seq == seq
    assert parse_datagram.length == length
    assert parse_datagram.payload == payload
    assert parse_datagram.compute_body() == body


# Datagram Receiving


@pt.mark.parametrize('_,__,___,payload,body', example_datagrams_good)
def test_datagram_rx_indiv(
        _: int, __: int, ___: int, payload: bytes, body: bytes
) -> None:
    """Test DatagramReceiver behavior with specific individual examples."""
    receiver = datagrams.DatagramReceiver()
    assert receiver.output() is None
    receiver.input(body)
    assert receiver.output() == payload


def input_body_seq(
        receiver: datagrams.DatagramReceiver,
        datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Add the sequence of datagram bodies as input to a receiver."""
    for (_, __, ___, ____, body) in datagram_sequence:
        receiver.input(body)


def output_body_seq(
        receiver: datagrams.DatagramReceiver,
        datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Get the sequence of datagram payloads as output from a receiver."""
    for (_, __, ___, payload, ____) in datagram_sequence:
        assert receiver.output() == payload


@hp.given(datagram_sequence=st.lists(
    st.sampled_from(example_datagrams_good), min_size=1, max_size=16
))
def test_datagram_rx_seq(
        datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Test DatagramReceiver sequential behavior."""
    receiver = datagrams.DatagramReceiver()
    assert receiver.output() is None
    input_body_seq(receiver, datagram_sequence)
    output_body_seq(receiver, datagram_sequence)
    assert receiver.output() is None
    input_body_seq(receiver, datagram_sequence)
    output_body_seq(receiver, datagram_sequence)
    assert receiver.output() is None


@pt.mark.parametrize(
    '_,__,___,____,body',
    example_datagrams_bad_crc + example_datagrams_bad_length
)
def test_datagram_rx_invalid_indiv(
        _: int, __: int, ___: int, ____: bytes, body: bytes
) -> None:
    """Test DatagramReceiver with individual invalid inputs."""
    receiver = datagrams.DatagramReceiver()
    receiver.input(body)
    with pt.raises(exceptions.ProtocolDataError):
        receiver.output()


@pt.mark.parametrize(
    '_,__,___,____,body',
    example_datagrams_bad_crc + example_datagrams_bad_length
)
def test_datagram_rx_invalid_seq(
        _: int, __: int, ___: int, ____: bytes, body: bytes
) -> None:
    """Test DatagramReceiver with specific invalid inputs in valid sequence."""
    receiver = datagrams.DatagramReceiver()
    input_body_seq(receiver, example_datagrams_good)
    receiver.input(body)
    input_body_seq(receiver, example_datagrams_good)
    output_body_seq(receiver, example_datagrams_good)
    with pt.raises(exceptions.ProtocolDataError):
        receiver.output()
    output_body_seq(receiver, example_datagrams_good)


# Datagram Sending


def input_payload_seq(
        sender: datagrams.DatagramSender,
        datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Add the sequence of datagram bodies as input to a receiver."""
    for (_, __, ___, payload, ____) in datagram_sequence:
        sender.input(payload)


def output_payload_seq(
        sender: datagrams.DatagramSender,
        datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Get the sequence of datagram payloads as output from a receiver."""
    for (_, __, length, payload, ___) in datagram_sequence:
        output = sender.output()
        assert output is not None
        assert output[5] == length
        assert output[6:] == payload


def test_datagram_tx_seq() -> None:
    """Test DatagramSender sequential behavior."""
    sender = datagrams.DatagramSender()
    assert sender.output() is None
    counter = 0
    for (_, __, ___, payload, body) in example_datagrams_good:
        sender.input(payload)
        assert sender.output() == body
        counter += 1
    assert sender.output() is None
    for (_, __, length, payload, body) in example_datagrams_good:
        sender.input(payload)
        output = sender.output()
        assert output is not None
        assert output[4] == counter
        assert output[5] == length
        assert output[6:] == payload
        counter += 1
    assert sender.output() is None


@hp.given(length=st.integers(min_value=256, max_value=260))
@hp.example(length=256)
def test_datagram_tx_invalid_indiv(length: int) -> None:
    """Test DatagramSender with specific individual invalid inputs."""
    sender = datagrams.DatagramSender()
    sender.input(bytes([0x00 for i in range(length)]))
    with pt.raises(exceptions.ProtocolDataError):
        sender.output()


@hp.given(
    length=st.integers(min_value=256, max_value=260),
    datagram_sequence=st.lists(
        st.sampled_from(example_datagrams_good), min_size=1, max_size=16
    )
)
@hp.example(length=256, datagram_sequence=example_datagrams_good)
def test_datagram_tx_invalid_seq(
        length: int, datagram_sequence: List[Tuple[int, int, int, bytes, bytes]]
) -> None:
    """Test DatagramSender with specific invalid inputs in a valid sequence."""
    sender = datagrams.DatagramSender()
    input_payload_seq(sender, datagram_sequence)
    sender.input(bytes([0x00 for i in range(length)]))
    input_payload_seq(sender, datagram_sequence)
    output_payload_seq(sender, datagram_sequence)
    with pt.raises(exceptions.ProtocolDataError):
        sender.output()
    output_payload_seq(sender, datagram_sequence)
