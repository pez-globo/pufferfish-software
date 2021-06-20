/*
 * OrderedMap.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "OrderedMap.h"

namespace Pufferfish::Util::Containers {

template <typename Key, typename Value, size_t max_pairs>
size_t OrderedMap<Key, Value, max_pairs>::size() const {
  return buffer_.size();
}

template <typename Key, typename Value, size_t max_pairs>
bool OrderedMap<Key, Value, max_pairs>::empty() const {
  return buffer_.empty();
}

template <typename Key, typename Value, size_t max_pairs>
bool OrderedMap<Key, Value, max_pairs>::full() const {
  return buffer_.full();
}

template <typename Key, typename Value, size_t max_pairs>
size_t OrderedMap<Key, Value, max_pairs>::available() const {
  return buffer_.available();
}

template <typename Key, typename Value, size_t max_pairs>
IndexStatus OrderedMap<Key, Value, max_pairs>::input(const Key &key, const Value &value) {
  size_t index = 0;
  if (find(key, index) == IndexStatus::ok) {
    buffer_[index].second = value;
    return IndexStatus::ok;
  }

  Pair pair{key, value};
  return buffer_.push_back(pair);
}

template <typename Key, typename Value, size_t max_pairs>
IndexStatus OrderedMap<Key, Value, max_pairs>::output(const Key &key, Value &value) const {
  size_t index = 0;
  IndexStatus status = find(key, index);
  if (status != IndexStatus::ok) {
    return status;
  }

  value = buffer_[index].second;
  return IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_pairs>
void OrderedMap<Key, Value, max_pairs>::clear() {
  buffer_.clear();
}

template <typename Key, typename Value, size_t max_pairs>
IndexStatus OrderedMap<Key, Value, max_pairs>::erase(const Key &key) {
  size_t index = 0;
  if (find(key, index) == IndexStatus::ok) {
    return buffer_.erase(index);
  }

  return IndexStatus::out_of_bounds;
}

template <typename Key, typename Value, size_t max_pairs>
bool OrderedMap<Key, Value, max_pairs>::has(const Key &key) const {
  size_t index = 0;
  return find(key, index) == IndexStatus::ok;
}

template <typename Key, typename Value, size_t max_pairs>
IndexStatus OrderedMap<Key, Value, max_pairs>::find(const Key &key, size_t &index) const {
  for (index = 0; index < size(); ++index) {
    if (buffer_[index].first == key) {
      return IndexStatus::ok;
    }
  }

  return IndexStatus::out_of_bounds;
}

}  // namespace Pufferfish::Util::Containers
