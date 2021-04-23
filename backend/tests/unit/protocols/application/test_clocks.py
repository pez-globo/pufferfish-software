"""Test the functionality of protocols.application.clocks classes."""

import hypothesis as hp
import hypothesis.strategies as st

from ventserver.protocols.application import clocks


@hp.given(
    local_sync_time=st.floats(
        min_value=0,  # s
        max_value=(3 * 10 ** 10),  # ~1000 years in s
        allow_nan=False, allow_infinity=False
    ),
    remote_sync_time=st.integers(
        min_value=-(3 * 10 ** 13),  # ~-1000 years in ms
        max_value=(3 * 10 ** 13)  # ~1000 years in ms
    ),
    time_delta=st.integers(
        min_value=-(3 * 10 ** 13),  # ~-1000 years in ms
        max_value=(3 * 10 ** 13)  # ~1000 years in ms
    )
)
@hp.example(local_sync_time=1, remote_sync_time=1000, time_delta=0)
@hp.example(local_sync_time=123.45, remote_sync_time=0, time_delta=-1)
@hp.example(local_sync_time=42.9, remote_sync_time=42, time_delta=1)
def test_normal_offsets(
        local_sync_time: float, remote_sync_time: int, time_delta: int
) -> None:
    """Test clock synchronization offsets in normal conditions."""
    synchronizer = clocks.ClockSynchronizer()
    synchronizer.input(clocks.UpdateEvent(
        current_time=local_sync_time, remote_time=remote_sync_time
    ))
    offset = synchronizer.output()
    assert abs(
        int(remote_sync_time + time_delta + offset) -
        int(local_sync_time * 1000 + time_delta)
    ) <= 2  # 1 ms tolerance for rounding error


@hp.given(time_delta=st.integers(
    min_value=0,
    max_value=(2 ** 32 - 1 - 1000)  # avoid uint_32 rollover in remote time
))
def test_no_reinitialization(time_delta: int) -> None:
    """Test clock synchronization offset initialization with no rollover."""
    synchronizer = clocks.ClockSynchronizer()
    synchronizer.input(clocks.UpdateEvent(current_time=1.0, remote_time=1000))
    offset = synchronizer.output()
    synchronizer.input(clocks.UpdateEvent(
        current_time=1.0 + time_delta * 1000,
        remote_time=1000 + time_delta
    ))
    assert offset == synchronizer.output()


def test_rollover() -> None:
    """Test clock synchronization offset reinitialization with rollover."""
    synchronizer = clocks.ClockSynchronizer()
    synchronizer.input(clocks.UpdateEvent(
        current_time=1.0, remote_time=2 ** 32 - 1000
    ))
    assert synchronizer.output() == 2000 - 2 ** 32
    synchronizer.input(clocks.UpdateEvent(
        current_time=2,
        remote_time=0
    ))
    assert synchronizer.output() == 2000
