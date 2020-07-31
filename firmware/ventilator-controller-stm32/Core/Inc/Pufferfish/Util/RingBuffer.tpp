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
BufferReadStatus RingBuffer<BufferSize>::read(uint8_t &byte) volatile {
  if (newestIndex == oldestIndex) {
      return BufferReadStatus::empty;
  }

  byte = buffer[oldestIndex];
  oldestIndex = (oldestIndex + 1) % maxSize;
  return BufferReadStatus::ok;
}

template<HAL::AtomicSize BufferSize>
BufferReadStatus RingBuffer<BufferSize>::peek(uint8_t &byte) const volatile {
  if (newestIndex == oldestIndex) {
      return BufferReadStatus::empty;
  }

  byte = buffer[oldestIndex];
  return BufferReadStatus::ok;
}

template<HAL::AtomicSize BufferSize>
BufferWriteStatus RingBuffer<BufferSize>::write(uint8_t writeByte) volatile {
  HAL::AtomicSize nextIndex = (newestIndex + 1) % maxSize;
  if (nextIndex == oldestIndex) {
      return BufferWriteStatus::full;
  }

  buffer[newestIndex] = writeByte;
  newestIndex = nextIndex;
  return BufferWriteStatus::ok;
}

template<HAL::AtomicSize BufferSize>
HAL::AtomicSize RingBuffer<BufferSize>::write(
    const uint8_t *writeBytes, HAL::AtomicSize writeSize
) {
  HAL::AtomicSize nextIndex = (newestIndex + 1) % maxSize;
  if (nextIndex == oldestIndex) {
      return 0;
  }

  HAL::AtomicSize newestAvailableIndex = (oldestIndex - 1 + maxSize) % maxSize;
  HAL::AtomicSize writtenSize;
  if (newestAvailableIndex >= nextIndex) {
    writtenSize = newestAvailableIndex - nextIndex + 1;
    newestIndex = newestAvailableIndex;
  } else { // Available bytes wrap around the end of the buffer
    // Only write to the end, let the remainder be written on the next call
    writtenSize = maxSize - nextIndex;
    newestIndex = maxSize - 1;
  }
  memcpy(buffer + nextIndex, writeBytes, writtenSize);
  return writtenSize;
}

template<HAL::AtomicSize BufferSize>
HAL::AtomicSize RingBuffer<BufferSize>::write(
    const uint8_t *writeBytes, HAL::AtomicSize writeSize
) volatile {
  HAL::AtomicSize i;
  for (i = 0; i < writeSize; ++i) {
    if (write(writeBytes[i]) != BufferWriteStatus::ok) {
      break;
    }
  }
  return i;
}

} // namespace Util
} // namespace Pufferfish
