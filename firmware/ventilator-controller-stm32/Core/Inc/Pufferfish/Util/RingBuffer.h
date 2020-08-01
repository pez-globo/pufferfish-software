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
#include "Pufferfish/Statuses.h"
#include "Pufferfish/HAL/Types.h"

namespace Pufferfish {
namespace Util {

// BufferSize is recommended to be a power of two for compiler optimization.
template <HAL::AtomicSize BufferSize>
class RingBuffer {
public:
  RingBuffer();

  static const HAL::AtomicSize maxSize = BufferSize;

  // Methods are declared volatile because they're usable with ISRs.
  BufferStatus read(uint8_t &readByte) volatile;
  BufferStatus peek(uint8_t &readByte) const volatile;
  BufferStatus write(uint8_t writeByte) volatile;
  HAL::AtomicSize write(const uint8_t *writeBytes, HAL::AtomicSize writeSize);
  HAL::AtomicSize write(const uint8_t *writeBytes, HAL::AtomicSize writeSize) volatile;

protected:
  uint8_t buffer[BufferSize];
  HAL::AtomicSize newestIndex = 0;
  HAL::AtomicSize oldestIndex = 0;
};

} // namespace Util
} // namespace Pufferfish

#include "RingBuffer.tpp"
