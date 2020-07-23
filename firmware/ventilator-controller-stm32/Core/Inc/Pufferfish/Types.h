/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 */

#pragma once

#include <stdint.h>

namespace Pufferfish {

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