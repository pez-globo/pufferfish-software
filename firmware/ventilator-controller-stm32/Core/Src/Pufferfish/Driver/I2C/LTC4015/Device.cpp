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

I2CDeviceStatus Device::is_charger_enabled(BatteryPower &battery_power) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::system_status), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  battery_power.charging_status = false;
  uint16_t system_status = 0;
  Util::read_ntoh(buffer.data(), system_status);
  if ((system_status & static_cast<uint16_t>(Mask::charger_enabled)) != 0) {
    battery_power.charging_status = true;
  }
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_battery_voltage(uint16_t &v_bat) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::vbat_addr), buffer);
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
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::vin_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vin_value = 0;
  Util::read_ntoh(buffer.data(), vin_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  v_in = vin_value * 1.648;  // Vin = [VIN] • 1.648mV
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_system_voltage(uint16_t &v_sys) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::vsys_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vsys_value = 0;
  Util::read_ntoh(buffer.data(), vsys_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  v_sys = vsys_value * 1.648;  // Vsys = [VSYS] • 1.648mV
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_battery_current(uint16_t &i_bat) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::ibat_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t i_bat_value = 0;
  Util::read_ntoh(buffer.data(), i_bat_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  i_bat = i_bat_value * 1.46487;  // Ibat = [IBAT] • 1.46487mV
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_input_current(uint16_t &i_in) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::iin_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t i_in_value = 0;
  Util::read_ntoh(buffer.data(), i_in_value);
  // NOLINTNEXTLINE(readability-magic-numbers)
  i_in = i_in_value * 1.46487;  // Iin = [IIN] • 1.46487mV
  return I2CDeviceStatus::ok;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
