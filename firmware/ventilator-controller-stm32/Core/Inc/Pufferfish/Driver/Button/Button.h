/// Button.h
/// Methods and classes for membrane buttons debounce calculation

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

#include "Pufferfish/Application/Debouncing.h"
#include "Pufferfish/HAL/Interfaces/DigitalInput.h"
#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Statuses.h"

namespace Pufferfish::Driver::Button {

/**
 * Abstract class for Membrane button input
 */
class Button {
 public:
  using Status = Application::Debouncer::Status;

  Button(HAL::DigitalInput &buttoninput, Application::Debouncer &debounce, HAL::Time &time)
      : button_input_(buttoninput), debounce_(debounce), time_(time) {}

  /**
   * Read button state
   * @param debounedOutput debounced output state
   * @param EdgeState rising edge on Low to High or falling edge on High to Low
   * @return rising edge on Low to High or falling edge on High to Low
   */
  Status read_state(bool &debounced_output, Application::EdgeDetector::State &switch_state_changed);

 private:
  HAL::DigitalInput &button_input_;
  Application::Debouncer &debounce_;
  HAL::Time &time_;
  Application::EdgeDetector edge_detect_;
};

}  // namespace Pufferfish::Driver::Button
