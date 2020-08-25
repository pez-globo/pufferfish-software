"""Test the functionality of sansio Filter classes."""

from typing import List, Optional

import hypothesis as hp
import hypothesis.strategies as st

from ventserver.sansio import protocols


# Filter interface


class MockFilter(protocols.Filter[int, int]):
    """Test derived class to test the Filter interface."""

    def __init__(self) -> None:
        """Initialize members."""
        self.num_emitted = 0
        self.num_to_emit = 0

    def input(self, event: Optional[int]) -> None:
        """Emit one more event."""
        if event is None:
            return

        self.num_to_emit += 1

    def output(self) -> Optional[int]:
        """Emit the next output event."""
        if self.num_emitted >= self.num_to_emit:
            return None
        self.num_emitted += 1
        return self.num_emitted


@hp.given(
    all_num_inputs=st.lists(
        st.integers(min_value=0, max_value=4), min_size=0, max_size=4
    )
)
def test_filter_output_all(all_num_inputs: List[int]) -> None:
    """Test Filter.output_all behavior."""
    test_filter = MockFilter()
    total_outputs = 0
    total_inputs = 0
    for num_inputs in all_num_inputs:
        for i in range(num_inputs):
            test_filter.input(i)
            total_inputs += 1
        for output in test_filter.output_all():
            total_outputs += 1
            assert output == total_outputs
        assert total_outputs == total_inputs


class UnbufferedFilter(protocols.Filter[str, str]):
    """Test derived class to test the Filter interface."""

    def __init__(self) -> None:
        """Initialize members."""
        self.last_input: Optional[str] = None

    def input(self, event: Optional[str]) -> None:
        """Store event."""
        if event is None:
            return

        self.last_input = event

    def output(self) -> Optional[str]:
        """Return the upper-cased form of the last event."""
        if self.last_input is None:
            return None

        last_input = self.last_input.upper()
        self.last_input = None
        return last_input


def test_unbuffered_filter() -> None:
    """Test UnbufferedFilter behavior."""
    unbuffered_filter = UnbufferedFilter()
    unbuffered_filter.input('foo')
    unbuffered_filter.input('bar')
    assert unbuffered_filter.output() == 'BAR'
    unbuffered_filter.input('foo')
    counter = 0
    for output in unbuffered_filter.output_all():
        assert output == 'FOO'
        counter += 1
    assert counter == 1
