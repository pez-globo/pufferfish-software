/// MockBufferedUART.h
/// This file has mock class and methods for unit testing of SPI Device.

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

#include <array>

#include "Pufferfish/HAL/Interfaces/SPIDevice.h"

namespace Pufferfish {
namespace HAL {
namespace Mock {

/**
 * An abstarct class for SPI device
 */
class SPIDevice : public Interfaces::SPIDevice {
 public:
  // maximum default time to wait for response from SPI, in ms
  const static uint32_t default_timeout = 2U;

  /**
   * Constructs an Mock SPI object
   */
  SPIDevice() = default;

  /**
   * @brief  Read method to read data from private buffer variable mReadBuf
   * @param  buf returns the data stored in setRead method
   * @param  count size of data to set
   * @return returns SPIDeviceStatus as ok
   */
  SPIDeviceStatus read(uint8_t *buf, size_t count) override;

  /**
   * @brief  Updates the private buffer variable mReadBuf with the input data
   * @param  buf update the private variable mReadBuf with the buffer input
   * @param  count size of data to set
   * @return None
   */
  void set_read(const uint8_t *buf, size_t count);

  /**
   * @brief  Writes data to the device
   * @param buf the data to be written
   * @param count the number of bytes to write
   * @return returns SPIDeviceStatus as ok
   */
  SPIDeviceStatus write(uint8_t *buf, size_t count) override;

  /**
   * @brief  Reads private buffer variable mWriteBuf and updates to buf
   * @param  buf returns the data stored in write method
   * @param  count buffer size to return
   * @return None
   */
  void get_write(uint8_t *buf, size_t &count);

  /**
   * @brief  Updates the private buffer variable mSetWriteReadBuf with the input
   * data
   * @param  buf update the private variable mSetWriteReadBuf with the buffer
   * input
   * @param  count size of data to set
   * @return None
   */
  void set_write_read(const uint8_t *buf, size_t count);

  /**
   * @brief  Write and Read data to and from the device
   * @param  txBuf pointer to transmission data buffer
   * @param  rxBuf pointer to reception data buffer
   * @param  count amount of data to be sent and received
   * @return returns SPIDeviceStatus as ok
   */
  SPIDeviceStatus write_read(uint8_t *tx_buf, uint8_t *rx_buf, size_t count) override;

  /**
   * @brief  Reads private buffer variable mGetWriteReadBuf and updates to buf
   * @param  buf returns the data stored in writeRead method
   * @param  count buffer size to return
   * @return None
   */
  void get_write_read(uint8_t *buf, size_t count);

  /**
   * @brief  Sets the mLastCS
   * @param  true(high) or false(low)
   * @return None
   */
  void chip_select(bool input) override;

  /**
   * @brief  Gets the last chip select input data
   * @param  None
   * @return returns mLastCS input data
   */
  [[nodiscard]] bool get_chip_select() const;

  /**
   * @brief  sets the return status
   * @param  input the SPIDeviceStatus
   * @return None
   */
  void set_return_status(SPIDeviceStatus input);

 private:
  /* Last chip select input */
  bool last_cs_ = false;

  /* return status */
  SPIDeviceStatus return_status_ = SPIDeviceStatus::ok;

  static const uint8_t read_buf_size = 50;
  static const uint8_t write_buf_size = 50;

  std::array<uint8_t, read_buf_size> read_buf_{};
  std::array<uint8_t, write_buf_size> write_buf_{};

  std::array<uint8_t, read_buf_size> set_write_read_buf_{};
  std::array<uint8_t, write_buf_size> get_write_read_buf_{};
  size_t write_count_{};
  size_t get_write_read_count_{};
};

}  // namespace Mock
}  // namespace HAL
}  // namespace Pufferfish
