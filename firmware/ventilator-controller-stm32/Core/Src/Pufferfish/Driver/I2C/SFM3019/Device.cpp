/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3019 Low Pressure Drop Digital Flow Meter
 */

#include "Pufferfish/Driver/I2C/SFM3019/Device.h"

#include <array>

#include "Pufferfish/HAL/STM32/Endian.h"
#include "Pufferfish/Util/Parse.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

// SFM3019

I2CDeviceStatus Device::start_measure() {
  static const uint8_t start_high = 0x36;
  static const uint8_t start_low = 0x08;
  std::array<uint8_t, 2> cmd{{start_high, start_low}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  return ret;
}

I2CDeviceStatus Device::stop_measure() {
  static const uint8_t stop_high = 0x3f;
  static const uint8_t stop_low = 0xf9;
  std::array<uint8_t, 2> cmd{{stop_high, stop_low}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  return ret;
}

I2CDeviceStatus Device::serial_number(uint32_t &sn) {
  static const uint8_t serial_high = 0xe1;
  static const uint8_t serial_low = 0x02;
  std::array<uint8_t, 2> cmd{{serial_high, serial_low}};

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

I2CDeviceStatus Device::read_conversion_factors(ConversionFactors & /*conversion*/) {
  static const uint8_t conversion_high = 0x36;
  static const uint8_t conversion_low = 0x61;
  static const uint8_t arg_high = 0x36;
  static const uint8_t arg_low = 0x08;
  // TODO(lietk12): we actually have to write with a CRC!
  std::array<uint8_t, 4> cmd{{conversion_high, conversion_low, arg_high, arg_low}};

  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  std::array<uint8_t, 3 * sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret2 = sensirion_.read_with_crc(buffer.data(), buffer.size(), crc_poly, crc_init);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  /*
  conversion.scale_factor = HAL::ntoh(Util::parse_network_order<uint16_t>(
      buffer.data(), buffer.size()));
  conversion.offset = HAL::ntoh(Util::parse_network_order<uint16_t>(
      buffer.data() + sizeof(int16_t), buffer.size()));
  conversion.flow_unit = HAL::ntoh(Util::parse_network_order<uint16_t>(
      buffer.data() + 2 * sizeof(int16_t), buffer.size()));*/
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_sample(Sample &sample, int16_t scale_factor, int16_t offset) {
  // read flow raw
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = sensirion_.read_with_crc(buffer.data(), buffer.size(), crc_poly, crc_init);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  sample.raw_flow = HAL::ntoh(Util::parse_network_order<uint16_t>(buffer.data(), buffer.size()));

  // convert to actual flow rate
  sample.flow = static_cast<float>(sample.raw_flow - offset) / static_cast<float>(scale_factor);

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::reset() {
  static const uint8_t reset = 0x06;
  std::array<uint8_t, 1> cmd{{reset}};

  I2CDeviceStatus ret = global_.write(cmd.data(), cmd.size());
  return ret;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
