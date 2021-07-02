/*
 * Set.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  Abstract interface for sets
 */

#pragma once

#include <array>

namespace Pufferfish::Util::Containers {

template <typename Key>
class Set {
 public:
  [[nodiscard]] virtual bool has(const Key &key) const = 0;
};

}  // namespace Pufferfish::Util::Containers
