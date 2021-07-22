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

#include "Pufferfish/Driver/I2C/HoneywellABP/Device.h"

#include <array>
#include <climits>

#include "Pufferfish/Util/Endian.h"
#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::I2C::HoneywellABP {

I2CDeviceStatus Device::read_sample(ABPSample &sample) {
  std::array<uint8_t, 2> data{{0, 0}};
  I2CDeviceStatus ret = dev_.read(data.data(), data.size());
  if (ret != I2CDeviceStatus::ok) {
    return ret;
  }

  static const uint16_t bridge_mask = 0x3FFF;
  sample.status = ABPStatus(data[0] >> status_shift);
  Util::read_bigend(data.data(), sample.bridge_data);
  sample.bridge_data &= bridge_mask;
  sample.pressure = raw_to_pressure(sample.bridge_data);
  sample.unit = unit;

  return I2CDeviceStatus::ok;
}

float Device::raw_to_pressure(uint16_t output) const {
  auto output_pressure = Util::clamp<uint16_t>(output, output_min, output_max);

  // Since these variables are uint16_t, we promote them to int32_t for signed
  // integer subtraction
  static const int32_t output_width =
      static_cast<int32_t>(output_max) - static_cast<int32_t>(output_min);
  const int32_t relative_output =
      static_cast<int32_t>(output_pressure) - static_cast<int32_t>(output_min);

  return static_cast<float>(relative_output) * (pmax - pmin) / static_cast<float>(output_width) +
         pmin;
}

}  // namespace Pufferfish::Driver::I2C::HoneywellABP
