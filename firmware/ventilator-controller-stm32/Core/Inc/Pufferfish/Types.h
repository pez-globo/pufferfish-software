/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 */

#pragma once

#include <stdint.h>

namespace Pufferfish {

// This is the largest type which can be atomically updated on 32-bit processors.
// For 16-bit processors, it should be changed to uint16_t.
// For 8-bit processors, it should be changed to uint8_t.
using AtomicSize = uint32_t;

/**
 * A unit of pressure read from sensors
 */
enum class PressureUnit {
  unknown = 0,
  psi,
  pa,
  kPa,
  mPa,
  mBar,
  bar
};

}
