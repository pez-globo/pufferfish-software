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
  std::array<uint8_t, sizeof(Command)> cmd{{get_upper(gas), get_lower(gas)}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  return ret;
}

I2CDeviceStatus Device::stop_measure() {
  std::array<uint8_t, sizeof(Command)> cmd{
      {get_upper(Command::stop_measure), get_lower(Command::stop_measure)}};
  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  return ret;
}

I2CDeviceStatus Device::read_product_id(uint32_t &product_number) {
  std::array<uint8_t, sizeof(Command)> cmd{
      {get_upper(Command::read_product_id), get_lower(Command::read_product_id)}};

  I2CDeviceStatus ret = sensirion_.write(cmd.data(), cmd.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  std::array<uint8_t, sizeof(uint32_t)> buffer{};
  I2CDeviceStatus ret2 = sensirion_.read_with_crc(buffer.data(), buffer.size(), crc_poly, crc_init);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  product_number = HAL::ntoh(Util::parse_network_order<uint32_t>(buffer.data(), buffer.size()));
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_conversion_factors(ConversionFactors & /*conversion*/) {
  // TODO(lietk12): we actually have to write with a CRC!
  std::array<uint8_t, sizeof(Command) + sizeof(GasType)> cmd{
      {get_upper(Command::read_conversion),
       get_lower(Command::read_conversion),
       get_upper(gas),
       get_lower(gas)}};

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
  std::array<uint8_t, sizeof(uint8_t)> cmd{{get_lower(Command::reset)}};

  I2CDeviceStatus ret = global_.write(cmd.data(), cmd.size());
  return ret;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
