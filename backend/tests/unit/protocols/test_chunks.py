"""Test the functionality of protocols.frames chunking classes."""

from typing import List, Optional

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.protocols import frames


# Splitting


@hp.given(delimiter=st.binary(max_size=4))
@hp.example(delimiter=b'')
@hp.example(delimiter=b'\0')
@hp.example(delimiter=b'\xff')
@hp.example(delimiter=b'\0\0')
@hp.example(delimiter=b'abcd')
def test_chunk_splitting_init(delimiter: bytes) -> None:
    """Test ChunkSplitter attr initialization."""
    if len(delimiter) == 1:
        frames.ChunkSplitter(delimiter=delimiter)
    else:
        with pt.raises(ValueError):
            frames.ChunkSplitter(delimiter=delimiter)


@hp.given(inputs=st.binary(max_size=32))
@hp.example(inputs=b'')
@hp.example(inputs=b'\0')
@hp.example(inputs=b'\0\0')
@hp.example(inputs=b'a\0b\0')
@hp.example(inputs=b'a\0b\0c')
def test_chunk_splitting_individual(inputs: bytes) -> None:
    """Test ChunkSplitter behavior on an individual input."""
    outputs = inputs.split(b'\0')

    splitter = frames.ChunkSplitter()
    assert splitter.output() is None
    splitter.input(inputs)
    for (i, chunk) in enumerate(splitter.output_all()):
        assert chunk == outputs[i]
    assert splitter.output() is None


@hp.given(input_sequence=st.lists(st.binary(max_size=32), max_size=8))
@hp.example(input_sequence=[b'', b'\0', b'a\0b', b'', b'\0'])
def test_chunk_splitting_seq(input_sequence: List[bytes]) -> None:
    """Test ChunkSplitter behavior on a sequence of inputs."""
    outputs = b''.join(input_sequence).split(b'\0')

    splitter = frames.ChunkSplitter()
    assert splitter.output() is None
    for inputs in input_sequence:
        splitter.input(inputs)
    for (i, chunk) in enumerate(splitter.output_all()):
        assert chunk == outputs[i]
    assert splitter.output() is None


# Merging


@hp.given(
    input_sequence=st.lists(
        st.one_of(st.none(), st.binary(max_size=4)), min_size=0, max_size=8
    )
)
@hp.example(input_sequence=[])
@hp.example(input_sequence=[None])
@hp.example(input_sequence=[b''])
@hp.example(input_sequence=[b'\0'])
@hp.example(input_sequence=[b'a\0'])
@hp.example(input_sequence=[b'a\0b'])
@hp.example(input_sequence=[b'a', b'b'])
@hp.example(input_sequence=[b'a', b'b', b'c'])
@hp.example(input_sequence=[b'a\0b', b'c'])
def test_chunk_merging(input_sequence: List[Optional[bytes]]) -> None:
    """Test ChunkMerger behavior."""
    all_inputs = [input for input in input_sequence if input is not None]
    all_output = b'\0'.join(input for input in all_inputs)
    if all_inputs:
        all_output += b'\0'

    merger = frames.ChunkMerger()
    assert merger.output() is None
    for chunk in input_sequence:
        merger.input(chunk)
    result = b''.join(output for output in merger.output_all())
    assert result == all_output
