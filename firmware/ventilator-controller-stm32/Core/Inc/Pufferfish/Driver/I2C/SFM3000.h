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
   * Reads out the flow rate from the sensor, and save it
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus readSample();

  /**
   * Returns the most up-to-date non-scaled flow rate
   * @return the non-scaled flow rate
   */
  uint16_t getFlowRaw();

  /**
   * Returns the most up-to-date flow rate
   * @return the most up-to-date flow rate
   */
  float getFlow();

  I2CDeviceStatus reset() override;
  I2CDeviceStatus test() override;
 private:
  SensirionSensor mSensirion;
  bool mMeasuring = false;
  float mScaleFactor;
  float mRawFlow = 0.0;
  float mFlow = 0.0;
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
