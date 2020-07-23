/*
 * Original work Copyright 2018, ij96
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * HoneywellABP.cpp
 *
 *  Created on: Jun 5, 2020
 *  Honeywell ABP sensor driver, modified from
 *    Arduino library by ij96 (https://github.com/ij96)
 */

#include "Pufferfish/Driver/I2C/HoneywellABP.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

constexpr ABPConfig HoneywellABP::ABPxxxx001PG2A3;
constexpr ABPConfig HoneywellABP::ABPxxxx005PG2A3;
constexpr ABPConfig HoneywellABP::ABPxxxx030PG2A3;

I2CDeviceStatus HoneywellABP::readSample() {
  uint8_t data[2] = {0};
  I2CDeviceStatus ret = mDev.read(data, 2);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  mStatus = HoneywellABP::Status(data[0] >> 6);
  mBridgeData = (data[0] << 8 | data[1]) & 0x3FFF;
  mPressure = rawToPressure(mBridgeData);

  return I2CDeviceStatus::ok;
}

float HoneywellABP::rawToPressure(uint16_t output) {
  if (output < mOutputMin)
    output = mOutputMin;
  else if (output > mOutputMax)
    output = mOutputMax;

  return float(output - mOutputMin) * (mPmax - mPmin)
      / (mOutputMax - mOutputMin) + mPmin;
}

I2CDeviceStatus HoneywellABP::test() {
  I2CDeviceStatus status;

  status = this->readSample();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  if (getStatus() != Status::noError && getStatus() != Status::staleData) {
    return I2CDeviceStatus::testFailed;
  }

  float press = getPressure();
  if (press < mPmin || press > mPmax) {
    return I2CDeviceStatus::testFailed;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus HoneywellABP::reset() {
  return I2CDeviceStatus::notSupported;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish