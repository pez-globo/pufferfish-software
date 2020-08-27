/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *  Driver for TCA9548A I2C extender
 */

#pragma once

#include "I2CMux.h"
#include "Pufferfish/Driver/Testable.h"
#include "Pufferfish/HAL/HAL.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

/**
 * Driver for TCA9548A I2C multiplexer
 */
class TCA9548A : public I2CMux, public Testable {
 public:
  static const uint16_t defaultI2CAddr = 0x70;

  explicit TCA9548A(HAL::I2CDevice &dev) : mDev(dev) {}

  I2CDeviceStatus selectSlot(uint8_t slot) override;

  /**
   * Read the current control register from the mux; see TCA datasheet
   * @param controlReg[out] the control register
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus readControlReg(uint8_t &controlReg);

  uint8_t getCurrentSlot() override { return mCurrentSlot; }

  I2CDeviceStatus test() override;
  I2CDeviceStatus reset() override;

 private:
  uint8_t mCurrentSlot = 0xFF;
  HAL::I2CDevice &mDev;
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
