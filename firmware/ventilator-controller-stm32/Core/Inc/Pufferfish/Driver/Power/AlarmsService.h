/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Util/Containers/Array.h"

namespace Pufferfish::Driver::Power {

// All alarm codes for power management need to be registered in the following array:
static constexpr auto alarm_codes =
    Util::Containers::make_array<LogEventCode>(LogEventCode::LogEventCode_charger_disconnected);

class AlarmsService {
 public:
  void transform(
      const MCUPowerStatus &mcu_power_status,
      Application::AlarmsManager &alarms_manager);
};

}  // namespace Pufferfish::Driver::Power
