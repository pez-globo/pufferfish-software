/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Device.h
 *
 *      Author: Rohan Purohit
 *
 *  A driver for the LTC4015 buck battery charger controller
 */

#pragma once

#include <climits>

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Driver/I2C/LTC4015/I2CDevice.h"
#include "Pufferfish/HAL/Interfaces/I2CDevice.h"
#include "Types.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

static const uint16_t device_addr = 0xD0;
/**
 * Low-level driver for LTC4015 buck battery charger controller
 */
class Device {
 public:
  explicit Device(HAL::Interfaces::I2CDevice &dev) : i2cdevice_(dev) {}

  /**
   * Checks if the charger is enabled
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_charging_status(bool &charging_status);

  /**
   * Reads out the battery voltage
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_battery_voltage(uint16_t &v_bat);

  /**
   * Reads out the input voltage
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_input_voltage(uint16_t &v_in);

  /**
   * Reads out the system voltage
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_system_voltage(uint16_t &v_sys);

  /**
   * Reads out the battery current
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_battery_current(uint16_t &i_bat);

  /**
   * Reads out the input current
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_input_current(uint16_t &i_in);

 private:
  I2CDevice i2cdevice_;

  static constexpr float batt_voltage_conversion = 128.176;  // µV
  static constexpr float voltage_conversion = 1.648;         // mV
  static constexpr float current_conversion = 1.46487;       // µV/RSNSB
};

}  // namespace Pufferfish::Driver::I2C::LTC4015
