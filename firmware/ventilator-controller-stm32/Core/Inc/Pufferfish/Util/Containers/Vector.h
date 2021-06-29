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
#include <cstdint>
#include <initializer_list>

#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util::Containers {

template <typename Element, size_t array_size>
class Vector {
 public:
  using Array = typename std::array<Element, array_size>;
  using Iterator = typename Array::iterator;
  using ConstIterator = typename Array::const_iterator;

  Vector() = default;
  // Construct the Vector with an initial list of elements, given as an
  // initializer list (e.g. Vector<uint8_t, 5> vec{1, 2, 3})
  // Note: this makes copies of the values!
  Vector(std::initializer_list<Element> init) : size_(init.size()) {
    for (const Element &elem : init) {
      push_back(elem);
    }
  }

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

  // TODO(lietk12): remove these methods!
  [[nodiscard]] constexpr const Element *buffer() const noexcept { return buffer_.data(); }
  constexpr Element *buffer() noexcept { return buffer_.data(); }

  // Note: these are std::array iterators, so their iterators can throw exceptions for bounds
  // errors! These methods are only provided to support range-based for loops, and they should not
  // be called directly by other code.
  [[nodiscard]] Iterator begin() noexcept;
  [[nodiscard]] ConstIterator cbegin() const noexcept;
  [[nodiscard]] Iterator end() noexcept;
  [[nodiscard]] ConstIterator cend() const noexcept;

 private:
  Array buffer_{};
  size_t size_ = 0;
};

template <size_t array_size>
using ByteVector = Vector<uint8_t, array_size>;

}  // namespace Pufferfish::Util::Containers

#include "Vector.tpp"
