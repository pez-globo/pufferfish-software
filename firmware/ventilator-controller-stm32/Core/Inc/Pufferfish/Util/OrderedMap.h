/*
 * OrderedMap.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  A statically-allocated map which remembers order of initial insertion.
 *  Backed by a Vector. Methods use early returns of status codes instead
 *  of exceptions for error handling, for bounds-checking.
 *  This Map is best suited for uses which don't need frequent lookups,
 *  as lookups are on average O(n).
 */

#pragma once

#include <cstddef>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "Vector.h"

namespace Pufferfish::Util {

template <typename Key, typename Value, size_t max_pairs>
class OrderedMap {
 public:
  using Pair = std::pair<Key, Value>;

  OrderedMap() = default;

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return max_pairs; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  void clear();
  IndexStatus insert(const Key &key, const Value &value);
  IndexStatus erase(const Key &key);
  IndexStatus find(const Key &key, size_t &index) const;

  [[nodiscard]] constexpr const Vector<Pair, max_pairs> &items() const { return buffer_; }
  constexpr Vector<Pair, max_pairs> &items() { return buffer_; }

 private:
  Vector<Pair, max_pairs> buffer_{};
};

}  // namespace Pufferfish::Util

#include "OrderedMap.tpp"