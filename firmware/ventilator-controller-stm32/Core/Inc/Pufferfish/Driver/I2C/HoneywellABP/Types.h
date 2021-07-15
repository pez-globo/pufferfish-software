/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 */

#pragma once

#include <cstdint>

namespace Pufferfish::Driver::I2C::HoneywellABP {

/**
 * A unit of pressure read from sensors
 */
enum class PressureUnit { unknown = 0, psi, pa, k_pa, m_pa, m_bar, bar };

/**
 * Status code reported by the Honeywell ABP pressure sensor as part of its
 * reading
 */
enum class ABPStatus {
  no_error = 0,
  command_mode = 1,
  stale_data = 2,
  diagnostic = 3,
};

/**
 * Configuration parameters for HoneywellABP constructor
 */
struct ABPConfig {
  uint16_t i2c_addr;
  float pmin;
  float pmax;
  PressureUnit unit;
};

/**
 * All data in a reading from the Honeywell ABP pressure sensor.
 */
struct ABPSample {
  ABPStatus status;
  uint16_t bridge_data;
  float pressure;
  PressureUnit unit;
};

static const ABPConfig abpxxxx001pg2a3 = {0x28, 0.0, 1.0, PressureUnit::psi};
static const ABPConfig abpxxxx005pg2a3 = {0x28, 0.0, 5.0, PressureUnit::psi};
static const ABPConfig abpxxxx030pg2a3 = {0x28, 0.0, 30.0, PressureUnit::psi};

}  // namespace Pufferfish::Driver::I2C::HoneywellABP
