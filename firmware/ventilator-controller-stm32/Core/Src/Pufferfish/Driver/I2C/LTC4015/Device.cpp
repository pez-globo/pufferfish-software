/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 *  A driver for LTC4015 buck battery charger
 */

#include "Pufferfish/Driver/I2C/LTC4015/Device.h"

#include "Pufferfish/HAL/STM32/Endian.h"
#include "Pufferfish/Util/Endian.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

I2CDeviceStatus Device::enable_coloumb_counter() {
  return ltc4015_.write(static_cast<uint16_t>(Command::enable_qcount));
}

I2CDeviceStatus Device::suspend_battery_charger() {
  return ltc4015_.write(static_cast<uint16_t>(Command::suspend_charger));
}

I2CDeviceStatus Device::read_battery_voltage(uint16_t &v_bat) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = ltc4015_.read(static_cast<uint16_t>(Command::vbat_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vbat_value = 0;
  Util::read_ntoh(buffer.data(), vbat_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  v_bat = vbat_value * 128.176;  // Vbat = [VBAT] • 128.176µV for lead-acid
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_input_voltage(uint16_t &v_in) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = ltc4015_.read(static_cast<uint16_t>(Command::vin_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vin_value = 0;
  Util::read_ntoh(buffer.data(), vin_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  v_in = vin_value * 1.648;  // Vin = [VIN] • 1.648mV
  return I2CDeviceStatus::ok;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
