/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  An interrupt-backed UART I/O endpoint, exposing a buffered read/write
 * interface.
 */

#pragma once

#include "Pufferfish/Statuses.h"
#include "Pufferfish/HAL/Types.h"

namespace Pufferfish {
namespace HAL {

class BufferedUART {
 public:
  /**
   * Read byte data from ring buffer
   * @param  Read byte output data
   * @return buffer status of ring buffer
   */
  virtual BufferStatus read(uint8_t &read_byte) volatile = 0;

  /**
   * Write byte data to ring buffer
   * @param  write byte input data
   * @return buffer status of ring buffer
   */
  virtual BufferStatus write(uint8_t write_byte) volatile = 0;

  /**
   * write data to ring buffer
   * @param  write_bytes  array of write bytes input
   * @param  write_size   size of array to write
   * @param  written_size size of written array
   * @return buffer status of ring buffer
   */
  virtual BufferStatus write(
      const uint8_t *write_bytes, AtomicSize write_size, HAL::AtomicSize &written_size) volatile = 0;

  /**
   * write data block to ring buffer
   * @param  write_byte  write byte input for block
   * @param  timeout     time to write block
   * @return buffer status of ring buffer
   */
  virtual BufferStatus write_block(uint8_t write_byte, uint32_t timeout) volatile = 0;

  /**
   * gets write data block from ring buffer
   * @param  byte       get write byte input for block
   * @param  timeout    time to get write block
   * @return buffer status of ring buffer
   */
  virtual BufferStatus write_block(
      const uint8_t *write_bytes,
      AtomicSize write_size,
      uint32_t timeout,
      HAL::AtomicSize &written_size) volatile = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
