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
 * A driver for the SDPxx flow sensor; modified from Sensirion Arduino Library
 */

#pragma once

#include "SensirionSensor.h"
#include "Pufferfish/Driver/Testable.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

/**
 * All data in a reading from the Sensirion SDP differential pressure sensor.
 */
struct SDPSample {
  float differentialPressure;
  float temperature;
  int16_t differentialPressureScale;
};

/**
 * Driver for SDPxx flow sensor
 */
class SDPSensor : public Testable {
 public:
  static const uint16_t SDP3xI2CAddr = 0x21;
  static const uint16_t SDP8xxI2CAddr = 0x25;

  SDPSensor(HAL::I2CDevice &dev)
      :
      mSensirion(dev) {
  }

  /**
   * start continuously making measurements in sensor
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus startContinuous(bool averaging = true);

  /**
   * wait for sensor to start continuously making measurements
   */
  void startContinuousWait(bool stabilize = true);

  /**
   * read continuously-measured data from sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus readFullSample(SDPSample &sample);

  /**
   * stop continuously making measurements in sensor
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus stopContinuous();

  /**
   * Reads the serial and product number
   * @param pn[out] the product number, refer to the sensor's datasheet
   * @param sn[out] the unique serial number
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus serialNumber(uint32_t &pn, uint64_t &sn);

  I2CDeviceStatus reset() override;
  I2CDeviceStatus test() override;
  
  I2CDeviceStatus readPressureSample(int16_t differentialPressureScale, float &differentialPressure);
 private:
  SensirionSensor mSensirion;
  bool mMeasuring = false;

  void parseReading(uint8_t data[], uint8_t size, SDPSample &sample);
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
