/// HALAnalogInput.h
/// This file has STM32 HAL class and methods for Analog Input.

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
#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

/**
 * Abstract class for HAL Analog input in STM32
 */
class HALAnalogInput : public AnalogInput {
 public:
  /**
   * @brief Constructs a new Analog Input
   * @param hadc and tTimeout
   */
  HALAnalogInput(ADC_HandleTypeDef &hadc, uint32_t t_timeout)
      : adc_input_(hadc), timeout_(t_timeout) {}

  /**
   * @brief Calls HAL_ADC_Start
   * @param  None
   * @return ADCStatus returns error/ok/busy
   */
  ADCStatus start() override;

  /**
   * @brief Reads a Analog conversion data
   * @param  AnalogDataRead, Out parameter returns the read data
   * @return ADCStatus returns error/ok/timeout
   */
  ADCStatus read(uint32_t &analog_data_read) override;

  /**
   * Calls HAL_ADC_Stop
   * @param  None
   * @return ADCStatus returns error/ok
   */
  ADCStatus stop() override;

 private:
  ADC_HandleTypeDef &adc_input_;
  uint32_t timeout_ = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
