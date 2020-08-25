"""Test the functionality of protocols.frames COBS encoding/decoding classes."""

from typing import Dict, List, Tuple

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.protocols import exceptions
from ventserver.protocols import frames


example_encodings = {
    # From Cheshire & Baker 1999
    b'': b'\x01',
    b'x': b'\x02x',
    b'xy': b'\x03xy',
    b'Hello World': b'\x0cHello World',
    b'\x45\x00\x00\x2c\x4c\x79\x00\x00\x40\x06\x4f\x37': (
        b'\x02\x45\x01\x04\x2c\x4c\x79\x01\x05\x40\x06\x4f\x37'
    ),
    # From Wikipedia
    b'\x00': b'\x01\x01',
    b'\x00\x00': b'\x01\x01\x01',
    b'\x11\x22\x00\x33': b'\x03\x11\x22\x02\x33',
    b'\x11\x22\x33\x44': b'\x05\x11\x22\x33\x44',
    bytes(list(range(1, 0xFF))): b'\xff' + bytes(list(range(1, 0xFF)))
}

example_decodings = {
    encoded: unencoded for (unencoded, encoded) in example_encodings.items()
}


# Decoding


@pt.mark.parametrize('data,expected', list(example_decodings.items()))
def test_cobs_dec_indiv(data: bytes, expected: bytes) -> None:
    """Test COBSDecoder behavior with specific individual examples."""
    decoder = frames.COBSDecoder()
    assert decoder.output() is None
    decoder.input(data)
    assert expected == decoder.output()
    assert decoder.output() is None


def input_decoding_seq(
        decoder: frames.COBSDecoder,
        decoding_sequence: List[Tuple[bytes, bytes]]
) -> None:
    """Add the sequence of encoded data as input to a decoder."""
    (all_data, _) = zip(*decoding_sequence)
    for data in all_data:
        decoder.input(data)


def output_decode_seq(
        decoder: frames.COBSDecoder,
        decoding_sequence: List[Tuple[bytes, bytes]]
) -> None:
    """Get the sequence of decoded data as output from a decoder."""
    (_, all_expected) = zip(*decoding_sequence)
    for expected in all_expected:
        assert expected == decoder.output()


@hp.given(decodings=st.lists(
    st.sampled_from(list(example_decodings.items())), min_size=1, max_size=16
))
def test_cobs_dec_seq(decodings: List[Tuple[bytes, bytes]]) -> None:
    """Test COBSDecoder behavior with a specific sequence of examples."""
    decoder = frames.COBSDecoder()
    input_decoding_seq(decoder, decodings)
    output_decode_seq(decoder, decodings)
    assert decoder.output() is None


@pt.mark.parametrize('invalid_encoded', [
    b'\x02',
])
def test_cobs_dec_errors_indiv(invalid_encoded: bytes) -> None:
    """Test COBSDecoder with specific individual invalid inputs."""
    decoder = frames.COBSDecoder()
    decoder.input(invalid_encoded)
    with pt.raises(exceptions.ProtocolDataError):
        decoder.output()


@pt.mark.parametrize('invalid_encoded,valid_decodings', [
    (b'\x02', example_decodings),
])
def test_cobs_dec_errors_seq(
        invalid_encoded: bytes, valid_decodings: Dict[bytes, bytes]
) -> None:
    """Test COBSDecoder with specific invalid inputs in a valid sequence."""
    decoder = frames.COBSDecoder()
    input_decoding_seq(decoder, list(valid_decodings.items()))
    decoder.input(invalid_encoded)
    input_decoding_seq(decoder, list(valid_decodings.items()))
    output_decode_seq(decoder, list(valid_decodings.items()))
    with pt.raises(exceptions.ProtocolDataError):
        decoder.output()
    output_decode_seq(decoder, list(valid_decodings.items()))


# Encoding


@pt.mark.parametrize('data,expected', list(example_encodings.items()))
def test_cobs_enc_indiv(data: bytes, expected: bytes) -> None:
    """Test COBSEncoder behavior with specific individual examples."""
    encoder = frames.COBSEncoder()
    assert encoder.output() is None
    encoder.input(data)
    assert expected == encoder.output()


def input_encoding_seq(
        encoder: frames.COBSEncoder,
        encoding_sequence: List[Tuple[bytes, bytes]]
) -> None:
    """Add the sequence of encoded data as input to an encoder."""
    (all_data, _) = zip(*encoding_sequence)
    for data in all_data:
        encoder.input(data)


def output_encoding_seq(
        encoder: frames.COBSEncoder,
        encoding_sequence: List[Tuple[bytes, bytes]]
) -> None:
    """Get the sequence of encoded data as output from an encoder."""
    (_, all_expected) = zip(*encoding_sequence)
    for expected in all_expected:
        assert expected == encoder.output()


@hp.given(encodings=st.lists(
    st.sampled_from(list(example_encodings.items())), min_size=1, max_size=16
))
def test_cobs_enc_encodings(encodings: List[Tuple[bytes, bytes]]) -> None:
    """Test COBSEncoder behavior with a specific sequence of examples."""
    encoder = frames.COBSEncoder()
    input_encoding_seq(encoder, encodings)
    output_encoding_seq(encoder, encodings)
    assert encoder.output() is None


@hp.given(invalid_length=st.integers(min_value=255, max_value=260))
@hp.example(invalid_length=255)
def test_cobs_enc_invalid_length(invalid_length: int) -> None:
    """Test COBSEncoder with invalid individual input lengths."""
    encoder = frames.COBSEncoder()
    encoder.input(bytes([0x00 for i in range(invalid_length)]))
    with pt.raises(exceptions.ProtocolDataError):
        encoder.output()


@hp.given(
    invalid_length=st.integers(min_value=255, max_value=260),
    valid_encodings=st.lists(
        st.sampled_from(list(example_encodings.items())),
        min_size=1, max_size=16
    )
)
def test_cobs_enc_errors_seq(
        invalid_length: int, valid_encodings: List[Tuple[bytes, bytes]]
) -> None:
    """Test COBSEncoder with invalid input lengths in a valid input sequence."""
    encoder = frames.COBSEncoder()
    input_encoding_seq(encoder, list(valid_encodings))
    encoder.input(bytes([0x00 for i in range(invalid_length)]))
    input_encoding_seq(encoder, list(valid_encodings))
    output_encoding_seq(encoder, list(valid_encodings))
    with pt.raises(exceptions.ProtocolDataError):
        encoder.output()
    output_encoding_seq(encoder, list(valid_encodings))


@hp.given(unencoded=st.binary(max_size=254))
@hp.example(unencoded=b'')
@hp.example(unencoded=bytes(list(range(254))))
def test_cobs_enc_invariants(unencoded: bytes) -> None:
    """Test COBSEncoder invariants."""
    encoder = frames.COBSEncoder()
    encoder.input(unencoded)
    output = encoder.output()
    assert output is not None
    assert b'\x00' not in output


# Round-trip


@hp.given(unencoded=st.binary(max_size=254))
@hp.example(unencoded=b'')
@hp.example(unencoded=bytes(list(range(254))))
def test_cobs_roundtrip_indiv(unencoded: bytes) -> None:
    """Test COBSEncoder+COBSDecoder round-tripping with individual inputs."""
    encoder = frames.COBSEncoder()
    decoder = frames.COBSDecoder()
    encoder.input(unencoded)
    decoder.input(encoder.output())
    assert decoder.output() == unencoded


@hp.given(unencoded_sequence=st.lists(st.binary(max_size=254), max_size=16))
def test_cobs_roundtrip_seq(unencoded_sequence: List[bytes]) -> None:
    """Test COBSEncoder+COBSDecoder round-tripping with input sequences."""
    encoder = frames.COBSEncoder()
    decoder = frames.COBSDecoder()
    for unencoded in unencoded_sequence:
        encoder.input(unencoded)
    for output in encoder.output_all():
        decoder.input(output)
    for unencoded in unencoded_sequence:
        assert decoder.output() == unencoded
    assert decoder.output() is None
