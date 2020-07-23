/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/Driver/I2C/SensirionSensor.h"
#include "Pufferfish/HAL/CRC.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus SensirionSensor::readWithCRC(uint8_t *buf, size_t count,
                                             uint8_t polynomial, uint8_t init) {
  if (count % 2) {
    return I2CDeviceStatus::invalidArguments;
  }

  uint8_t buf_crc[3];

  for (; count > 0; count -= 2, buf += 2) {
    I2CDeviceStatus ret = mDev.read(buf_crc, sizeof(buf_crc));
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }

    uint8_t expected = Pufferfish::HAL::computeCRC8(buf_crc, 2, polynomial,
                                                    init, false, false, 0x00);
    if (expected != buf_crc[2]) {
      return I2CDeviceStatus::crcCheckFailed;
    }

    buf[0] = buf_crc[0];
    buf[1] = buf_crc[1];
  }

  return I2CDeviceStatus::ok;
}
I2CDeviceStatus SensirionSensor::write(uint8_t *buf, size_t count) {
  return mDev.write(buf, count);
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish