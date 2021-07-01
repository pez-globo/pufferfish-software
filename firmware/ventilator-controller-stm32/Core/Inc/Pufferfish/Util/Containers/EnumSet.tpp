/*
 * EnumSet.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <utility>

#include "EnumSet.h"

namespace Pufferfish::Util::Containers {

template <typename Key, size_t capacity>
EnumSet<Key, capacity>::EnumSet(std::initializer_list<Key> init) noexcept {
  for (const auto &key : init) {
    input(key);
  }
}

template <typename Key, size_t capacity>
size_t EnumSet<Key, capacity>::size() const {
  return size_;
}

template <typename Key, size_t capacity>
bool EnumSet<Key, capacity>::empty() const {
  return size_ == 0;
}

template <typename Key, size_t capacity>
bool EnumSet<Key, capacity>::full() const {
  return size_ == max_size();
}

template <typename Key, size_t capacity>
size_t EnumSet<Key, capacity>::available() const {
  return max_size() - size_;
}

template <typename Key, size_t capacity>
IndexStatus EnumSet<Key, capacity>::input(const Key &key) noexcept {
  auto index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return IndexStatus::out_of_bounds;
  }

  if (!occupancies_[index]) {
    ++size_;
    occupancies_[index] = true;
  }
  return IndexStatus::ok;
}

template <typename Key, size_t capacity>
void EnumSet<Key, capacity>::clear() {
  for (size_t i = 0; i < max_size(); ++i) {
    occupancies_[i] = false;
  }
  size_ = 0;
}

template <typename Key, size_t capacity>
IndexStatus EnumSet<Key, capacity>::erase(const Key &key) noexcept {
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

template <typename Key, size_t capacity>
bool EnumSet<Key, capacity>::has(const Key &key) const {
  auto index = static_cast<size_t>(key);
  if (index >= max_size()) {
    return false;
  }

  return occupancies_[index];
}

}  // namespace Pufferfish::Util::Containers
