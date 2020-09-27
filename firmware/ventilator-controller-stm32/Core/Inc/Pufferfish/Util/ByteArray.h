/*
 * ByteArray.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  A basic statically-allocated variable-length byte buffer.
 *  Backed by an array. Methods use early returns of status codes instead
 *  of exceptions for error handling, for bounds-checking.
 *  Obviously, direct access of the buffer and size fields does not
 *  provide any bounds-checking.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include "Pufferfish/HAL/Types.h"
#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util {

// BufferSize is recommended to be a power of two for compiler optimization.
template <size_t ArraySize>
class ByteArray {
public:
  ByteArray();

  static const HAL::AtomicSize maxSize = ArraySize;

  uint8_t buffer[ArraySize];

  size_t size() const;
  bool empty() const;
  bool full() const;
  size_t available() const;

  void clear();
  IndexStatus resize(size_t newSize);
  IndexStatus pushBack(uint8_t newByte);

  void copyFrom(
      const ByteArray<ArraySize> &sourceBytes, size_t destStartIndex = 0
  );
  void copyFrom(
      const uint8_t *sourceBytes, size_t sourceSize, size_t destStartIndex = 0
  );

protected:
  size_t _size = 0;
};

}

#include "ByteArray.tpp"
