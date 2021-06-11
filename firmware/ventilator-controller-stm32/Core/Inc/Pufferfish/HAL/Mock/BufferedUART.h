/// BufferedUART.h
/// This file has mock class and methods for unit testing of Buffered
/// UART.

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Pufferfish/HAL/Interfaces/BufferedUART.h"
#include "Pufferfish/HAL/Types.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/Containers/RingBuffer.h"

namespace Pufferfish::HAL::Mock {

/**
 * UART RX and TX with non-blocking queue interface.
 *
 * Backed by ring buffers serviced by interrupt handlers.
 * This class allows a "polling" approach to reading and writing
 * data which is sent/received asynchronously from the other peer.
 * This provides a comparable interface to Arduino's Serial class,
 * but with a non-blocking interface.
 */
template <AtomicSize rx_buffer_size, AtomicSize tx_buffer_size>
class BufferedUART : public Interfaces::BufferedUART {
 public:
  /**
   * Mock constructor for BufferedUART
   */
  explicit BufferedUART();

  /**
   * Read byte data from ring buffer
   * @param  Read byte output data
   * @return buffer status of ring buffer
   */
  BufferStatus read(uint8_t &read_byte) volatile override;

  /**
   * sets read byte data from ring buffer
   * @param  Set read byte input data
   * @return None
   */
  void set_read(const uint8_t &byte) volatile;

  /**
   * Write byte data to ring buffer
   * @param  write byte input data
   * @return buffer status of ring buffer
   */
  BufferStatus write(uint8_t write_byte) volatile override;

  /**
   * Gets write byte data from ring buffer
   * @param  write byte output data
   * @return None
   */
  void get_write(uint8_t &byte) volatile;

  /**
   * write data to ring buffer
   * @param  write_bytes  array of write bytes input
   * @param  write_size   size of array to write
   * @param  written_size size of written array
   * @return buffer status of ring buffer
   */
  BufferStatus write(
      const uint8_t *write_bytes,
      AtomicSize write_size,
      HAL::AtomicSize &written_size) volatile override;

  /**
   * write data block to ring buffer
   * @param  write_byte  write byte input for block
   * @param  timeout     time to write block
   * @return buffer status of ring buffer
   */
  BufferStatus write_block(uint8_t write_byte, uint32_t timeout) volatile override;

  /**
   * gets write data block from ring buffer
   * @param  byte       get write byte input for block
   * @param  timeout    time to get write block
   * @return buffer status of ring buffer
   */
  void get_write_block(uint8_t *byte, uint32_t timeout) volatile;

  /**
   * write data block to ring buffer
   * @param  write_bytes array of write bytes input for block
   * @param  write_size   size of array to write
   * @param  timeout     time to write block
   * @param  written_size size of written array
   * @return buffer status of ring buffer
   */
  BufferStatus write_block(
      const uint8_t *write_bytes,
      AtomicSize write_size,
      uint32_t timeout,
      HAL::AtomicSize &written_size) volatile override;

 private:
  volatile Util::Containers::RingBuffer<rx_buffer_size, uint8_t> rx_buffer_;
  volatile Util::Containers::RingBuffer<tx_buffer_size, uint8_t> tx_buffer_;
};

static const size_t mock_large_uart_buffer_size = 4096;
using LargeBufferedUART = BufferedUART<mock_large_uart_buffer_size, mock_large_uart_buffer_size>;

static const size_t mock_read_only_uart_buffer_size = 512;
using ReadOnlyBufferedUART = BufferedUART<mock_read_only_uart_buffer_size, 1>;

}  // namespace Pufferfish::HAL::Mock

#include "Pufferfish/HAL/Mock/BufferedUART.tpp"
