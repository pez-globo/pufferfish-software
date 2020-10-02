/// Time.h
/// This file has interface class and methods for time.

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

#include <cstdint>

namespace Pufferfish {
namespace HAL {

/**
 * An abstract class for time
 */
class Time {
 public:
  /**
   * Returns the number of millisecond since the startup,
   *  will be rolled over every 50 days
   * @returnthe millisecond since start
   */
  virtual uint32_t millis() = 0;

  /**
   * Block the execution for the given number of millisecond
   *  This function provide delay in the range of [ms, ms + 1)
   * @param ms the number of millisecond to delay
   */
  virtual void delay(uint32_t ms) = 0;

  /**
   * Returns the number of microsecond since the startup,
   * will be rolled over every around 67 seconds with 64 MHz system clock
   * @return the number of microsecond
   */
  virtual uint32_t micros() = 0;

  /**
   * Block the execution for the provided microseconds
   *  The delay provided should be relatively accurate [us, us + 3)
   * @param microseconds the number of microseconds to delay,
   *  must be less than 67 seconds with 64 MHz system clock
   */
  virtual void delay_micros(uint32_t microseconds) = 0;
};

} /* namespace HAL */
} /* namespace Pufferfish */
