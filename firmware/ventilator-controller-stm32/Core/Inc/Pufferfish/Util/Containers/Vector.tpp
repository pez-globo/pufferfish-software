/*
 * Vector.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstring>

#include "Vector.h"

namespace Pufferfish::Util::Containers {

template <typename Element, size_t array_size>
size_t Vector<Element, array_size>::size() const {
  return size_;
}

template <typename Element, size_t array_size>
bool Vector<Element, array_size>::empty() const {
  return size_ == 0;
}

template <typename Element, size_t array_size>
bool Vector<Element, array_size>::full() const {
  return size_ == array_size;
}

template <typename Element, size_t array_size>
size_t Vector<Element, array_size>::available() const {
  return array_size - size_;
}

template <typename Element, size_t array_size>
void Vector<Element, array_size>::clear() {
  size_ = 0;
}

template <typename Element, size_t array_size>
IndexStatus Vector<Element, array_size>::resize(size_t new_size) {
  if (new_size > array_size) {
    return IndexStatus::out_of_bounds;
  }
  size_ = new_size;
  return IndexStatus::ok;
}

template <typename Element, size_t array_size>
IndexStatus Vector<Element, array_size>::push_back(const Element &new_elem) {
  if (size_ == array_size) {
    return IndexStatus::out_of_bounds;
  }
  buffer_[size_] = new_elem;
  ++size_;
  return IndexStatus::ok;
}

template <typename Element, size_t array_size>
IndexStatus Vector<Element, array_size>::erase(size_t index) {
  if (index >= size_) {
    return IndexStatus::out_of_bounds;
  }

  for (size_t i = index; i < size_ - 1; ++i) {
    buffer_[i] = buffer_[i + 1];
  }

  --size_;
  return IndexStatus::ok;
}

template <typename Element, size_t array_size>
constexpr const Element &Vector<Element, array_size>::operator[](size_t position) const noexcept {
  return buffer_[position];
}

template <typename Element, size_t array_size>
constexpr Element &Vector<Element, array_size>::operator[](size_t position) noexcept {
  return buffer_[position];
}

template <typename Element, size_t array_size>
template <size_t source_size>
IndexStatus Vector<Element, array_size>::copy_from(
    const std::array<Element, source_size> &source_bytes, size_t dest_start_index) {
  return copy_from(source_bytes.data(), source_bytes.size(), dest_start_index);
}

template <typename Element, size_t array_size>
IndexStatus Vector<Element, array_size>::copy_from(
    const Vector<Element, array_size> &source_bytes, size_t dest_start_index) {
  return copy_from(source_bytes.buffer(), source_bytes.size(), dest_start_index);
}

template <typename Element, size_t array_size>
IndexStatus Vector<Element, array_size>::copy_from(
    const Element *source_bytes, size_t source_size, size_t dest_start_index) {
  if (source_size + dest_start_index > array_size) {
    return IndexStatus::out_of_bounds;
  }
  size_ = source_size + dest_start_index;
  memcpy(
      buffer_.data() + dest_start_index,
      source_bytes,
      sizeof(Element) * (size_ - dest_start_index));
  return IndexStatus::ok;
}

template <typename Element, size_t array_size>
typename Vector<Element, array_size>::Iterator Vector<Element, array_size>::begin() noexcept {
  return buffer_.begin();
}

template <typename Element, size_t array_size>
typename Vector<Element, array_size>::ConstIterator Vector<Element, array_size>::cbegin()
    const noexcept {
  return buffer_.cbegin();
}

template <typename Element, size_t array_size>
typename Vector<Element, array_size>::Iterator Vector<Element, array_size>::end() noexcept {
  return buffer_.begin() + size_;
}

template <typename Element, size_t array_size>
typename Vector<Element, array_size>::ConstIterator Vector<Element, array_size>::cend()
    const noexcept {
  return buffer_.cbegin() + size_;
}

}  // namespace Pufferfish::Util::Containers
