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

  ~TaggedUnion() = default;
  TaggedUnion() = default;
  TaggedUnion(const TaggedUnion&) = default;
  TaggedUnion(TaggedUnion&&) noexcept = default;
  TaggedUnion& operator=(TaggedUnion&&) noexcept = default;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
  TaggedUnion& operator=(const TaggedUnion& other) noexcept = default;

  template <typename Value>
  void set(const Value& new_value);

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes);
  TagValue tag;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes);
  Union value;
};

}  // namespace Pufferfish::Util
