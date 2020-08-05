"""Test the functionality of sansio.channels.DequeChannel."""

import collections
from typing import Iterable, List, Optional

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.sansio import channels


def push_items(
        channel: channels.DequeChannel[bytes], items: Iterable[bytes],
        initial_length: int = 0, maxlen: Optional[int] = None
) -> None:
    """Push items."""
    for (i, item) in enumerate(items):
        channel.input(None)
        if maxlen is None or initial_length + i < maxlen:
            channel.input(item)
        else:
            with pt.raises(IndexError):
                channel.input(item)
        channel.input(None)


@hp.given(
    precontents=st.lists(st.binary(max_size=2), min_size=0, max_size=4),
    contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4)
)
@hp.example(precontents=[], contents=[])
@hp.example(precontents=[b''], contents=[b'ab'])
def test_channel_push_unlimited(
        precontents: List[bytes], contents: List[bytes]
) -> None:
    """Test DequeChannel behavior from an unlimited buffer."""
    buffer = collections.deque(precontents)
    channel = channels.DequeChannel(buffer=buffer, maxlen=None)
    push_items(channel, contents)
    assert list(buffer) == precontents + contents


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
def test_channel_push_maxlen(
        precontents: List[bytes], contents: List[bytes], maxlen: int
) -> None:
    """Test DequeChannel behavior with a max length."""
    buffer = collections.deque(precontents)
    channel = channels.DequeChannel(buffer=buffer, maxlen=maxlen)
    push_items(
        channel, contents, initial_length=len(precontents), maxlen=maxlen
    )
    assert list(buffer) == (
        precontents + contents[:max(0, maxlen - len(precontents))]
    )


def pull_items(channel: channels.DequeChannel[bytes]) -> List[bytes]:
    """Pull all items."""
    results = []
    while True:
        output = channel.output()
        if output is None:
            break
        results.append(output)
    return results


@hp.given(contents=st.lists(st.binary(max_size=2), min_size=0, max_size=4))
@hp.example(contents=[])
@hp.example(contents=[b'ab'])
def test_channel_pull(contents: List[bytes]) -> None:
    """Test DequeChannel behavior from a prepopulated buffer."""
    buffer = collections.deque(contents)
    channel = channels.DequeChannel(buffer=buffer)
    results = pull_items(channel)
    assert results == contents
    assert channel.output() is None


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
def test_channel(
        precontents: List[bytes], contents: List[bytes], maxlen: Optional[int]
) -> None:
    """Test DequeChannel behavior."""
    channel: channels.DequeChannel[bytes] = channels.DequeChannel(
        maxlen=maxlen
    )
    assert channel.output() is None
    # Push precontents
    push_items(channel, precontents, maxlen=maxlen)
    # Pull precontents
    results = pull_items(channel)
    assert results == precontents[:maxlen]
    # Push precontents and contents
    push_items(channel, precontents, maxlen=maxlen)
    push_items(
        channel, contents, initial_length=(
            len(precontents) if maxlen is None
            else min(maxlen, len(precontents))
        ), maxlen=maxlen
    )
    # Pull everything
    results = pull_items(channel)
    assert results == (precontents + contents)[:maxlen]
