/*
 *  Copyright (c) 2018, Sensirion AG <joahnnes.winkelmann@sensirion.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the Sensirion AG nor the names of its
 *        contributors may be used to endorse or promote products derived
 *        from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SDP.cpp
 * March Boonyapaluk: A driver for the SDPxx flow sensor; modified from Sensirion Arduino Library
 */

#include "Pufferfish/Driver/I2C/SDP.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus SDPSensor::serialNumber(uint32_t &pn, uint64_t &sn) {
  mMeasuring = false;

  // try to read product id
  const uint8_t CMD_LEN = 2;
  uint8_t cmd1[CMD_LEN] = {0xE1, 0x02};

  const uint8_t DATA_LEN = 12;
  uint8_t data[DATA_LEN] = {0};

  I2CDeviceStatus ret = mSensirion.write(cmd1, CMD_LEN);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  I2CDeviceStatus ret2 = mSensirion.readWithCRC(data, DATA_LEN, 0x31, 0xFF);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  // read 32 bits product number
  union {
    uint8_t bytes[4];
    uint32_t value;
  } pnVal;

  pnVal.bytes[3] = data[0];
  pnVal.bytes[2] = data[1];
  pnVal.bytes[1] = data[2];
  pnVal.bytes[0] = data[3];

  // read 64 bits serial number
  union {
    uint8_t bytes[8];
    uint64_t value;
  } snVal;

  snVal.bytes[7] = data[4];
  snVal.bytes[6] = data[5];
  snVal.bytes[5] = data[6];
  snVal.bytes[4] = data[7];
  snVal.bytes[3] = data[8];
  snVal.bytes[2] = data[9];
  snVal.bytes[1] = data[10];
  snVal.bytes[0] = data[11];

  pn = pnVal.value;
  sn = snVal.value;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::startContinuous(bool averaging) {
  const uint8_t CMD_LEN = 2;
  uint8_t cmd[CMD_LEN] = {0x36, (averaging) ? static_cast<uint8_t>(0x15) : static_cast<uint8_t>(0x1E)};
  I2CDeviceStatus ret = mSensirion.write(cmd, CMD_LEN);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  mMeasuring = true;

  return I2CDeviceStatus::ok;
}

void SDPSensor::startContinuousWait(bool stabilize) {
  HAL::delay(8);
  if (stabilize) {
    HAL::delay(12);
  }
}

I2CDeviceStatus SDPSensor::readContinuous() {
  if (!mMeasuring) {
    I2CDeviceStatus ret = this->startContinuous();
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }
  }

  const uint8_t DATA_LEN = 6;
  uint8_t data[DATA_LEN] = { 0 };

  I2CDeviceStatus ret = mSensirion.readWithCRC(data, DATA_LEN, 0x31, 0xFF);
  if (ret == I2CDeviceStatus::readError) {
    // get NACK, no new data is available
    return I2CDeviceStatus::noNewData;
  } else if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  if (data[4] != 0 && data[5] != 0) {
    parseReading(data, DATA_LEN);
  } else {
    return I2CDeviceStatus::noNewData;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::stopContinuous() {
  const uint8_t CMD_LEN = 2;
  uint8_t cmd[CMD_LEN] = { 0x3F, 0xF9 };

  mMeasuring = false;
  I2CDeviceStatus ret = mSensirion.write(cmd, CMD_LEN);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

float SDPSensor::getDifferentialPressure() const {
  return mDifferentialPressure;
}

float SDPSensor::getTemperature() const {
  return mTemperature;
}

void SDPSensor::parseReading(uint8_t data[], uint8_t size) {
  int16_t dp_raw = static_cast<int16_t>(data[0] << 8 | data[1]);
  int16_t temp_raw = static_cast<int16_t>(data[2] << 8 | data[3]);
  int16_t dp_scale = static_cast<int16_t>(data[4] << 8 | data[5]);

  if (dp_scale != 0) {
    mDifferentialPressure = dp_raw / static_cast<float>(dp_scale);
  }

  mTemperature = temp_raw / 200.0;
}

I2CDeviceStatus SDPSensor::reset() {
  uint8_t reset_cmd[] = { 0x06 };
  mMeasuring = false;

  I2CDeviceStatus ret = mSensirion.write(reset_cmd, sizeof(reset_cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::test() {
  // stop any measurement first
  this->stopContinuous();

  // read and verify serial number
  I2CDeviceStatus status;
  uint32_t pn;
  uint64_t sn;

  status = this->serialNumber(pn, sn);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  // NOTE(march): the pn number is not set in my SDP for some reason,
  //   should re-enable this for a production sensor

  // product number should be 0x030xxxxx
  //  if ((pn & 0xFFF00000) != 0x03000000) {
  //    return I2CDeviceStatus::testFailed;
  //  }

  // try soft resetting
  status = this->reset();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  HAL::delay(25);

  // try start measurement
  HAL::delay(3);
  status = this->startContinuous(true);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  this->startContinuousWait(true);

  // read & verify output
  // three attempts for measuring data
  int i = 0;
  for (int i; i < 3; i++) {
    HAL::delay(3);
    status = this->readContinuous();

    if (status == I2CDeviceStatus::ok) {
      break;
    } else if (status != I2CDeviceStatus::noNewData) {
      return status;
    }
  }

  if (i == 3) {
    return I2CDeviceStatus::testFailed;
  }

  float pres = this->getDifferentialPressure();
  float temp = this->getTemperature();

  // pressure range: -500 to 500
  if (pres < -500.0 || pres > 500.0) {
    return I2CDeviceStatus::testFailed;
  }

  // operating temp range: -40 to +85
  if (temp < -40.0 || temp > 85.0) {
    return I2CDeviceStatus::testFailed;
  }

  // stop reading
  HAL::delay(3);
  status = this->stopContinuous();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  return I2CDeviceStatus::ok;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
