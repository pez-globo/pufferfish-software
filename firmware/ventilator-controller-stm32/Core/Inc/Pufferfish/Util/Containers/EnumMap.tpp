/*
 * EnumMap.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "EnumMap.h"

#include <utility>

namespace Pufferfish::Util::Containers {

template <typename Key, typename Value, size_t max_key>
EnumMap<Key, Value, max_key>::EnumMap(std::initializer_list<std::pair<Key, Value>> init) {
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
    buffer_[i].first = false;
  }
  size_ = 0;
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::insert(const Key &key, const Value &value) {
  size_t index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return IndexStatus::out_of_bounds;
  }

  if (!buffer_[index].first) {
    ++size_;
    buffer_[index].first = true;
  }
  buffer_[index].second = value;
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::erase(const Key &key) {
  if (!has(key)) {
    return IndexStatus::out_of_bounds;
  }

  size_t index = static_cast<size_t>(key);
  if (buffer_[index].first) {
    --size_;
    buffer_[index].first = false;
  }
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
bool EnumMap<Key, Value, max_key>::has(const Key &key) const {
  size_t index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return false;
  }

  return buffer_[index].first;
}

template <typename Key, typename Value, size_t max_key>
IndexStatus EnumMap<Key, Value, max_key>::find(const Key &key, Value &value) const {
  if (!has(key)) {
    return IndexStatus::out_of_bounds;
  }

  value = buffer_[static_cast<size_t>(key)].second;
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_key>
const Value &EnumMap<Key, Value, max_key>::operator[](const Key &key) const {
  return buffer_[static_cast<size_t>(key)].second;
}

template <typename Key, typename Value, size_t max_key>
Value &EnumMap<Key, Value, max_key>::operator[](const Key &key) {
  return buffer_[static_cast<size_t>(key)].second;
}

}  // namespace Pufferfish::Util::Containers
