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
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project
 * contributors
 *
 * A driver for the SDPxx flow sensor; modified from Sensirion Arduino Library
 */

#pragma once

#include "Pufferfish/Driver/Testable.h"
#include "Pufferfish/HAL/HAL.h"
#include "SensirionSensor.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

/**
 * All data in a reading from the Sensirion SDP differential pressure sensor.
 */
struct SDPSample {
  float differential_pressure;
  float temperature;
};

/**
 * Driver for SDPxx flow sensor
 */
class SDPSensor : public Testable {
 public:
  static const uint16_t sdp3x_i2c_addr = 0x21;
  static const uint16_t sdp8xx_i2c_addr = 0x25;

  // Cppcheck false positive, dev cannot be given to SensirionSensor ctor as
  // const ref cppcheck-suppress constParameter
  explicit SDPSensor(HAL::I2CDevice &dev) : sensirion_(dev) {}

  /**
   * start continuously making measurements in sensor
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus start_continuous(bool averaging = true);

  /**
   * wait for sensor to start continuously making measurements
   */
  static void start_continuous_wait(bool stabilize = true);

  /**
   * read continuously-measured data from sensor
   * @param sample[out] the sensor reading; only valid on success
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus read_sample(SDPSample &sample);

  /**
   * stop continuously making measurements in sensor
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus stop_continuous();

  /**
   * Reads the serial and product number
   * @param pn[out] the product number, refer to the sensor's datasheet
   * @param sn[out] the unique serial number
   * @return ok on success, error code otherwise
   */
  I2CDeviceStatus serial_number(uint32_t &pn, uint64_t &sn);

  I2CDeviceStatus reset() override;
  I2CDeviceStatus test() override;

 private:
  SensirionSensor sensirion_;
  bool measuring_ = false;

  static void parse_reading(const uint8_t data[], uint8_t size,
                            SDPSample &sample);
};

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
