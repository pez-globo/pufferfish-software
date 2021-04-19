/// MockPWM.cpp
/// This file has methods for mock abstract interfaces for testing Time
/// related methods.

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

#include <Pufferfish/HAL/Mock/Time.h>

namespace Pufferfish::HAL::Mock {

void Time::set_millis(uint32_t input) {
  millis_value_ = input;
}

uint32_t Time::millis() {
  return millis_value_;
}

void Time::delay(uint32_t ms) {
  for (volatile uint32_t value = 0; value < ms; value++) {
    /* do nothing */
  }
}

void Time::set_micros(uint32_t input) {
  micros_value_ = input;
}

uint32_t Time::micros() {
  return micros_value_;
}

void Time::delay_micros(uint32_t microseconds) {
  for (volatile uint32_t value = 0; value < microseconds; value++) {
    /* do nothing */
  }
}

}  // namespace Pufferfish::HAL::Mock
