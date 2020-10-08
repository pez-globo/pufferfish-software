/// BufferedUART.h
/// This file has interface class and methods for BufferedUART.

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

#include "Pufferfish/HAL/Types.h"
#include "Pufferfish/Statuses.h"

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
      const uint8_t *write_bytes,
      AtomicSize write_size,
      HAL::AtomicSize &written_size) volatile = 0;

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
