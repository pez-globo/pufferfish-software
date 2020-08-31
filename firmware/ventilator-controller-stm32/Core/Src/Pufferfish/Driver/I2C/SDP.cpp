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
 * SDP.cpp
 * March Boonyapaluk: A driver for the SDPxx flow sensor; modified from
 * Sensirion Arduino Library
 */

#include "Pufferfish/Driver/I2C/SDP.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus SDPSensor::serial_number(uint32_t &pn, uint64_t &sn) {
  measuring_ = false;

  // try to read product id
  const uint8_t cmd_len = 2;
  uint8_t cmd1[cmd_len] = {0xE1, 0x02};

  const uint8_t data_len = 12;
  uint8_t data[data_len] = {0};

  I2CDeviceStatus ret = sensirion_.write(cmd1, cmd_len);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  I2CDeviceStatus ret2 = sensirion_.read_with_crc(data, data_len, 0x31, 0xFF);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  // read 32 bits product number
  union {
    uint8_t bytes[4];
    uint32_t value;
  } pn_val{};

  pn_val.bytes[3] = data[0];
  pn_val.bytes[2] = data[1];
  pn_val.bytes[1] = data[2];
  pn_val.bytes[0] = data[3];

  // read 64 bits serial number
  union {
    uint8_t bytes[8];
    uint64_t value;
  } sn_val{};

  sn_val.bytes[7] = data[4];
  sn_val.bytes[6] = data[5];
  sn_val.bytes[5] = data[6];
  sn_val.bytes[4] = data[7];
  sn_val.bytes[3] = data[8];
  sn_val.bytes[2] = data[9];
  sn_val.bytes[1] = data[10];
  sn_val.bytes[0] = data[11];

  pn = pn_val.value;
  sn = sn_val.value;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::start_continuous(bool averaging) {
  const uint8_t cmd_len = 2;
  uint8_t cmd[cmd_len] = {0x36, (averaging) ? static_cast<uint8_t>(0x15)
                                            : static_cast<uint8_t>(0x1E)};
  I2CDeviceStatus ret = sensirion_.write(cmd, cmd_len);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  measuring_ = true;

  return I2CDeviceStatus::ok;
}

void SDPSensor::start_continuous_wait(bool stabilize) {
  HAL::delay(8);
  if (stabilize) {
    HAL::delay(12);
  }
}

I2CDeviceStatus SDPSensor::read_sample(SDPSample &sample) {
  if (!measuring_) {
    I2CDeviceStatus ret = this->start_continuous();
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }
  }

  const uint8_t data_len = 6;
  uint8_t data[data_len] = {0};

  I2CDeviceStatus ret = sensirion_.read_with_crc(data, data_len, 0x31, 0xFF);
  if (ret == I2CDeviceStatus::read_error) {
    // get NACK, no new data is available
    return I2CDeviceStatus::no_new_data;
  }
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  if (data[4] != 0 && data[5] != 0) {
    parse_reading(data, data_len, sample);
  } else {
    return I2CDeviceStatus::no_new_data;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::stop_continuous() {
  const uint8_t cmd_len = 2;
  uint8_t cmd[cmd_len] = {0x3F, 0xF9};

  measuring_ = false;
  I2CDeviceStatus ret = sensirion_.write(cmd, cmd_len);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

void SDPSensor::parse_reading(const uint8_t data[], uint8_t /*size*/,
                              SDPSample &sample) {
  int16_t dp_raw = data[0] << 8 | data[1];
  int16_t temp_raw = data[2] << 8 | data[3];
  int16_t dp_scale = data[4] << 8 | data[5];

  if (dp_scale != 0) {
    sample.differential_pressure = dp_raw / static_cast<float>(dp_scale);
  }

  sample.temperature = temp_raw / 200.0;
}

I2CDeviceStatus SDPSensor::reset() {
  uint8_t reset_cmd[] = {0x06};
  measuring_ = false;

  I2CDeviceStatus ret = sensirion_.write(reset_cmd, sizeof(reset_cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SDPSensor::test() {
  // stop any measurement first
  this->stop_continuous();

  // read and verify serial number
  I2CDeviceStatus status;
  uint32_t pn = 0;
  uint64_t sn = 0;

  status = this->serial_number(pn, sn);
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
  status = this->start_continuous(true);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  this->start_continuous_wait(true);

  // read & verify output
  // three attempts for measuring data
  int i = 0;
  SDPSample sample{};
  for (i = 0; i < 3; i++) {
    HAL::delay(3);
    status = this->read_sample(sample);

    if (status == I2CDeviceStatus::ok) {
      break;
    }
    if (status != I2CDeviceStatus::no_new_data) {
      return status;
    }
  }

  if (i == 3) {
    return I2CDeviceStatus::test_failed;
  }

  // pressure range: -500 to 500
  if (sample.differential_pressure < -500.0 ||
      sample.differential_pressure > 500.0) {
    return I2CDeviceStatus::test_failed;
  }

  // operating temp range: -40 to +85
  if (sample.temperature < -40.0 || sample.temperature > 85.0) {
    return I2CDeviceStatus::test_failed;
  }

  // stop reading
  HAL::delay(3);
  status = this->stop_continuous();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  return I2CDeviceStatus::ok;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
