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
#include <initializer_list>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "Vector.h"

namespace Pufferfish::Util::Containers {

template <typename Key, typename Value, size_t max_pairs>
class OrderedMap {
 public:
  using Pair = std::pair<Key, Value>;

  OrderedMap() = default;
  OrderedMap(std::initializer_list<Pair> init) : buffer_(init) {}

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return max_pairs; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  void clear();  // O(1)
  // Note: this makes a copy of value!
  IndexStatus insert(const Key &key, const Value &value);  // O(m)
  IndexStatus erase(const Key &key);                       // O(m)
  [[nodiscard]] bool has(const Key &key) const;            // O(m)
  IndexStatus find(const Key &key, size_t &index) const;   // O(m)

  [[nodiscard]] constexpr const Vector<Pair, max_pairs> &items() const { return buffer_; }
  constexpr Vector<Pair, max_pairs> &items() { return buffer_; }

 private:
  Vector<Pair, max_pairs> buffer_{};
};

}  // namespace Pufferfish::Util::Containers

#include "OrderedMap.tpp"
