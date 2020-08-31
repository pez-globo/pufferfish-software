/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3000 Low Pressure Drop Digital Flow Meter
 */

#include "Pufferfish/Driver/I2C/SFM3000.h"

#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

I2CDeviceStatus SFM3000::start_measure() {
  uint8_t cmd[] = {0x10, 0x00};
  I2CDeviceStatus ret = sensirion_.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }
  measuring_ = true;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::serial_number(uint32_t &sn) {
  uint8_t cmd[] = {0x31, 0xAE};
  measuring_ = false;

  I2CDeviceStatus ret = sensirion_.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  union Uint32Buffer {
    uint8_t bytes[sizeof(sn)];
    uint32_t value;
  } buffer{};

  I2CDeviceStatus ret2 =
      sensirion_.read_with_crc(buffer.bytes, sizeof(buffer.bytes), 0x31, 0x00);
  if (ret2 != I2CDeviceStatus::ok) {
    return ret2;
  }

  sn = Pufferfish::HAL::ntoh(buffer.value);
  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::read_sample(SFM3000Sample &sample) {
  // read flow raw
  if (!measuring_) {
    I2CDeviceStatus ret = this->start_measure();
    if (ret != I2CDeviceStatus::ok) {
      return ret;
    }
    HAL::delay(1);
  }

  union Uint16Buffer {
    uint8_t bytes[sizeof(uint16_t)];
    uint16_t value;
  } buffer{};

  I2CDeviceStatus ret =
      sensirion_.read_with_crc(buffer.bytes, sizeof(buffer.bytes), 0x31, 0x00);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  sample.raw_flow = Pufferfish::HAL::ntoh(buffer.value);

  // convert to actual flow rate
  sample.flow = static_cast<int>(sample.raw_flow - offset_flow) / scale_factor_;

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::reset() {
  uint8_t cmd[] = {0x20, 0x00};
  measuring_ = false;

  I2CDeviceStatus ret = sensirion_.write(cmd, sizeof(cmd));
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus SFM3000::test() {
  // read serial number
  I2CDeviceStatus status;
  uint32_t sn = 0;

  status = this->serial_number(sn);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  // start measurement
  status = this->start_measure();
  if (status != I2CDeviceStatus::ok) {
    return status;
  }
  SFM3000Sample sample{};

  // ignore the first read, might be invalid
  this->read_sample(sample);
  HAL::delay(1);

  // read and verify output
  status = this->read_sample(sample);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  // pressure range: -200 to 200
  if (sample.flow < -200.0 || sample.flow > 200.0) {
    return I2CDeviceStatus::test_failed;
  }

  return I2CDeviceStatus::ok;
}

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
