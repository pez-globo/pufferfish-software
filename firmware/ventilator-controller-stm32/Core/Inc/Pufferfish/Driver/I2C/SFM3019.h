/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SFM3000.h
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3000 Low Pressure Drop Digital Flow Meter
 */

#pragma once

#include <cstdint>

#include "Pufferfish/HAL/Interfaces/I2CDevice.h"
#include "Pufferfish/Driver/Testable.h"
#include "SensirionSensor.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

/**
 * All data in a reading from the Sensirion SFM3000 mass flow meter.
 */
struct SFM3019Sample {
  int16_t raw_flow;
  float flow;
};

struct SFM3019ConversionFactors {
  int16_t scale_factor = 170;
  int16_t offset = -24576;
  uint16_t flow_unit;
};

/**
 * Driver for Sensirion SFM3019 flow sensor
 */
class SFM3019 : public Testable {
 public:
  static constexpr uint16_t default_i2c_addr = 0x2e;

  explicit SFM3019(
      HAL::I2CDevice &dev, HAL::I2CDevice &global_dev)
      : sensirion_(dev), global_(global_dev) {}

  /**
   * Starts a flow measurement
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus start_measure();

  /**
   * Stops the flow measurement
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus stop_measure();

  /**
   * Reads out the conversion factors
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_conversion_factors(SFM3019ConversionFactors &conversion);

  /**
   * Reads out the serial number
   * @param sn[out] the unique serial number
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus serial_number(uint32_t &sn);

  /**
   * Reads out the flow rate from the sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_sample(SFM3019Sample &sample, int16_t scale_factor, int16_t offset);

  I2CDeviceStatus reset() override;
  I2CDeviceStatus test() override;

 private:
  static const uint8_t crc_poly = 0x31;
  static const uint8_t crc_init = 0xff;

  SensirionSensor sensirion_;
  HAL::I2CDevice &global_;
  float scale_factor_;
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
