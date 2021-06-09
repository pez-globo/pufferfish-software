/*
 * Vector.h
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

#include <array>
#include <cstddef>
#include <initializer_list>

#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util::Containers {

template <typename Element, size_t array_size>
class Vector {
 public:
  Vector() = default;
  // Construct the Vector with an initial list of elements, given as an
  // initializer list (e.g. Vector vect{v1, v2, v3};)
  // Note: this makes copies of the values!
  Vector(std::initializer_list<Element> init) : buffer_(init), size_(init.size()) {}

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return array_size; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  void clear();
  IndexStatus resize(size_t new_size);
  IndexStatus push_back(const Element &new_elem);
  IndexStatus erase(size_t index);

  // Note: these don't perform bounds-checking!
  constexpr const Element &operator[](size_t position) const noexcept;
  constexpr Element &operator[](size_t position) noexcept;

  // Note: these perform shallow copies!
  template <size_t source_size>
  IndexStatus copy_from(
      const std::array<Element, source_size> &source_bytes, size_t dest_start_index = 0);
  IndexStatus copy_from(
      const Vector<Element, array_size> &source_bytes, size_t dest_start_index = 0);
  IndexStatus copy_from(
      const Element *source_bytes, size_t source_size, size_t dest_start_index = 0);

  [[nodiscard]] constexpr const Element *buffer() const noexcept { return buffer_.data(); }
  constexpr Element *buffer() noexcept { return buffer_.data(); }

 private:
  std::array<Element, array_size> buffer_{};
  size_t size_ = 0;
};

template <size_t array_size>
using ByteVector = Vector<uint8_t, array_size>;

}  // namespace Pufferfish::Util::Containers

#include "Vector.tpp"
