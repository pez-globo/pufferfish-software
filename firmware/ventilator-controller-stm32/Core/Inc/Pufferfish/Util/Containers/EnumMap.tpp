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

template <typename Key, typename Value, size_t max_key>
EnumMap<Key, Value, max_key>::EnumMap(std::initializer_list<std::pair<Key, Value>> init) noexcept {
  for (const auto &p : init) {
    insert(p.first, p.second);
  }
}

template <typename Key, typename Value, size_t max_key>
size_t EnumMap<Key, Value, max_key>::size() const {
  return size_;
}

template <typename Key, typename Value, size_t max_key>
bool EnumMap<Key, Value, max_key>::empty() const {
  return size_ == 0;
}

template <typename Key, typename Value, size_t max_key>
bool EnumMap<Key, Value, max_key>::full() const {
  return size_ == max_size();
}

template <typename Key, typename Value, size_t max_key>
size_t EnumMap<Key, Value, max_key>::available() const {
  return max_size() - size_;
}

template <typename Key, typename Value, size_t max_key>
void EnumMap<Key, Value, max_key>::clear() {
  for (size_t i = 0; i < max_size(); ++i) {
    occupancies_[i] = false;
  }
  size_ = 0;
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::insert(const Key &key, const Value &value) noexcept {
  auto index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return IndexStatus::out_of_bounds;
  }

  if (!occupancies_[index]) {
    ++size_;
    occupancies_[index] = true;
  }
  values_[index] = value;
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::erase(const Key &key) noexcept {
  if (!has(key)) {
    return IndexStatus::out_of_bounds;
  }

  auto index = static_cast<size_t>(key);
  if (occupancies_[index]) {
    --size_;
    occupancies_[index] = false;
  }
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
bool EnumMap<Key, Value, max_key>::has(const Key &key) const {
  auto index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return false;
  }

  return occupancies_[index];
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::find(const Key &key, Value &value) const {
  if (!has(key)) {
    return IndexStatus::out_of_bounds;
  }

  value = values_[static_cast<size_t>(key)];
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
const Value &EnumMap<Key, Value, max_key>::operator[](const Key &key) const noexcept {
  return values_[static_cast<size_t>(key)];
}

template <typename Key, typename Value, size_t max_key>
Value &EnumMap<Key, Value, max_key>::operator[](const Key &key) noexcept {
  return values_[static_cast<size_t>(key)];
}

}  // namespace Pufferfish::Util::Containers
