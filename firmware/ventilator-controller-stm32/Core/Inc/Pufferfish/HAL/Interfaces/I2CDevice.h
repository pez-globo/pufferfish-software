/// I2CDevice.h
/// This file has interface class and methods for I2C Device.

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

#include <cstddef>
#include <cstdint>

#include "Pufferfish/Statuses.h"

namespace Pufferfish {
namespace HAL {

/**
 * An abstract class which represents an arbitrary I2C device with read/write
 * interface
 */
class I2CDevice {
 public:
  /**
   * Reads data from the device
   * @param buf[out]    output of the data
   * @param count   the number of bytes to be read
   * @return ok on success, error code otherwise
   */
  virtual I2CDeviceStatus read(uint8_t *buf, size_t count) = 0;

  /**
   * Writes data to the device
   * @param buf the data to be written
   * @param count the number of bytes to write
   * @return ok on success, error code otherwise
   */
  virtual I2CDeviceStatus write(uint8_t *buf, size_t count) = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
