/// MockBufferedUART.h
/// This file has mock class and methods for unit testing of Digital
/// Output.

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

#include "Pufferfish/HAL/Interfaces/DigitalOutput.h"

namespace Pufferfish {
namespace HAL {

/**
 * Represents a GPIO output in STM32, for mock functional testing
 */
class MockDigitalOutput : public DigitalOutput {
 public:
  /**
   * Constructs a new DigitalOutput
   * @param None
   */
  MockDigitalOutput() = default;

  /**
   * Writes a digital output to the GPIO pin,
   * @param set the output value to lastOutput
   */
  void write(bool output) override;

  /**
   * Test method to get value passed on write method
   */
  [[nodiscard]] bool get_write() const;

 private:
  bool last_output_ = false;
};

}  // namespace HAL
}  // namespace Pufferfish
