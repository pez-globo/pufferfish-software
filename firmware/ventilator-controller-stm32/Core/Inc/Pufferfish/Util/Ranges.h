/// \file
/// \brief Convenience functions for working with ranges of numbers
///
/// Functions for clamping within ranges

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <algorithm>

namespace Pufferfish {
namespace Util {

/**
 * Checks whether the value is within the provided range
 * @param value the number to clamp
 * @param floor the minimum allowed value (inclusive)
 * @param ceiling the maximum allowed value (inclusive
 * @return true if the value is in range, false otherwise
 */
template <typename Numeric>
constexpr bool within(Numeric value, Numeric floor, Numeric ceiling) {
  return value >= floor && value <= ceiling;
}

/**
 * Clamps the value to be within the provided range
 *
 * Note: overrides NaNs
 * @param value the number to clamp
 * @param floor the minimum allowed value
 * @param ceiling the maximum allowed value
 * @return the value if it's within the range, or the nearest floor or ceiling
 */
template <typename Numeric>
constexpr Numeric clamp(Numeric value, Numeric floor, Numeric ceiling) {
  return std::min(ceiling, std::max(floor, value));
}

}  // namespace Util
}  // namespace Pufferfish
