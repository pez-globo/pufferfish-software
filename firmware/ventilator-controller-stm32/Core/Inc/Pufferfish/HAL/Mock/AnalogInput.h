/// MockAnalogInput.h
/// This file has mock class and methods for unit testing of Analog
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

#pragma once

#include "Pufferfish/HAL/Interfaces/AnalogInput.h"

namespace Pufferfish {
namespace HAL {
namespace Mock {

/**
 * Abstract class for HAL Analog input in STM32
 */
class AnalogInput : public Interfaces::AnalogInput {
 public:
  /**
   * @brief Constructs a new Analog Input
   * @param hadc and tTimeout
   */
  AnalogInput() = default;

  /**
   * @brief  Updates the mLastState to true
   * @param  None
   * @return ADCStatus returns ok
   */
  ADCStatus start() override;

  /**
   * @brief  Returns analog input data stored in lastInput
   * @param  AnalogDataRead, Out parameter returns the lastInput
   * @return ADCStatus returns ok
   */
  ADCStatus read(uint32_t &analog_data_read) override;

  /**
   * @brief  Updates the mLastState to false
   * @param  None
   * @return ADCStatus returns ok
   */
  ADCStatus stop() override;

  /**
   * @brief  Provides the state of Analog Input
   * @param  None
   * @return return state START(true)/STOP(false)
   */
  [[nodiscard]] bool get_analog_input_state() const;

  /**
   * @brief  Updates the lastInput with setAnalogDataRead
   * @param  setAnalogDataRead value to update the lastInput
   * @return None
   */
  void set_read(uint32_t set_analog_data_read);

  /**
   * @brief  updates the private variable set_return_status_ for mock test
   * @param  adc_status status to return from methods
   * @return None
   */
  void set_return_status(ADCStatus adc_status);

 private:
  /* State for Analog input START */
  bool last_state_ = false;
  /* Last Input data set */
  uint32_t last_input_ = 0;
  /* set return status */
  ADCStatus set_return_status_ = ADCStatus::ok;
};

} //namespace Mock
}  // namespace HAL
}  // namespace Pufferfish
