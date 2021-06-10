/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"

namespace Pufferfish::Driver::Power {

// All alarm codes for power management need to be registered in the following array:
static constexpr auto alarm_codes =
    Util::make_array<LogEventCode>(LogEventCode::LogEventCode_charger_disconnected);

class AlarmsService {
 public:
  virtual void transform(
      MCUPowerStatus &mcu_power_status,
      ActiveLogEvents &active_log_events,
      Application::AlarmsManager &alarms_manager);
};

}  // namespace Pufferfish::Driver::Power
