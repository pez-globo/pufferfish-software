/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 */

#include "Pufferfish/Driver/I2C/LTC4015/I2CDevice.h"

#include <array>

#include "Pufferfish/Util/Bytes.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

I2CDeviceStatus I2CDevice::is_device_ready() {
  I2CDeviceStatus ret = dev_.is_device_ready();
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus I2CDevice::read(uint16_t address, std::array<uint8_t, sizeof(uint16_t)> &buf) {
  I2CDeviceStatus ret = dev_.read(address, buf.data(), buf.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus I2CDevice::write(uint16_t command) {
  std::array<uint8_t, sizeof(uint16_t)> buf{
      {Util::get_byte<1>(command), Util::get_byte<0>(command)}};
  return dev_.write(buf.data(), buf.size());
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
