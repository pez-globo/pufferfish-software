/// MockBufferedUART.h
/// This file has mock class and methods for unit testing of PWM.

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

#include "Pufferfish/HAL/Interfaces/PWM.h"

namespace Pufferfish {
namespace HAL {
namespace Mock {

/**
 * A generic PWM driver
 */
class PWM : public Interfaces::PWM {
 public:
  /**
   * Constructs a new PWM object
   * @param None
   */
  PWM() = default;

  /**
   * Set a duty cycle of PWM, updates the private variable mLastRawDuty
   * @param duty    a number between 0.0 and 1.0 (inclusive) for the desired
   * duty cycle
   */
  void set_duty_cycle_raw(uint32_t duty) override;

  /**
   * @brief  Get a duty cycle of PWM updated in setDutyCycleRaw
   * @param  None
   * @return returns the duty cycle
   */
  [[nodiscard]] float get_duty_cycle_raw() const;

  /**
   * Start the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  PWMStatus start() override;

  /**
   * Stop the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  PWMStatus stop() override;

  /**
   * Returns the maximum duty cycle updated in setMaxDutyCycle
   * @return the maximum duty cycle
   */
  uint32_t get_max_duty_cycle() override;

  /**
   * @brief  sets the max duty cycle
   * @param  duty    Provides the max duty cycle
   * @return None
   */
  void set_max_duty_cycle(uint32_t duty);

  /**
   * @brief  Provides the state of PWM
   * @param  None
   * @return State of PWM START(true)/STOP(false)
   */
  [[nodiscard]] bool get_pwm_state() const;

  /**
   * @brief  sets return_status_ private variable
   * @param  input PWMStatus
   * @return None
   */
  void set_return_status(PWMStatus input);

 private:
  /* Return status */
  PWMStatus return_status_ = PWMStatus::ok;
  /* Stores the Last raw duty cycle value */
  uint32_t last_raw_duty_ = 0;
  /* Stores the last Max duty cycle value */
  uint32_t last_max_duty_ = 0;

  bool state_ = false;
};


} /* namespace Mock */
} /* namespace HAL */
} /* namespace Pufferfish */
