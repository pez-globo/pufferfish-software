/*
 * COBS.cpp
 *
 *  Created on: May 17, 2020
 *      Author: Ethan Li
 *
 *  Import of the COBS algorithm from Christopher Baker's PacketSerial project.
 *
 *  Copyright (c) 2011 Christopher Baker <https://christopherbaker.net>
 *  Copyright (c) 2011 Jacques Fortier <https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing>
 *  SPDX-License-Identifier: MIT
 *
 *  MIT License
 *  Copyright (c) 2017 Christopher Baker https://christopherbaker.net
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "COBS.h"

namespace Pufferfish { namespace Util {

size_t encodeCOBS(
    const uint8_t *buffer, size_t size, uint8_t *encodedBuffer
) {
  size_t read_index = 0;
  size_t write_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;

  while (read_index < size) {
    if (buffer[read_index] == 0) {
      encodedBuffer[code_index] = code;
      code = 1;
      code_index = write_index++;
      read_index++;
    } else {
      encodedBuffer[write_index++] = buffer[read_index++];
      code++;

      if (code == 0xFF) {
        encodedBuffer[code_index] = code;
        code = 1;
        code_index = write_index++;
      }
    }
  }

  encodedBuffer[code_index] = code;

  return write_index;
}

size_t decodeCOBS(
    const uint8_t *encodedBuffer, size_t size, uint8_t *decodedBuffer
) {
  if (size == 0) {
    return 0;
  }

  size_t read_index = 0;
  size_t write_index = 0;
  uint8_t code = 0;
  uint8_t i = 0;

  while (read_index < size) {
    code = encodedBuffer[read_index];

    if (read_index + code > size && code != 1) {
      return 0;
    }

    read_index++;

    for (i = 1; i < code; i++) {
      decodedBuffer[write_index++] = encodedBuffer[read_index++];
    }

    if (code != 0xFF && read_index != size) {
      decodedBuffer[write_index++] = '\0';
    }
  }

  return write_index;
}

size_t getEncodedCOBSBufferSize(size_t unencodedBufferSize)
{
  return unencodedBufferSize + unencodedBufferSize / 254 + 1;
}

} }
