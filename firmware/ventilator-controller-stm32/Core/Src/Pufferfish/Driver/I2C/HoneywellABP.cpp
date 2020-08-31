/*
 * Original work Copyright 2018, ij96
 * Modified work Copyright 2020, the Pez Globo team and the Pufferfish project
 * contributors
 *
 * HoneywellABP.cpp
 *
 *  Created on: Jun 5, 2020
 *  Honeywell ABP sensor driver, modified from
 *    Arduino library by ij96 (https://github.com/ij96)
 */

#include "Pufferfish/Driver/I2C/HoneywellABP.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {

constexpr ABPConfig HoneywellABP::abpxxxx001pg2a3;
constexpr ABPConfig HoneywellABP::abpxxxx005pg2a3;
constexpr ABPConfig HoneywellABP::abpxxxx030pg2a3;

I2CDeviceStatus HoneywellABP::read_sample(ABPSample &sample) {
  uint8_t data[2] = {0};
  I2CDeviceStatus ret = dev_.read(data, 2);
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  sample.status = ABPStatus(data[0] >> 6);
  sample.bridge_data = (data[0] << 8 | data[1]) & 0x3FFF;
  sample.pressure = raw_to_pressure(sample.bridge_data);
  sample.unit = unit;

  return I2CDeviceStatus::ok;
}

float HoneywellABP::raw_to_pressure(uint16_t output) const {
  if (output < output_min) {
    output = output_min;
  } else if (output > output_max) {
    output = output_max;
  }

  return float(output - output_min) * (pmax - pmin) /
             (output_max - output_min) +
         pmin;
}

I2CDeviceStatus HoneywellABP::test() {
  I2CDeviceStatus status;

  ABPSample sample{};

  status = this->read_sample(sample);
  if (status != I2CDeviceStatus::ok) {
    return status;
  }

  if (sample.status != ABPStatus::no_error &&
      sample.status != ABPStatus::stale_data) {
    return I2CDeviceStatus::test_failed;
  }

  if (sample.pressure < pmin || sample.pressure > pmax) {
    return I2CDeviceStatus::test_failed;
  }

  return I2CDeviceStatus::ok;
}

I2CDeviceStatus HoneywellABP::reset() { return I2CDeviceStatus::not_supported; }

}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
