/// MockBufferedUART.h
/// This file has mock class and methods for unit testing of Digital
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

#include "Pufferfish/HAL/Interfaces/DigitalInput.h"

namespace Pufferfish {
namespace HAL {

/**
 * Represents a GPIO input in STM32
 */
class MockDigitalInput : public DigitalInput {
 public:
  /**
   * Constructs a new Digital Input
   * @param None
   */
  MockDigitalInput() = default;

  /**
   * Reads a digital input from the GPIO pin
   * @return TRUE/FALSE based on the value set on setRead
   */
  bool read() override;

  /*
   * @brief  Test method to set value to be returned by the read method
   * @param  setting - Set TRUE/FASLE to read
   * @return None
   */
  void set_read(bool setting);

 private:
  bool last_input_ = false;
};

}  // namespace HAL
}  // namespace Pufferfish
