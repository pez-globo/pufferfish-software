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
#include "Pufferfish/Util/Endian.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

// SFM3019

I2CDeviceStatus Device::start_measure() {
  return sensirion_.write(static_cast<uint16_t>(gas));
}

I2CDeviceStatus Device::stop_measure() {
  return sensirion_.write(static_cast<uint16_t>(Command::stop_measure));
}

I2CDeviceStatus Device::read_product_id(uint32_t &product_number) {
  I2CDeviceStatus ret = sensirion_.write(static_cast<uint16_t>(Command::read_product_id));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  std::array<uint8_t, sizeof(uint32_t)> buffer{};
  I2CDeviceStatus ret2 = sensirion_.read(buffer);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  Util::read_ntoh(buffer.data(), product_number);
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::set_averaging(uint8_t averaging_window) {
  return sensirion_.write(static_cast<uint16_t>(Command::set_averaging), averaging_window);
}

I2CDeviceStatus Device::request_conversion_factors() {
  return sensirion_.write(
      static_cast<uint16_t>(Command::read_conversion), static_cast<uint16_t>(gas));
}

I2CDeviceStatus Device::read_conversion_factors(ConversionFactors &conversion) {
  std::array<uint8_t, 3 * sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = sensirion_.read(buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  Util::read_ntoh(buffer.data(), conversion.scale_factor);
  Util::read_ntoh(buffer.data() + sizeof(uint16_t), conversion.offset);
  Util::read_ntoh(buffer.data() + 2 * sizeof(uint16_t), conversion.flow_unit);
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_sample(Sample &sample, int16_t scale_factor, int16_t offset) {
  // read flow raw
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = sensirion_.read(buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  Util::read_ntoh(buffer.data(), sample.raw_flow);

  // convert to actual flow rate
  sample.flow = static_cast<float>(sample.raw_flow - offset) / static_cast<float>(scale_factor);

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::reset() {
  return global_.write(static_cast<uint8_t>(Command::reset));
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
