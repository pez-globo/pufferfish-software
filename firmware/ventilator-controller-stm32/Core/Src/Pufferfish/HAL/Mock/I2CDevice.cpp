/// I2CDevice.cpp
/// This file has methods for mock abstract interfaces for testing I2C
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

#include "Pufferfish/HAL/Mock/I2CDevice.h"

namespace Pufferfish::HAL::Mock {

I2CDeviceStatus I2CDevice::read(uint8_t *buf, size_t count) {
  I2CDeviceStatus return_status_;
  if (read_buf_queue_.empty()) {
    return I2CDeviceStatus::no_new_data;
  }

  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  const auto &return_status_buf = read_status_queue_.front();
  return_status_ = return_status_buf[index];

  if (return_status_ != I2CDeviceStatus::ok) {
    return return_status_;
  }

  const auto &read_buf = read_buf_queue_.front();
  for (index = 0; index < minumum; index++) {
    buf[index] = read_buf[index];
  }

  read_buf_queue_.pop();
  read_status_queue_.pop();

  return I2CDeviceStatus::ok;
}

// TODO(lietk12): Add implementation of this method
// NOLINTNEXTLINE(misc-unused-parameters)
I2CDeviceStatus I2CDevice::read(uint16_t address, uint8_t *buf, size_t count) {
  return I2CDeviceStatus::ok;
}

void I2CDevice::add_read(const uint8_t *buf, size_t count, I2CDeviceStatus &status) {
  size_t index = 0;
  size_t minumum = (count < read_buf_size) ? count : read_buf_size;

  read_buf_queue_.emplace();
  read_status_queue_.emplace();

  auto &read_buf = read_buf_queue_.back();
  auto &read_status_buf = read_status_queue_.back();
  for (index = 0; index < minumum; index++) {
    read_buf[index] = buf[index];
  }

  read_status_buf[index] = status;
}

I2CDeviceStatus return_status_;
I2CDeviceStatus I2CDevice::write(uint8_t *buf, size_t count) {
  size_t index = 0;
  size_t write_count = 0;

  write_status_queue_.emplace();
  auto &write_status_buf = write_status_queue_.back();
  write_status_buf[index] = get_return_status();
  return_status_ = write_status_buf[index];

  if (return_status_ != I2CDeviceStatus::ok) {
    return return_status_;
  }

  write_buf_queue_.emplace();
  auto &write_buf = write_buf_queue_.back();

  write_count = (count < write_buf_size) ? count : write_buf_size;
  for (index = 0; index < write_count; index++) {
    write_buf[index] = buf[index];
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus I2CDevice::get_write(uint8_t *buf, size_t &count) {
  if (write_buf_queue_.empty()) {
    return I2CDeviceStatus::no_new_data;
  }

  size_t index = 0;
  const auto &write_buf = write_buf_queue_.front();
  const auto &return_status_buf = write_status_queue_.front();
  return_status_ = return_status_buf[index];

  if (return_status_ != I2CDeviceStatus::ok) {
    return return_status_;
  }

  count = write_buf.size();
  for (index = 0; index < count; index++) {
    buf[index] = write_buf[index];
  }

  write_buf_queue_.pop();
  write_status_queue_.pop();

  return I2CDeviceStatus::ok;
}

void I2CDevice::set_return_status(I2CDeviceStatus input) {
  return_status_ = input;
}

I2CDeviceStatus I2CDevice::get_return_status() {
  return return_status_;
}

}  // namespace Pufferfish::HAL::Mock
