/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *  Driver for TCA9548A I2C extender
 */

#include "Pufferfish/Driver/I2C/TCA9548A.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus TCA9548A::selectSlot(uint8_t slot) {
  if (slot > 7) {
    return I2CDeviceStatus::invalidExtSlot;
  }

  if (slot == mCurrentSlot) {
    // already on the right slot, can skip
    return I2CDeviceStatus::ok;
  }

  uint8_t cmd = 1 << slot;
  I2CDeviceStatus ret = mDev.write(&cmd, 1);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  mCurrentSlot = slot;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus TCA9548A::readControlReg(uint8_t &controlReg) {
  I2CDeviceStatus ret = mDev.read(&controlReg, 1);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus TCA9548A::test() {
  I2CDeviceStatus status;
  mCurrentSlot = 0xFF;

  status = this->selectSlot(4);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  uint8_t conreg;
  status = this->readControlReg(conreg);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  if (conreg != (1 << 4)) {
    return I2CDeviceStatus::testFailed;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus TCA9548A::reset() {
  return I2CDeviceStatus::notSupported;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish