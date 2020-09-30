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

#include "Pufferfish/Driver/I2C/SensirionSensor.h"
#include "Pufferfish/Driver/Testable.h"
#include "Pufferfish/HAL/Interfaces/I2CDevice.h"
#include "Types.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

static const uint16_t default_i2c_addr = 0x2e;

/**
 * Stateless low-level driver for Sensirion SFM3019 flow sensor device
 */
class Device {
 public:
  explicit Device(HAL::I2CDevice &dev, HAL::I2CDevice &global_dev)
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
  I2CDeviceStatus read_conversion_factors(ConversionFactors &conversion);

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
  I2CDeviceStatus read_sample(Sample &sample, int16_t scale_factor, int16_t offset);

  /**
   * Causes a global I2C device reset
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus reset();

 private:
  static const uint8_t crc_poly = 0x31;
  static const uint8_t crc_init = 0xff;

  SensirionSensor sensirion_;
  HAL::I2CDevice &global_;
};

}  // namespace Pufferfish::Driver::I2C::SFM3019
