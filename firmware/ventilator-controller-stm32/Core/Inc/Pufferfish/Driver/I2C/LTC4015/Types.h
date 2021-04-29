/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * LTC40.h
 *
 *      Author: Rohan purohit
 *  A driver for the LTC4015 buck battery charger controller
 */

#pragma once

#include <cstdint>

namespace Pufferfish {
namespace Driver {
namespace I2C {
namespace LTC4015 {

enum class Command : uint16_t {
  system_status = 0x3900,
  vbat_addr = 0x3a00,
  vin_addr = 0x3b00,
  vsys_addr = 0x3c00,
  ibat_addr = 0x3d00,
  iin_addr = 0x3e00,
  config_bits = 0x1400
};

enum class Mask : uint16_t { charger_enabled = 0x2000 };

}  // namespace LTC4015
}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
