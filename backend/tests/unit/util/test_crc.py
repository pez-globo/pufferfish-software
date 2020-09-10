"""Test the functionality of the util.crc module."""

from crcmod import predefined as predefinedcrc  # type: ignore

import hypothesis as hp
import hypothesis.strategies as st

import pytest as pt  # type: ignore

from ventserver.util import crc


CRC32C_REFERENCE = predefinedcrc.mkCrcFun('crc-32c')


@pt.mark.parametrize('data,expected', [
    # From the catalogue of parameterized CRC algorithms
    (b'123456789', 0xE3069283),
    # Calculated using the Sunshine Online CRC Calculator
    (b'', 0x00000000),
    (b'\x00', 0x527d5351),
    (b'\x01', 0xa016d052),
])
def test_crc32c(data: bytes, expected: int) -> None:
    """Test CRC-32C against known reference data and expected values."""
    assert crc.compute_reflected_crc(
        data, crc.CRC32C_REFLECTED_TABLE
    ) == expected


@hp.given(data=st.binary(min_size=0, max_size=512))
def test_crc32c_reference(data: bytes) -> None:
    """Test CRC-32C table-based implementation against reference library."""
    assert crc.compute_reflected_crc(
        data, crc.CRC32C_REFLECTED_TABLE
    ) == CRC32C_REFERENCE(data)
