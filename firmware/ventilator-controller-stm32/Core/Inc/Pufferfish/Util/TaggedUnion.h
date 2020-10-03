/*
 * Tagged Union.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  Utilities for working with unions slightly more safely.
 */

#pragma once

#include <array>

#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util {

template <typename Union, typename TagValue>
class TaggedUnion {
 public:
  using Tag = TagValue;

  template <typename Value>
  void set(const Value &new_value);

  TagValue tag;
  Union value;
};

}  // namespace Pufferfish::Util
