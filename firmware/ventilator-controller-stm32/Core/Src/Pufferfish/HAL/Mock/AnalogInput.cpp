/// AnalogInput.cpp
/// This file has methods for mock abstract interfaces for testing analog
/// Input.

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

#include <Pufferfish/HAL/Mock/AnalogInput.h>

namespace Pufferfish::HAL::Mock {

ADCStatus AnalogInput::start() {
  if (set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  last_state_ = true;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}

ADCStatus AnalogInput::read(uint32_t &analog_data_read) {
  if (set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  analog_data_read = last_input_;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}

void AnalogInput::set_read(uint32_t set_analog_data_read) {
  /* Update the lastInput with input analog data read value */
  last_input_ = set_analog_data_read;
}

ADCStatus AnalogInput::stop() {
  if (set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  last_state_ = false;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}
void AnalogInput::set_return_status(ADCStatus adc_status) {
  set_return_status_ = adc_status;
}

bool AnalogInput::get_analog_input_state() const {
  return last_state_;
  ;
}
}  // namespace Pufferfish::HAL::Mock
