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

#include <iostream>

#include "Pufferfish/Util/Containers/Vector.h"
#include "catch2/catch.hpp"

namespace Pufferfish::HAL::Mock {
using Pufferfish::Util::Containers::ByteVector;

I2CDeviceStatus I2CDevice::read(uint8_t *buf, size_t count) {
  if (read_buf_queue_.empty()) {
    return I2CDeviceStatus::no_new_data;
  }

  REQUIRE(read_buf_queue_.size() == read_status_queue_.size());

  I2CDeviceStatus return_status = read_status_queue_.front();
  read_status_queue_.pop();

  const auto &read_buf = read_buf_queue_.front();

  REQUIRE(count == read_buf.size());

  for (size_t index = 0; index < read_buf_size; index++) {
    buf[index] = read_buf[index];
  }

  read_buf_queue_.pop();

  return return_status;
}

// TODO(lietk12): Add implementation of this method
// NOLINTNEXTLINE(misc-unused-parameters)
I2CDeviceStatus I2CDevice::read(uint16_t address, uint8_t *buf, size_t count) {
  return I2CDeviceStatus::ok;
}

void I2CDevice::add_read(const uint8_t *buf, size_t count, I2CDeviceStatus status) {
  read_status_queue_.push(status);

  size_t read_count = (count < read_buf_size) ? count : read_buf_size;

  read_buf_queue_.emplace();

  auto &read_buf = read_buf_queue_.back();

  for (size_t index = 0; index < read_count; index++) {
    read_buf.push_back(buf[index]);
  }
}

I2CDeviceStatus I2CDevice::write(uint8_t *buf, size_t count) {
  I2CDeviceStatus return_status = write_status_queue_.front();
  write_status_queue_.pop();

  write_buf_queue_.emplace();
  auto &write_buf = write_buf_queue_.back();

  size_t write_count = (count < write_buf_size) ? count : write_buf_size;
  for (size_t index = 0; index < write_count; index++) {
    write_buf.push_back(buf[index]);
  }

  REQUIRE(count == write_buf.size());

  return return_status;
}

I2CDeviceStatus I2CDevice::get_write(uint8_t *buf, size_t &count) {
  if (write_buf_queue_.empty()) {
    return I2CDeviceStatus::no_new_data;
  }

  const auto &write_buf = write_buf_queue_.front();
  count = write_buf.size();

  for (size_t index = 0; index < count; index++) {
    buf[index] = write_buf[index];
  }

  write_buf_queue_.pop();

  return I2CDeviceStatus::ok;
}

void I2CDevice::add_write_status(I2CDeviceStatus status) {
  write_status_queue_.push(status);
}

}  // namespace Pufferfish::HAL::Mock
