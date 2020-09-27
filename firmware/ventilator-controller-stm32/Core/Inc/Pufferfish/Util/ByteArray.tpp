/*
 * ByteArray.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstring>

#include "ByteArray.h"

namespace Pufferfish::Util {

template <size_t array_size>
ByteArray<array_size>::ByteArray() = default;

template <size_t array_size>
size_t ByteArray<array_size>::size() const {
  return size_;
}

template <size_t array_size>
bool ByteArray<array_size>::empty() const {
  return size_ == 0;
}

template <size_t array_size>
bool ByteArray<array_size>::full() const {
  return size_ == max_size;
}

template <size_t array_size>
size_t ByteArray<array_size>::available() const {
  return max_size - size_;
}

template <size_t array_size>
void ByteArray<array_size>::clear() {
  size_ = 0;
}

template <size_t array_size>
IndexStatus ByteArray<array_size>::resize(size_t newSize) {
  if (newSize > max_size) {
    return IndexStatus::out_of_bounds;
  }
  size_ = newSize;
  return IndexStatus::ok;
}

template <size_t array_size>
IndexStatus ByteArray<array_size>::push_back(uint8_t new_byte) {
  if (size_ == max_size) {
    return IndexStatus::out_of_bounds;
  }
  buffer[size_] = new_byte;
  ++size_;
  return IndexStatus::ok;
}

template <size_t array_size>
void ByteArray<array_size>::copy_from(
    const ByteArray<array_size> &source_bytes, size_t dest_start_index) {
  copy_from(source_bytes.buffer, source_bytes.size(), dest_start_index);
}

template <size_t array_size>
void ByteArray<array_size>::copy_from(
    const uint8_t *source_bytes, size_t source_size, size_t dest_start_index) {
  size_ = max_size;
  if (source_size + dest_start_index < size_) {
    size_ = source_size + dest_start_index;
  }
  memcpy(buffer + dest_start_index, source_bytes, size_ - dest_start_index);
}

}  // namespace Pufferfish::Util
