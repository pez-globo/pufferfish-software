/*
 * ByteArray.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_UTIL_BYTEARRAY_TPP_
#define INC_PUFFERFISH_UTIL_BYTEARRAY_TPP_

#include "ByteArray.h"

#include <cstring>

namespace Pufferfish { namespace Util {

template<size_t ArraySize>
ByteArray<ArraySize>::ByteArray() {}

template<size_t ArraySize>
size_t ByteArray<ArraySize>::size() const {
  return _size;
}

template<size_t ArraySize>
bool ByteArray<ArraySize>::empty() const {
  return _size == 0;
}

template<size_t ArraySize>
bool ByteArray<ArraySize>::full() const {
  return _size == maxSize;
}

template<size_t ArraySize>
size_t ByteArray<ArraySize>::available() const {
  return maxSize - _size;
}


template<size_t ArraySize>
void ByteArray<ArraySize>::clear() {
  _size = 0;
}

template<size_t ArraySize>
IndexStatus ByteArray<ArraySize>::resize(size_t newSize) {
  if (newSize > maxSize) {
    return IndexStatus::outOfBounds;
  }
  _size = newSize;
  return IndexStatus::ok;
}

template<size_t ArraySize>
IndexStatus ByteArray<ArraySize>::pushBack(uint8_t newByte) {
  if (_size == maxSize) {
    return IndexStatus::outOfBounds;
  }
  buffer[_size] = newByte;
  ++_size;
  return IndexStatus::ok;
}

template<size_t ArraySize>
void ByteArray<ArraySize>::copyFrom(
    const ByteArray<ArraySize> &sourceBytes, size_t destStartIndex
) {
  copyFrom(sourceBytes.buffer, sourceBytes.size(), destStartIndex);
}

template<size_t ArraySize>
void ByteArray<ArraySize>::copyFrom(
    const uint8_t *sourceBytes, size_t sourceSize, size_t destStartIndex
) {
  _size = maxSize;
  if (sourceSize + destStartIndex < _size) {
    _size = sourceSize + destStartIndex;
  }
  memcpy(buffer + destStartIndex, sourceBytes, _size - destStartIndex);
}


} }

#endif /* INC_PUFFERFISH_UTIL_BYTEARRAY_TPP_ */
