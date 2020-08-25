/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "RingBuffer.h"

#include <cstring>

namespace Pufferfish {
namespace Util {

template<HAL::AtomicSize BufferSize>
RingBuffer<BufferSize>::RingBuffer() {}

template<HAL::AtomicSize BufferSize>
BufferStatus RingBuffer<BufferSize>::read(uint8_t &byte) volatile {
  if (newestIndex == oldestIndex) {
      return BufferStatus::empty;
  }

  byte = buffer[oldestIndex];
  oldestIndex = (oldestIndex + 1) % maxSize;
  return BufferStatus::ok;
}

template<HAL::AtomicSize BufferSize>
BufferStatus RingBuffer<BufferSize>::peek(uint8_t &byte) const volatile {
  if (newestIndex == oldestIndex) {
      return BufferStatus::empty;
  }

  byte = buffer[oldestIndex];
  return BufferStatus::ok;
}

template<HAL::AtomicSize BufferSize>
BufferStatus RingBuffer<BufferSize>::write(uint8_t writeByte) volatile {
  HAL::AtomicSize nextIndex = (newestIndex + 1) % maxSize;
  if (nextIndex == oldestIndex) {
      return BufferStatus::full;
  }

  buffer[newestIndex] = writeByte;
  newestIndex = nextIndex;
  return BufferStatus::ok;
}

} // namespace Util
} // namespace Pufferfish
