/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SFM3000.h
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3000 Low Pressure Drop Digital Flow Meter
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Util/Bytes.h"

namespace Pufferfish {
namespace Driver {
namespace I2C {
namespace SFM3019 {

enum class GasType : uint16_t { o2 = 0x3603, air = 0x3608, mixture = 0x3632 };

enum class Command : uint16_t {
  start_measure_o2 = static_cast<uint16_t>(GasType::o2),
  start_measure_air = static_cast<uint16_t>(GasType::air),
  start_measure_mixture = static_cast<uint16_t>(GasType::mixture),
  update_concentration = 0xe17d,
  reset_i2c_addr_ptr = 0xe000,
  stop_measure = 0x3ff9,
  config_averaging = 0x366a,
  read_conversion = 0x3661,
  reset = 0x0006,
  sleep = 0x3677,
  read_product_id = 0xe102
};

constexpr uint8_t get_upper(GasType gas) {
  return Util::get_byte<1>(static_cast<uint16_t>(gas));
}
constexpr uint8_t get_lower(GasType gas) {
  return Util::get_byte<0>(static_cast<uint16_t>(gas));
}
constexpr uint8_t get_upper(Command command) {
  return Util::get_byte<1>(static_cast<uint16_t>(command));
}
constexpr uint8_t get_lower(Command command) {
  return Util::get_byte<0>(static_cast<uint16_t>(command));
}

/**
 * All data in a reading from the Sensirion SFM3000 mass flow meter.
 */
struct Sample {
  int16_t raw_flow;
  float flow;
};

struct ConversionFactors {
  static const int16_t default_scale_factor = 170;
  static const int16_t default_offset = -24576;

  int16_t scale_factor = 2 * default_scale_factor;
  int16_t offset = default_offset;
  uint16_t flow_unit{};
};

}  // namespace SFM3019
}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
