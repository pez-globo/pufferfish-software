/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "MockBufferedUART.h"

namespace Pufferfish {
namespace HAL {

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
MockBufferedUART<rx_buffer_size, tx_buffer_size>::MockBufferedUART() {
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
BufferStatus MockBufferedUART<rx_buffer_size, tx_buffer_size>::read(uint8_t &read_byte) volatile {
  return rx_buffer_.read(read_byte);
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
void MockBufferedUART<rx_buffer_size, tx_buffer_size>::set_read(const uint8_t &byte) volatile {
  rx_buffer_.write(byte);
}


template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
BufferStatus MockBufferedUART<rx_buffer_size, tx_buffer_size>::write(uint8_t write_byte) volatile {
  BufferStatus status = tx_buffer_.write(write_byte);
  return status;
}


template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
void MockBufferedUART<rx_buffer_size, tx_buffer_size>::get_write(uint8_t &byte) volatile {
  tx_buffer_.read(byte);
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
BufferStatus MockBufferedUART<rx_buffer_size, tx_buffer_size>::write(
    const uint8_t *write_bytes, AtomicSize write_size, HAL::AtomicSize &written_size) volatile {
  for (written_size = 0; written_size < write_size; ++written_size) {
    if (write(write_bytes[written_size]) != BufferStatus::ok) {
      break;
    }
  }
  if (write_size == written_size) {
    return BufferStatus::ok;
  }
  return BufferStatus::partial;
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
BufferStatus MockBufferedUART<rx_buffer_size, tx_buffer_size>::write_block(
    uint8_t write_byte, uint32_t timeout) volatile {
	uint32_t index;
	for(index = 0; index < timeout; index++) {
		if (write(write_byte) != BufferStatus::ok) {
		  break;
		}
	}
	if(index == timeout) {
	  return BufferStatus::ok;
	}
	return BufferStatus::partial;
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
void MockBufferedUART<rx_buffer_size, tx_buffer_size>::get_write_block(
    uint8_t *byte, const uint32_t timeout) volatile {
	for(uint32_t index = 0; index < timeout; index++) {
		if (read(byte[index]) != BufferStatus::ok) {
		  break;
		}
	}
}

template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
BufferStatus MockBufferedUART<rx_buffer_size, tx_buffer_size>::write_block(
    const uint8_t *write_bytes,
    AtomicSize write_size,
    uint32_t timeout,
    HAL::AtomicSize &written_size) volatile {
  uint8_t index = 0;
  while (written_size < write_size) {
    AtomicSize just_written = 0;
    write(write_bytes + written_size, write_size - written_size, just_written);
    written_size += just_written;
	if(++index >= timeout) {
		break;
	}
  }
  if (write_size == written_size) {
    return BufferStatus::ok;
  }
  return BufferStatus::partial;
}

}  // namespace HAL
}  // namespace Pufferfish
