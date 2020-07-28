/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  An interrupt-safe byte stream backed by a circular buffer.
 *  Inspired by https://hackaday.com/2015/10/29/embed-with-elliot-going-round-with-circular-buffers/
 */

#pragma once

#include <stdint.h>
#include "Pufferfish/Types.h"
#include "Pufferfish/Statuses.h"

namespace Pufferfish {
namespace Util {

// BufferSize is recommended to be a power of two for compiler optimization.
template <AtomicSize BufferSize>
class RingBuffer {
public:
  RingBuffer();

  static const AtomicSize maxSize = BufferSize;

  // Methods are declared volatile because they're usable with ISRs.
  BufferReadStatus read(uint8_t &readByte) volatile;
  BufferReadStatus peek(uint8_t &readByte) const volatile;
  BufferWriteStatus write(uint8_t writeByte) volatile;
  AtomicSize write(const uint8_t *writeBytes, AtomicSize writeSize);
  AtomicSize write(const uint8_t *writeBytes, AtomicSize writeSize) volatile;

protected:
  uint8_t buffer[BufferSize];
  AtomicSize newestIndex = 0;
  AtomicSize oldestIndex = 0;
};

} // namespace Util
} // namespace Pufferfish

#include "RingBuffer.tpp"
