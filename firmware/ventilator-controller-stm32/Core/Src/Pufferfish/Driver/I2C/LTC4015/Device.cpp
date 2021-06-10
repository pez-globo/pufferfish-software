/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 *  A driver for LTC4015 buck battery charger
 */

#include "Pufferfish/Driver/I2C/LTC4015/Device.h"

#include "Pufferfish/HAL/Interfaces/Endian.h"
#include "Pufferfish/Util/Endian.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

I2CDeviceStatus Device::read_charging_status(bool &charging_status) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::system_status), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  charging_status = false;
  uint16_t system_status = 0;
  Util::read_bigend(buffer.data(), system_status);
  if ((system_status & static_cast<uint16_t>(Mask::charger_enabled)) != 0) {
    charging_status = true;
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
  Util::read_bigend(buffer.data(), vbat_value);
  v_bat =
      vbat_value *
      batt_voltage_conversion;  // https://www.analog.com/media/en/technical-documentation/data-sheets/4015fb.pdf
                                // #69
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_input_voltage(uint16_t &v_in) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::vin_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vin_value = 0;
  Util::read_bigend(buffer.data(), vin_value);
  v_in =
      vin_value *
      voltage_conversion;  // https://www.analog.com/media/en/technical-documentation/data-sheets/4015fb.pdf
                           // #69
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_system_voltage(uint16_t &v_sys) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::vsys_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t vsys_value = 0;
  Util::read_bigend(buffer.data(), vsys_value);
  v_sys =
      vsys_value *
      voltage_conversion;  // https://www.analog.com/media/en/technical-documentation/data-sheets/4015fb.pdf
                           // #69
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_battery_current(uint16_t &i_bat) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::ibat_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t i_bat_value = 0;
  Util::read_bigend(buffer.data(), i_bat_value);
  i_bat =
      i_bat_value *
      current_conversion;  // https://www.analog.com/media/en/technical-documentation/data-sheets/4015fb.pdf
                           // #69
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus Device::read_input_current(uint16_t &i_in) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  I2CDeviceStatus ret = i2cdevice_.read(static_cast<uint16_t>(Command::iin_addr), buffer);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  uint16_t i_in_value = 0;
  Util::read_bigend(buffer.data(), i_in_value);
  i_in =
      i_in_value *
      current_conversion;  // https://www.analog.com/media/en/technical-documentation/data-sheets/4015fb.pdf
                           // #70
  return I2CDeviceStatus::ok;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
