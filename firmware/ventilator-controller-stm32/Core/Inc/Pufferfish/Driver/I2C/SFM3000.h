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

#include "SensirionSensor.h"
#include "Pufferfish/Driver/Testable.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {


/**
 * All data in a reading from the Sensirion SFM3000 mass flow meter.
 */
struct SFM3000Sample {
  uint16_t rawFlow;
  float flow;
};

/**
 * Driver of Sensirion SFM3000 flow sensor
 */
class SFM3000 : public Testable {
 public:
  static const uint16_t defaultI2CAddr = 0x40;

  static const int offsetFlow = 32000;
  static constexpr float scaleFactorAir = 140.0f;
  static constexpr float scaleFactorO2 = 142.8f;

  SFM3000(HAL::I2CDevice &dev, float scaleFactor = scaleFactorAir)
      :
      mSensirion(dev),
      mScaleFactor(scaleFactor) {
  }

  /**
   * Starts a flow measurement
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus startMeasure();

  /**
   * Reads out the serial number
   * @param sn[out] the unique serial number
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus serialNumber(uint32_t &sn);

  /**
   * Reads out the flow rate from the sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus readSample(SFM3000Sample &sample);

  I2CDeviceStatus reset() override;
  I2CDeviceStatus test() override;
 private:
  SensirionSensor mSensirion;
  bool mMeasuring = false;
  float mScaleFactor;
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
