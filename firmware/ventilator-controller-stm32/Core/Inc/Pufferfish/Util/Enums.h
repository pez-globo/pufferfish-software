/*
 * Enums.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  Utilities for working with enums.
 */

#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <type_traits>

namespace Pufferfish::Util {

// EnumValues is inspired by https://stackoverflow.com/a/33091821 by user janm,
// but the implementation of the variadic template recursion is different

template <typename Enum, Enum... values>
class EnumValues;

template <typename Enum>
class EnumValues<Enum> {
 public:
  using EnumType = Enum;
  using UnderlyingType = typename std::underlying_type<Enum>::type;

  static bool constexpr includes(UnderlyingType /*test_value*/) { return false; }
  static bool constexpr includes(EnumType /*test_value*/) { return false; }

  static UnderlyingType constexpr max() { return std::numeric_limits<UnderlyingType>::min(); }
};

template <typename Enum, Enum first_value, Enum... remaining_values>
class EnumValues<Enum, first_value, remaining_values...> {
 public:
  using EnumType = Enum;
  using UnderlyingType = typename std::underlying_type<Enum>::type;

  static bool constexpr includes(UnderlyingType test_value) {
    return test_value == static_cast<UnderlyingType>(first_value) ||
           EnumValues<Enum, remaining_values...>::includes(test_value);
  }
  static bool constexpr includes(EnumType test_value) {
    return test_value == first_value || EnumValues<Enum, remaining_values...>::includes(test_value);
  }

  static UnderlyingType constexpr max() {
    return std::max(
        static_cast<UnderlyingType>(first_value), EnumValues<Enum, remaining_values...>::max());
  }
};

}  // namespace Pufferfish::Util
