/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3000 Low Pressure Drop Digital Flow Meter
 */

#include "Pufferfish/Driver/I2C/SFM3000.h"
#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus SFM3000::startMeasure() {
  uint8_t cmd[] = {0x10, 0x00};
  I2CDeviceStatus ret = mSensirion.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  mMeasuring = true;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::serialNumber(uint32_t &sn) {
  uint8_t cmd[] = {0x31, 0xAE};
  mMeasuring = false;

  I2CDeviceStatus ret = mSensirion.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  I2CDeviceStatus ret2 = mSensirion.readWithCRC((uint8_t *) &sn, sizeof(sn), 0x31,
                                                0x00);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  sn = Pufferfish::HAL::ntoh(sn);
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::readSample(SFM3000Sample &sample) {
  // read flow raw
  if (!mMeasuring) {
    I2CDeviceStatus ret = this->startMeasure();
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }
    HAL::delay(1);
  }

  uint16_t val;
  I2CDeviceStatus ret = mSensirion.readWithCRC((uint8_t *) &val, sizeof(val), 0x31,
                                               0x00);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  sample.rawFlow = Pufferfish::HAL::ntoh(val);

  // convert to actual flow rate
  sample.flow = static_cast<int>(sample.rawFlow - offsetFlow) / mScaleFactor;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::reset() {
  uint8_t cmd[] = { 0x20, 0x00 };
  mMeasuring = false;

  I2CDeviceStatus ret = mSensirion.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::test() {
  // read serial number
  I2CDeviceStatus status;
  uint32_t sn;

  status = this->serialNumber(sn);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  // start measurement
  status = this->startMeasure();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  SFM3000Sample sample;

  // ignore the first read, might be invalid
  this->readSample(sample);
  HAL::delay(1);

  // read and verify output
  status = this->readSample(sample);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }


  // pressure range: -200 to 200
  if (sample.flow < -200.0 || sample.flow > 200.0) {
    return I2CDeviceStatus::testFailed;
  }

  return I2CDeviceStatus::ok;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
