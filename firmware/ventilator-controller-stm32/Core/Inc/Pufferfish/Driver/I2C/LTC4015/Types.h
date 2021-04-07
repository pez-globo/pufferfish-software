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
  limit_alerts = 0x3600,
  vbat_lo_alert_limit = 0x013a,
  vbat_hi_alert_limit = 0x023a,
  vin_low_alert_limit = 0x033b,
  vin_hi_alert_limit = 0x043b,
  vsys_lo_alert_limit = 0x053c,
  vsys_hi_alert_limit = 0x063c,
  qcount_lo_alert_limit = 0x1013,
  qcount_hi_alert_limit = 0x1113,
  qcount = 0x1300,
  enable_qcount = 0x1400,
  suspend_charger = 0x1400
};

struct LimitAlerts {
  uint8_t vbat_lo_alert{};
  uint8_t vbat_hi_alert{};
  uint8_t vin_low_alert{};
  uint8_t vin_hi_alert{};
  uint8_t vsys_hi_alert{};
  uint8_t vsys_lo_alert{};
  uint8_t qcount_lo_alert{};
  uint8_t qcount_hi_alert{};
};

}  // namespace LTC4015
}  // namespace I2C
}  // namespace Driver
}  // namespace Pufferfish
