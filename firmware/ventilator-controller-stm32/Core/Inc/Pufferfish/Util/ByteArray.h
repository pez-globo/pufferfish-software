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

#include <cstddef>
#include <cstdint>

#include "Pufferfish/HAL/Types.h"
#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util {

// BufferSize is recommended to be a power of two for compiler optimization.
template <size_t array_size>
class ByteArray {
 public:
  ByteArray();

  static const HAL::AtomicSize max_size = array_size;

  uint8_t buffer[array_size]{};

  [[nodiscard]] size_t size() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  void clear();
  IndexStatus resize(size_t new_size);
  IndexStatus push_back(uint8_t new_byte);

  void copy_from(const ByteArray<array_size> &source_bytes, size_t dest_start_index = 0);
  void copy_from(const uint8_t *source_bytes, size_t source_size, size_t dest_start_index = 0);

 private:
  size_t size_ = 0;
};

}  // namespace Pufferfish::Util

#include "ByteArray.tpp"
