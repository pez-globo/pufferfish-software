/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3019 Low Pressure Drop Digital Flow Meter
 */

#include "Pufferfish/Driver/I2C/SFM3019.h"

#include <array>

#include "Pufferfish/HAL/STM32/Endian.h"
// FIXME: After HAL/Interfaces/Time.h is created, change to
// "Pufferfish/HAL/HAL.h"
#include "Pufferfish/HAL/STM32/Time.h"
#include "Pufferfish/Util/Parse.h"

namespace Pufferfish::Driver::I2C {

// SFM3019

I2CDeviceStatus SFM3019::start_measure() {
  static const uint8_t start_high = 0x36;
  static const uint8_t start_low = 0x08;
  std::array<uint8_t, 2> cmd{{start_high, start_low}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  measuring_ = true;

  return I2CDeviceStatus::ok;
}

bool SFM3019::measuring() const {
  return measuring_;
}

I2CDeviceStatus SFM3019::stop_measure() {
  static const uint8_t stop_high = 0x3f;
  static const uint8_t stop_low = 0xf9;
  std::array<uint8_t, 2> cmd{{stop_high, stop_low}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  measuring_ = false;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3019::serial_number(uint32_t &sn) {
  static const uint8_t serial_high = 0xe1;
  static const uint8_t serial_low = 0x02;
  std::array<uint8_t, 2> cmd{{serial_high, serial_low}};
  measuring_ = false;

  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  std::array<uint8_t, sizeof(uint32_t)> buffer{};
  I2CDeviceStatus ret2 = sensirion_.read_with_crc(buffer.data(), buffer.size(), crc_poly, crc_init);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  sn = HAL::ntoh(Util::parse_network_order<uint32_t>(buffer.data(), buffer.size()));
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3019::read_sample(SFM3019Sample &sample) {
  // read flow raw
  if (!measuring_) {
    I2CDeviceStatus ret = this->start_measure();
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }
    HAL::delay(1);
  }

  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = sensirion_.read_with_crc(buffer.data(), buffer.size(), crc_poly, crc_init);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  sample.raw_flow = HAL::ntoh(Util::parse_network_order<uint16_t>(buffer.data(), buffer.size()));

  // convert to actual flow rate
  sample.flow =
      static_cast<float>(static_cast<int32_t>(sample.raw_flow) - offset_flow) / scale_factor_;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3019::reset() {
  static const uint8_t reset = 0x06;
  std::array<uint8_t, 1> cmd{{reset}};
  measuring_ = false;

  I2CDeviceStatus ret = global_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3019::test() {
  // read serial number
  I2CDeviceStatus status;
  uint32_t sn = 0;

  status = this->serial_number(sn);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  if (sn != 0x04020611) {
    return I2CDeviceStatus::test_failed;
  }

  // start measurement
  status = this->start_measure();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  SFM3019Sample sample{};

  static const uint32_t startup_delay = 12;
  HAL::delay(startup_delay);
  // ignore the first read, might be invalid
  this->read_sample(sample);
  HAL::delay(1);

  // read and verify output
  status = this->read_sample(sample);
  if (status != I2CDeviceStatus::ok) {
    this->stop_measure();
    return status;
  }

  // pressure range: -200 to 200
  static const float flow_min = -200;
  static const float flow_max = 200;
  if (sample.flow < flow_min || sample.flow > flow_max) {
    return I2CDeviceStatus::test_failed;
  }

  return I2CDeviceStatus::ok;
}

}  // namespace Pufferfish::Driver::I2C
