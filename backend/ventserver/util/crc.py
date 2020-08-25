"""CRC calculation.

Provides a table-lookup implementation of CRC computation, along with an
implementation of the CRC-32C (Castagnoli) specification.

    Typical usage example:

    buffer = b'123456789'
    print(compute_reflected_crc(buffer))  # 0xE3069283
"""

from typing import List


def reflect(data: int, num_bits: int) -> int:
    """Reflect a number by bit positions.

    Args:
        data: An integer whose bits will be reflected.
        num_bits: The width of (i.e. number of bits in) data.

    Returns:
        An integer of width num_bits, whose bits are the reflection of data.

    """
    reflection = 0x0
    for bit in range(num_bits):
        if data & 0x1:
            reflection = reflection | (1 << (num_bits - 1 - bit))
        data = data >> 1
    return reflection


def generate_reflected_table(polynomial: int, num_bits: int = 32) -> List[int]:
    """Generate CRC byte lookup table for a given polynomial.

    Args:
        polynomial: A CRC polynomial represented as an integer.
        num_bits: The width of the CRC polynomial.

    Returns:
        A CRC lookup table, represented as a lookup table indexed by byte value.

    """
    table = []
    reflected_polynomial = reflect(polynomial, num_bits)

    for dividend in range(256):
        byte = dividend
        for _ in range(8):
            if byte & 0x1:
                byte = byte >> 1
                byte = byte ^ reflected_polynomial
            else:
                byte = byte >> 1
        table.append(byte)

    return table


CRC32C_POLYNOMIAL = 0x1EDC6F41
CRC32C_REFLECTED_TABLE = generate_reflected_table(CRC32C_POLYNOMIAL, 32)


def compute_reflected_crc(
        buffer: bytes, table: List[int],
        initial_remainder: int = 0xFFFFFFFF, final_xor: int = 0xFFFFFFFF
) -> int:
    """Compute 32-bit reflected crc using a lookup table.

    Args:
        buffer: The data over which the CRC should be computed, represented as
            a byte buffer.
        table: A CRC lookup table, such as is returned by
            generate_reflected_table.
        initial_remainder: An integer representing the initial value of the
            CRC remainder.
        final_xor: An integer representing the value which the final CRC
            remainder should be XOR'ed with, resulting in the CRC checksum.

    Returns:
        The CRC checksum of the input data computed using the provided CRC
        parameters.

    """
    remainder = initial_remainder
    for byte in buffer:
        data = byte ^ (remainder & 0xFF)
        remainder = table[data] ^ (remainder >> 8)

    return remainder ^ final_xor
