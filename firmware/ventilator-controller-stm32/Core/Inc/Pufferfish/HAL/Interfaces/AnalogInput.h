/// AnalogInput.h
/// This file has interface class and methods for Analog Input.

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
 * Represents a Analog input in STM32
 */
class AnalogInput {
 public:
  /**
   * @brief Calls HAL_ADC_Start
   * @param  None
   * @return ADCStatus returns error/ok/busy
   */
  virtual ADCStatus start() = 0;

  /**
   * @brief Reads a Analog conversion data
   * @param  AnalogDataRead, Out parameter returns the read data
   * @return ADCStatus returns error/ok/timeout
   */
  virtual ADCStatus read(uint32_t &analog_data_read) = 0;

  /**
   * Calls HAL_ADC_Stop
   * @param  None
   * @return ADCStatus returns error/ok
   */
  virtual ADCStatus stop() = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
