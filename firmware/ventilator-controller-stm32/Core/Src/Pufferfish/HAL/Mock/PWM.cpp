/// PWM.cpp
/// This file has methods for mock abstract interfaces for testing PWM.

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

#include <Pufferfish/HAL/Mock/PWM.h>

namespace Pufferfish::HAL::Mock {

void PWM::set_duty_cycle_raw(uint32_t duty) {
  last_raw_duty_ = duty;
}

float PWM::get_duty_cycle_raw() const {
  return last_raw_duty_;
}

PWMStatus PWM::start() {
  if (return_status_ != PWMStatus::ok) {
    return PWMStatus::hal_error;
  }
  state_ = true;
  return PWMStatus::ok;
}

bool PWM::get_pwm_state() const {
  return state_;
}
PWMStatus PWM::stop() {
  if (return_status_ != PWMStatus::ok) {
    return PWMStatus::hal_error;
  }
  state_ = false;
  return PWMStatus::ok;
}

uint32_t PWM::get_max_duty_cycle() {
  return last_max_duty_;
}

void PWM::set_max_duty_cycle(uint32_t duty) {
  last_max_duty_ = duty;
}

void PWM::set_return_status(PWMStatus input) {
  return_status_ = input;
}

}  // namespace Pufferfish::HAL::Mock
