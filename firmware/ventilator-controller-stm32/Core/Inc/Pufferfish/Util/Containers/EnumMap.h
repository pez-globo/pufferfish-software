/*
 * EnumMap.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  A statically-allocated map for a finite, pre-determined set of keys.
 *  Backed by an array of pairs. Methods use early returns of status codes instead
 *  of exceptions for error handling, for bounds-checking.
 *  This Map is designed for frequent lookups but doesn't support traversal.
 */

#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <utility>

#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util::Containers {

// Key must be an enum backed by an unsigned integer, or else it must support static_cast'ing to a
// size_t. The Map is able to store keys which get casted to integers from 0 to max_key, inclusive.
template <typename Key, typename Value, size_t max_key>
class EnumMap {
 public:
  EnumMap() = default;
  // Construct the EnumMap with an initial set of key-value pairs, given as an
  // initializer list (e.g. EnumMap map{{k1, v1}, {k2, v2}, {k3, v3}};)
  // Note: you must ensure that keys are unique; the value for the last copy of a duplicated key
  // will overwrite all previous values. Note: this constructor makes copies of the values!
  // cppcheck has a false positive in wanting this constructor to be explicit:
  // clang-tidy says we can't use explicit for a constructor with an initializer list!
  // cppcheck-suppress noExplicitConstructor
  EnumMap(std::initializer_list<std::pair<Key, Value>> init) noexcept;

  [[nodiscard]] static constexpr size_t max_key_value() noexcept { return max_key; }

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return max_key + 1; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  void clear();  // O(n)
  // Note: this makes a copy of value!
  IndexStatus insert(const Key &key, const Value &value) noexcept;  // O(1)
  IndexStatus erase(const Key &key) noexcept;                       // O(1)
  [[nodiscard]] bool has(const Key &key) const;                     // O(1)
  // Note: this copies the value in the map to the value output parameter!
  IndexStatus find(const Key &key, Value &value) const;  // O(1)

  // Note: these don't check whether the key exists or is in-bounds!
  const Value &operator[](const Key &key) const noexcept;
  Value &operator[](const Key &key) noexcept;

 private:
  std::array<bool, max_key + 1> occupancies_{};
  std::array<Value, max_key + 1> values_{};
  size_t size_ = 0;
};

}  // namespace Pufferfish::Util::Containers

#include "EnumMap.tpp"
