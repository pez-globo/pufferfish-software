"""Test the functionality of sansio.streams.BytearrayStream."""

from typing import Iterable, List, Optional

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.sansio import streams


def push_items(
        stream: streams.BytearrayStream, items: Iterable[bytes],
        initial_length: int = 0, maxlen: Optional[int] = None
) -> None:
    """Push items."""
    length = initial_length
    for item in items:
        stream.input(None)
        if maxlen is None or max(0, maxlen - length) >= len(item):
            length += len(item)
            stream.input(item)
        else:
            with pt.raises(IndexError):
                stream.input(item)
        stream.input(None)


@hp.given(
    precontents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4)
)
@hp.example(precontents=[], contents=[])
@hp.example(precontents=[b''], contents=[b'ab'])
def test_stream_push_unlimited(
        precontents: List[bytes], contents: List[bytes]
) -> None:
    """Test BytearrayStream behavior from an unlimited buffer."""
    all_precontents = b''.join(precontents)
    all_contents = b''.join(contents)

    buffer = bytearray(all_precontents)
    stream = streams.BytearrayStream(buffer=buffer, maxlen=None)
    push_items(stream, contents)
    assert buffer == all_precontents + all_contents


@hp.given(
    precontents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    maxlen=st.integers(min_value=0, max_value=20)
)
@hp.example(precontents=[], contents=[], maxlen=0)
@hp.example(precontents=[], contents=[b'ab'], maxlen=0)
@hp.example(precontents=[], contents=[b'ab'], maxlen=1)
@hp.example(precontents=[], contents=[b'ab', b'c'], maxlen=1)
@hp.example(precontents=[b'ab'], contents=[], maxlen=0)
@hp.example(precontents=[b'ab'], contents=[], maxlen=1)
@hp.example(precontents=[b'ab'], contents=[b'c'], maxlen=1)
@hp.example(precontents=[b'ab', b'c'], contents=[], maxlen=1)
@hp.example(precontents=[b'ab', b'c'], contents=[b'd', b''], maxlen=1)
@hp.example(precontents=[], contents=[b'ab', b'c'], maxlen=1)
def test_stream_push_maxlen(
        precontents: List[bytes], contents: List[bytes], maxlen: int
) -> None:
    """Test BytearrayChannel behavior with a max length."""
    all_precontents = b''.join(precontents)

    buffer = bytearray(b''.join(precontents))
    channel = streams.BytearrayStream(buffer=buffer, maxlen=maxlen)
    push_items(
        channel, contents, initial_length=len(all_precontents), maxlen=maxlen
    )


@hp.given(contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4))
@hp.example(contents=[])
@hp.example(contents=[b'ab'])
def test_stream_pull(contents: List[bytes]) -> None:
    """Test BytearrayStream behavior from a prepopulated buffer."""
    all_contents = b''.join(contents)

    buffer = bytearray(all_contents)
    stream = streams.BytearrayStream(buffer=buffer)
    results = [chunk for chunk in stream.output_all() if chunk]
    assert results == [bytes([byte]) for byte in all_contents]
    assert stream.output() is None


@hp.given(
    precontents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    maxlen=st.integers(min_value=0, max_value=10)
)
@hp.example(precontents=[], contents=[], maxlen=0)
@hp.example(precontents=[], contents=[b'ab'], maxlen=0)
@hp.example(precontents=[], contents=[b'ab'], maxlen=1)
@hp.example(precontents=[], contents=[b'ab', b'c'], maxlen=1)
@hp.example(precontents=[b'ab'], contents=[], maxlen=0)
@hp.example(precontents=[b'ab'], contents=[], maxlen=1)
@hp.example(precontents=[b'ab'], contents=[b'c'], maxlen=1)
@hp.example(precontents=[b'ab', b'c'], contents=[], maxlen=1)
@hp.example(precontents=[b'ab', b'c'], contents=[b'd', b''], maxlen=1)
@hp.example(precontents=[], contents=[b'ab', b'c'], maxlen=1)
@hp.example(precontents=[b'ab', b'c'], contents=[b'd', b''], maxlen=None)
def test_stream(
        precontents: List[bytes], contents: List[bytes], maxlen: Optional[int]
) -> None:
    """Test BytearrayStream behavior."""
    stream = streams.BytearrayStream(maxlen=maxlen)
    assert stream.output() is None
    # Push precontents
    push_items(stream, precontents, maxlen=maxlen)
    # Pull precontents
    results = [chunk for chunk in stream.output_all() if chunk]
    expected = bytearray()
    for chunk in precontents:
        if not chunk:
            continue
        if maxlen is None or len(expected) + len(chunk) <= maxlen:
            expected.extend(chunk)
    assert results == [bytes([byte]) for byte in expected]
    assert stream.output() is None
    # Push precontents and contents
    push_items(stream, precontents + contents, maxlen=maxlen)
    # Pull everything
    results = [chunk for chunk in stream.output_all() if chunk]
    for chunk in contents:
        if not chunk:
            continue
        if maxlen is None or len(expected) + len(chunk) <= maxlen:
            expected.extend(chunk)
    assert results == [bytes([byte]) for byte in expected]
