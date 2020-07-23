/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/Driver/I2C/ExtendedI2CDevice.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus ExtendedI2CDevice::read(uint8_t *buf, size_t count) {
  I2CDeviceStatus stat = mMux.selectSlot(mExtSlot);
  if (stat != I2CDeviceStatus::ok) {
    return stat;
  }

  return mDev.read(buf, count);
}

I2CDeviceStatus ExtendedI2CDevice::write(uint8_t *buf, size_t count) {
  I2CDeviceStatus stat = mMux.selectSlot(mExtSlot);
  if (stat != I2CDeviceStatus::ok) {
    return stat;
  }

  return mDev.write(buf, count);
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish