/*
 * Original work Copyright 2018, ij96
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *  Honeywell ABP sensor driver, modified from
 *    Arduino library by ij96 (https://github.com/ij96)
 */

#pragma once

#include "Pufferfish/HAL/HAL.h"
#include "Pufferfish/Types.h"
#include "Pufferfish/Driver/Testable.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

struct ABPConfig {
  uint16_t i2cAddr;
  float pmin;
  float pmax;
  PressureUnit unit;
};

/**
 * Driver for Honeywell ABP pressure sensor
 */
class HoneywellABP : public Testable {
 public:
  static constexpr ABPConfig ABPxxxx001PG2A3 = {0x28, 0.0, 1.0,
                                                PressureUnit::psi};
  static constexpr ABPConfig ABPxxxx005PG2A3 = {0x28, 0.0, 5.0,
                                                PressureUnit::psi};
  static constexpr ABPConfig ABPxxxx030PG2A3 = {0x28, 0.0, 30.0,
                                                PressureUnit::psi};

  enum class Status {
    noError = 0,
    commandMode = 1,
    staleData = 2,
    diagnotic = 3,
  };

  HoneywellABP(HAL::I2CDevice &dev, ABPConfig cfg)
      :
      mDev(dev),
      mPmin(cfg.pmin),
      mPmax(cfg.pmax),
      mUnit(cfg.unit) {
  }

  float rawToPressure(uint16_t raw);
  I2CDeviceStatus readSample();

  Status getStatus() const {
    return mStatus;
  }
  uint16_t getBridgeData() const {
    return mBridgeData;
  }
  float getPressure() const {
    return mPressure;
  }
  PressureUnit getUnit() const {
    return mUnit;
  }

  I2CDeviceStatus test() override;
  I2CDeviceStatus reset() override;
 protected:
  Pufferfish::HAL::I2CDevice &mDev;

  // pressure range (refer to datasheet)
  const float mPmin;  // minimum pressure
  const float mPmax;  // maximum pressure

  // sensor 14-bit output range
  const uint16_t mOutputMin = 0x0666;  // 10% of 2^14
  const uint16_t mOutputMax = 0x399A;  // 90% of 2^14
  const PressureUnit mUnit;

  // sensor reading
  Status mStatus = Status::noError;
  uint16_t mBridgeData = 0;
  float mPressure = 0.0;
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
