/*
 * EnumMap.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <utility>

#include "EnumMap.h"

namespace Pufferfish::Util::Containers {

template <typename Key, typename Value, size_t capacity>
EnumMap<Key, Value, capacity>::EnumMap(std::initializer_list<std::pair<Key, Value>> init) noexcept {
  for (const auto &p : init) {
    input(p.first, p.second);
  }
}

template <typename Key, typename Value, size_t capacity>
size_t EnumMap<Key, Value, capacity>::size() const {
  return keys_.size();
}

template <typename Key, typename Value, size_t capacity>
bool EnumMap<Key, Value, capacity>::empty() const {
  return keys_.empty();
}

template <typename Key, typename Value, size_t capacity>
bool EnumMap<Key, Value, capacity>::full() const {
  return keys_.full();
}

template <typename Key, typename Value, size_t capacity>
size_t EnumMap<Key, Value, capacity>::available() const {
  return keys_.available();
}

template <typename Key, typename Value, size_t capacity>
IndexStatus EnumMap<Key, Value, capacity>::input(const Key &key, const Value &value) noexcept {
  IndexStatus status = keys_.input(key);
  if (status != IndexStatus::ok) {
    return status;
  }

  values_[keys_.index_of(key)] = value;
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t capacity>
IndexStatus EnumMap<Key, Value, capacity>::output(const Key &key, Value &value) const {
  if (!has(key)) {
    return IndexStatus::out_of_bounds;
  }

  value = values_[keys_.index_of(key)];
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t capacity>
void EnumMap<Key, Value, capacity>::clear() {
  keys_.clear();
}

template <typename Key, typename Value, size_t capacity>
IndexStatus EnumMap<Key, Value, capacity>::erase(const Key &key) noexcept {
  return keys_.erase(key);
}

template <typename Key, typename Value, size_t capacity>
bool EnumMap<Key, Value, capacity>::has(const Key &key) const {
  return keys_.has(key);
}

template <typename Key, typename Value, size_t capacity>
const Value &EnumMap<Key, Value, capacity>::operator[](const Key &key) const noexcept {
  return values_[static_cast<size_t>(key)];
}

template <typename Key, typename Value, size_t capacity>
Value &EnumMap<Key, Value, capacity>::operator[](const Key &key) noexcept {
  return values_[static_cast<size_t>(key)];
}

}  // namespace Pufferfish::Util::Containers
