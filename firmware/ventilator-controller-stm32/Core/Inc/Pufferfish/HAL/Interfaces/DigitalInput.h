/// DigitalInput.h
/// This file has interface class and methods for Digital Input.

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

namespace Pufferfish {
namespace HAL {

/**
 * Represents a GPIO input in STM32
 */
class DigitalInput {
 public:
  /**
   * Reads a digital input from the GPIO pin
   * @return true if the pin state is high, false if low
   */
  virtual bool read() = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
