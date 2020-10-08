/// PWM.cpp
/// This file has methods to interface with PWM class.

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

#include "Pufferfish/HAL/Interfaces/PWM.h"

namespace Pufferfish::HAL {

PWMStatus PWM::set_duty_cycle(float duty) {
  if (duty < 0.0 || 1.0 < duty) {
    return PWMStatus::invalid_duty_cycle;
  }

  set_duty_cycle_raw(static_cast<uint32_t>(duty * get_max_duty_cycle()));
  return PWMStatus::ok;
}

}  // namespace Pufferfish::HAL
