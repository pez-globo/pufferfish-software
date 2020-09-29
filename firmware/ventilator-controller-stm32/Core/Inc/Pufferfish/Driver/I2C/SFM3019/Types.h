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

namespace Pufferfish {
namespace Driver {
namespace I2C {
namespace SFM3019 {

/**
 * All data in a reading from the Sensirion SFM3000 mass flow meter.
 */
struct Sample {
  int16_t raw_flow;
  float flow;
};

struct ConversionFactors {
  int16_t scale_factor = 170;
  int16_t offset = -24576;
  uint16_t flow_unit;
};

}  // namespace SFM3019
}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
