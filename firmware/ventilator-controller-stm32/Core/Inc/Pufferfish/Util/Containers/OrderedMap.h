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
 *  as lookups are on average O(m).
 */

#pragma once

#include <cstddef>
#include <initializer_list>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/Containers/Vector.h"

namespace Pufferfish::Util::Containers {

template <typename Key, typename Value, size_t max_pairs>
class OrderedMap {
 public:
  using Pair = std::pair<Key, Value>;

  OrderedMap() = default;
  // Construct the OrderedMap with an initial set of key-value pairs, given as an
  // initializer list (e.g. OrderedMap map{{k1, v1}, {k2, v2}, {k3, v3}};)
  // Note: you must ensure that keys are unique; the value for the last copy of a duplicated key
  // will overwrite all previous values. Note: this makes copies of the values!
  OrderedMap(std::initializer_list<Pair> init) : buffer_(init) {}

  [[nodiscard]] size_t size() const;
  [[nodiscard]] static constexpr size_t max_size() noexcept { return max_pairs; }
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool full() const;
  [[nodiscard]] size_t available() const;

  // Note: this makes a copy of value!
  IndexStatus input(const Key &key, const Value &value);  // O(m)
  // Note: this copies the value in the map to the value output parameter!
  IndexStatus output(const Key &key, Value &value) const;  // O(m)
  void clear();                                            // O(1)
  IndexStatus erase(const Key &key);                       // O(m)
  [[nodiscard]] bool has(const Key &key) const;            // O(m)

  [[nodiscard]] constexpr const Vector<Pair, max_pairs> &items() const { return buffer_; }
  constexpr Vector<Pair, max_pairs> &items() { return buffer_; }

 private:
  Vector<Pair, max_pairs> buffer_{};

  IndexStatus find(const Key &key, size_t &index) const;  // O(m)
};

}  // namespace Pufferfish::Util::Containers

#include "OrderedMap.tpp"
