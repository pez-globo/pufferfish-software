/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * LTC4015.h
 *
 *      Author: Rohan Purohit
 */

#pragma once

#include <array>

#include "Pufferfish/HAL/Interfaces/I2CDevice.h"

namespace Pufferfish::Driver::I2C::LTC4015 {
/**
 * A class for LTC4015 I2C device
 */
class I2CDevice {
 public:
  explicit I2CDevice(HAL::I2CDevice &dev) : dev_(dev) {}

  /**
   * Reads a data from the sensor
   *
   * @param buf[out] the buffer for the data output
   * @param address the address of the register we want to read from
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read(uint16_t address, std::array<uint8_t, sizeof(uint16_t)> &buf);

  /**
   * Writes command to the device
   * @param command the command to be sent
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus write(uint16_t command);

 private:
  HAL::I2CDevice &dev_;
};
}  // namespace Pufferfish::Driver::I2C::LTC4015
