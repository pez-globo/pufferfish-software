/// MockPWM.cpp
/// This file has methods for mock abstract interfaces for testing SPI
/// Device.

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

#include "Pufferfish/HAL/Mock/MockSPIDevice.h"

namespace Pufferfish::HAL {

SPIDeviceStatus MockSPIDevice::read(uint8_t *buf, size_t count) {
  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  if (return_status_ != SPIDeviceStatus::ok) {
    return SPIDeviceStatus::read_error;
  }

  for (index = 0; index < minumum; index++) {
    buf[index] = read_buf_[index];
  }
  return SPIDeviceStatus::ok;
}

void MockSPIDevice::set_read(const uint8_t *buf, size_t count) {
  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  for (index = 0; index < minumum; index++) {
    read_buf_[index] = buf[index];
  }
}

SPIDeviceStatus MockSPIDevice::write(uint8_t *buf, size_t count) {
  size_t index = 0;

  if (return_status_ != SPIDeviceStatus::ok) {
    return SPIDeviceStatus::write_error;
  }

  write_count_ = (count < write_buf_size) ? count : write_buf_size;
  for (index = 0; index < write_count_; index++) {
    write_buf_[index] = buf[index];
  }

  return SPIDeviceStatus::ok;
}

void MockSPIDevice::get_write(uint8_t *buf, size_t &count) {
  size_t index = 0;

  count = write_count_;
  for (index = 0; index < count; index++) {
    buf[index] = write_buf_[index];
  }
}

void MockSPIDevice::set_write_read(const uint8_t *buf, size_t count) {
  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  for (index = 0; index < minumum; index++) {
    set_write_read_buf_[index] = buf[index];
  }
}

SPIDeviceStatus MockSPIDevice::write_read(uint8_t *tx_buf, uint8_t *rx_buf, size_t count) {
  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  if (return_status_ != SPIDeviceStatus::ok) {
    return SPIDeviceStatus::read_error;
  }

  for (index = 0; index < minumum; index++) {
    rx_buf[index] = set_write_read_buf_[index];
  }

  get_write_read_count_ = (count < write_buf_size) ? count : write_buf_size;
  for (index = 0; index < write_count_; index++) {
    get_write_read_buf_[index] = tx_buf[index];
  }

  return SPIDeviceStatus::ok;
}

void MockSPIDevice::get_write_read(uint8_t *buf, size_t count) {
  size_t index = 0;

  count = get_write_read_count_;
  for (index = 0; index < count; index++) {
    buf[index] = get_write_read_buf_[index];
  }
}

void MockSPIDevice::chip_select(bool input) {
  last_cs_ = input;
}

bool MockSPIDevice::get_chip_select() const {
  return last_cs_;
}

void MockSPIDevice::set_return_status(SPIDeviceStatus input) {
  return_status_ = input;
}

}  // namespace Pufferfish::HAL
