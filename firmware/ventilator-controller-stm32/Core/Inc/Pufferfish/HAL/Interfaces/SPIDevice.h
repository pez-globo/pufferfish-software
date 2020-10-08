/// SPIDevice.h
/// This file has interface class and methods for SPI Device.

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
 * An abstract class which represents an SPI device with read/write interface
 */
class SPIDevice {
 public:
  /**
   * Reads data from the device
   * @param buf[out]    output of the data
   * @param count   the number of bytes to be read
   * @return ok on success, error code otherwise
   */
  virtual SPIDeviceStatus read(uint8_t *buf, size_t count) = 0;

  /**
   * Writes data to the device
   * @param buf the data to be written
   * @param count the number of bytes to write
   * @return ok on success, error code otherwise
   */
  virtual SPIDeviceStatus write(uint8_t *buf, size_t count) = 0;

  /**
   * Write and Read data to and from the device
   * @param txBuf pointer to transmission data buffer
   * @param rxBuf pointer to reception data buffer
   * @param count amount of data to be sent and received
   * @return ok on success
   */
  virtual SPIDeviceStatus write_read(uint8_t *tx_buf, uint8_t *rx_buf, size_t count) = 0;

  /**
   * To make the chip select as high or low
   * @param cs true(high) or false(low)
   */
  virtual void chip_select(bool input) = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
