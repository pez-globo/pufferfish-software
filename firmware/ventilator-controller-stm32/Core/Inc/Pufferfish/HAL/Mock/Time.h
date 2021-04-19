/// Time.h
/// This file has mock class and methods for unit testing of Time.

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

#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish {
namespace HAL {
namespace Mock {

/**
 * A mock class for time
 */
class Time : public Interfaces::Time {
 public:
  /**
   * @brief Constructor for Time
   * @param None
   */
  Time() = default;

  /**
   * @brief  Set the millis value into private variable
   * @param  Input the millis value to set into private variable
   * @return None
   */
  void set_millis(uint32_t input);

  /**
   * @brief  Returns the millis value set
   * @param  None
   * @return millis value updated in millis_value_
   */
  uint32_t millis() override;

  /**
   * @brief  mock delay in milli
   * @param  ms dealy in milli seconds
   * @return None
   */
  void delay(uint32_t ms) override;

  /**
   * @brief  Set the micros value into private variable
   * @param  Input the micros value to set into private variable
   * @return None
   */
  void set_micros(uint32_t input);

  /**
   * @brief  Returns the micros value set
   * @param  None
   * @return micros value updated in micros_value_
   */
  uint32_t micros() override;

  /**
   * @brief mock delay in micros
   * @param microseconds dealy in micro seconds
   * @return None
   */
  void delay_micros(uint32_t microseconds) override;

 private:
  uint32_t micros_value_ = 0;
  uint32_t millis_value_ = 0;
};


}  // namespace Mock
}  // namespace HAL
}  // namespace Pufferfish
