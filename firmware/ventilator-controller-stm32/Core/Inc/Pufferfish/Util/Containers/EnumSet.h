/*
 * EnumSet.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  A statically-allocated set for a finite, pre-determined set of keys.
 *  Backed by an array of pairs. Methods use early returns of status codes instead
 *  of exceptions for error handling, for bounds-checking.
 *  This Set is designed for frequent lookups but doesn't support traversal.
 */

#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "Set.h"

namespace Pufferfish::Util::Containers {

// Key must be an enum backed by an unsigned integer, or else it must support static_cast'ing to a
// size_t. The Set is able to store keys which get casted to integers from 0 to capacity - 1,
// inclusive.
template <typename Key, size_t capacity>
class EnumSet : public Set<Key> {
 public:
  using InitializerList = std::initializer_list<Key>;

  EnumSet() = default;
  // Construct the EnumSet with an initial set of keys, given as an
  // initializer list (e.g. EnumSet set{k1, k2, k3};)
  // cppcheck has a false positive in wanting this constructor to be explicit:
  // clang-tidy says we can't use explicit for a constructor with an initializer list!
  // cppcheck-suppress noExplicitConstructor
  EnumSet(InitializerList init) noexcept;

  [[nodiscard]] static constexpr size_t index_of(const Key &key) { return static_cast<size_t>(key); }
  // Note: when capacity is 0, this returns the max value of size_t
  [[nodiscard]] static constexpr size_t max_key_value() noexcept { return capacity - 1; }

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return capacity; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  IndexStatus input(const Key &key) noexcept;             // O(1)
  void clear();                                           // O(n)
  IndexStatus erase(const Key &key) noexcept;             // O(1)
  [[nodiscard]] bool has(const Key &key) const override;  // O(1)

 private:
  std::array<bool, capacity> occupancies_{};
  size_t size_ = 0;
};

}  // namespace Pufferfish::Util::Containers

#include "EnumSet.tpp"
