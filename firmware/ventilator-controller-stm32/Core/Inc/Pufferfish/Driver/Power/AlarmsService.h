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

using Application::LogEventCode;
using Application::MCUPowerStatus;

// All alarm codes for power management need to be registered in the following array:
static constexpr auto alarm_codes = Util::Containers::make_array<LogEventCode>(
    LogEventCode::LogEventCode_charger_disconnected,
    LogEventCode::LogEventCode_battery_low,
    LogEventCode::LogEventCode_battery_critical);

class AlarmsService {
 public:
  void transform(
      const MCUPowerStatus &mcu_power_status, Application::AlarmsManager &alarms_manager);

 private:
  enum class BatteryLevel { critical, low, normal };

  BatteryLevel battery_level_ = BatteryLevel::normal;
  static constexpr float battery_low_power = 30;
  static constexpr float critical_battery_power = 5;
};

}  // namespace Pufferfish::Driver::Power
