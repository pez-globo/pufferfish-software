/// PWM.h
/// This file has interface class and methods for PWM.

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

#include "Pufferfish/Statuses.h"

namespace Pufferfish {
namespace HAL {

/**
 * A generic PWM driver
 */
class PWM {
 public:
  /**
   * Set a duty cycle of PWM, can be done when PWM is active
   * this function does NOT start the PWM output
   * @param duty    a number between 0.0 and 1.0 (inclusive) for the desired
   * duty cycle
   */
  PWMStatus set_duty_cycle(float duty);

  /**
   * Set a duty cycle of PWM, can be done when PWM is active
   * this function does NOT start the PWM output.
   * This is faster than setDutyCycle() as no floating point calculation is done
   * @param duty    an integer between 0 and getMaxDutyCycle() (inclusive) for
   * the desired duty cycle
   */
  virtual void set_duty_cycle_raw(uint32_t duty) = 0;

  /**
   * Start the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  virtual PWMStatus start() = 0;

  /**
   * Stop the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  virtual PWMStatus stop() = 0;

  /**
   * Returns the maximum duty cycle that can be set with setDutyCycleRaw()
   * @return the maximum duty cycle
   */
  virtual uint32_t get_max_duty_cycle() = 0;
};

} /* namespace HAL */
} /* namespace Pufferfish */
