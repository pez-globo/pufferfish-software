/*
 * COBS.cpp
 *
 *  Created on: May 17, 2020
 *      Author: Ethan Li
 *
 *  Import of the COBS algorithm from Christopher Baker's PacketSerial project.
 *
 *  Copyright (c) 2011 Christopher Baker <https://christopherbaker.net>
 *  Copyright (c) 2011 Jacques Fortier
 * <https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing> SPDX-License-Identifier: MIT
 *
 *  MIT License
 *  Copyright (c) 2017 Christopher Baker https://christopherbaker.net
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: The above copyright notice and this
 * permission notice shall be included in all copies or substantial portions of the Software. THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Pufferfish/Util/COBS.h"

namespace Pufferfish::Util {

static const size_t max_block_size = 254;

size_t encode_cobs(const uint8_t *buffer, size_t size, uint8_t *encoded_buffer) {
  size_t read_index = 0;
  size_t write_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;

  while (read_index < size) {
    if (buffer[read_index] == 0) {
      encoded_buffer[code_index] = code;
      code = 1;
      code_index = write_index++;
      read_index++;
    } else {
      encoded_buffer[write_index++] = buffer[read_index++];
      code++;

      if (code == max_block_size + 1) {
        encoded_buffer[code_index] = code;
        code = 1;
        code_index = write_index++;
      }
    }
  }

  encoded_buffer[code_index] = code;

  return write_index;
}

size_t decode_cobs(const uint8_t *encoded_buffer, size_t size, uint8_t *decoded_buffer) {
  if (size == 0) {
    return 0;
  }

  size_t read_index = 0;
  size_t write_index = 0;

  while (read_index < size) {
    uint8_t code = encoded_buffer[read_index];

    if (read_index + code > size && code != 1) {
      return 0;
    }

    read_index++;

    for (uint8_t i = 1; i < code; i++) {
      decoded_buffer[write_index++] = encoded_buffer[read_index++];
    }

    if (code != max_block_size + 1 && read_index != size) {
      decoded_buffer[write_index++] = '\0';
    }
  }

  return write_index;
}

size_t get_encoded_cobs_buffer_size(size_t unencoded_buffer_size) {
  return unencoded_buffer_size + unencoded_buffer_size / max_block_size + 1;
}

}  // namespace Pufferfish::Util
