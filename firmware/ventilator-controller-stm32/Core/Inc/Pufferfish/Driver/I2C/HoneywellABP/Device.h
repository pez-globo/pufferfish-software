/*
 * Original work Copyright 2018, ij96
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project
 * contributors
 *
 *  Honeywell ABP sensor driver, modified from
 *    Arduino library by ij96 (https://github.com/ij96)
 */

#pragma once

#include "Pufferfish/HAL/Interfaces/I2CDevice.h"
#include "Types.h"

namespace Pufferfish::Driver::I2C::HoneywellABP {

/**
 * Driver for Honeywell ABP pressure sensor
 */
class Device {
 public:
  Device(HAL::Interfaces::I2CDevice &dev, const ABPConfig &cfg)
      : dev_(dev), pmin(cfg.pmin), pmax(cfg.pmax), unit(cfg.unit) {}

  [[nodiscard]] float raw_to_pressure(uint16_t output) const;

  /**
   * Reads out the pressure from the sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_sample(Sample &sample);

 private:
  static const uint8_t status_shift = 6;
  // sensor 14-bit output range
  static const uint16_t output_min = 0x0666;  // 10% of 2^14
  static const uint16_t output_max = 0x399A;  // 90% of 2^14

  Pufferfish::HAL::Interfaces::I2CDevice &dev_;

  // pressure range (refer to datasheet)
  const float pmin;  // minimum pressure
  const float pmax;  // maximum pressure
  const PressureUnit unit;
};

}  // namespace Pufferfish::Driver::I2C::HoneywellABP
