/*
 * Original work Copyright 2018, ij96
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project
 * contributors
 *
 *  Honeywell ABP sensor driver, modified from
 *    Arduino library by ij96 (https://github.com/ij96)
 */

#pragma once

#include "Pufferfish/Driver/Testable.h"
#include "Pufferfish/HAL/Interfaces/I2CDevice.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {
namespace HoneywellABP {

/**
 * Driver for Honeywell ABP pressure sensor
 */
class Device : public Testable {
 public:
  Device(HAL::Interfaces::I2CDevice &dev, const ABPConfig &cfg)
      : dev_(dev), pmin(cfg.pmin), pmax(cfg.pmax), unit(cfg.unit) {}

  [[nodiscard]] float raw_to_pressure(uint16_t output) const;

  /**
   * Reads out the pressure from the sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_sample(ABPSample &sample);

  I2CDeviceStatus test() override;
  I2CDeviceStatus reset() override;

 private:
  Pufferfish::HAL::Interfaces::I2CDevice &dev_;

  // pressure range (refer to datasheet)
  const float pmin;  // minimum pressure
  const float pmax;  // maximum pressure

  // sensor 14-bit output range
  const uint16_t output_min = 0x0666;  // 10% of 2^14
  const uint16_t output_max = 0x399A;  // 90% of 2^14
  const PressureUnit unit;
};

}  // namespace HoneywellABP
}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
