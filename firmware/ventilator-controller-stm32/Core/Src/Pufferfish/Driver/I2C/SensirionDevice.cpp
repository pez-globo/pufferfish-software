/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/Driver/I2C/SensirionDevice.h"

#include <array>

#include "Pufferfish/HAL/CRC.h"
#include "Pufferfish/Util/Bytes.h"

namespace Pufferfish::Driver::I2C {

I2CDeviceStatus SensirionDevice::read_with_crc(
    uint8_t *buf, size_t count, uint8_t polynomial, uint8_t init) {
  if (count % 2 > 0) {
    return I2CDeviceStatus::invalid_arguments;
  }

  std::array<uint8_t, 3> buf_crc{};

  for (; count > 0; count -= 2, buf += 2) {
    I2CDeviceStatus ret = dev_.read(buf_crc.data(), buf_crc.size());
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }

    uint8_t expected =
        Pufferfish::HAL::compute_crc8(buf_crc.data(), 2, polynomial, init, false, false, 0x00);
    if (expected != buf_crc[2]) {
      return I2CDeviceStatus::crc_check_failed;
    }

    buf[0] = buf_crc[0];
    buf[1] = buf_crc[1];
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SensirionDevice::write(uint8_t command) {
  return dev_.write(&command, sizeof(uint8_t));
}

I2CDeviceStatus SensirionDevice::write(uint16_t command) {
  std::array<uint8_t, sizeof(uint16_t)> buf{
      {Util::get_byte<1>(command), Util::get_byte<0>(command)}};
  return dev_.write(buf.data(), buf.size());
}

I2CDeviceStatus SensirionDevice::write(
    uint16_t command, uint16_t arg, uint8_t polynomial, uint8_t init) {
  static const size_t write_buf_size = 2 * sizeof(uint16_t) + sizeof(uint8_t);
  std::array<uint8_t, write_buf_size> write_buf{
      {Util::get_byte<1>(command),
       Util::get_byte<0>(command),
       Util::get_byte<1>(arg),
       Util::get_byte<0>(arg),
       0}};

  uint8_t crc = Pufferfish::HAL::compute_crc8(
      write_buf.data() + sizeof(uint16_t), sizeof(uint16_t), polynomial, init, false, false, 0x00);
  write_buf[4] = crc;

  return dev_.write(write_buf.data(), write_buf.size());
}

}  // namespace Pufferfish::Driver::I2C
