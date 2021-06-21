/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/Power/AlarmsService.h"

namespace Pufferfish::Driver::Power {

void AlarmsService::generate_alarm(
    const LogEventCode &alarm_code, bool alarm_active, Application::AlarmsManager &alarms_manager) {
  if (alarm_active) {
    alarms_manager.activate_alarm(alarm_code, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(alarm_code);
  }
}

// AlarmsService
void AlarmsService::transform(
    const MCUPowerStatus &mcu_power_status, Application::AlarmsManager &alarms_manager) const {
  bool charger_disconnected = false;
  if (!mcu_power_status.charging) {
    charger_disconnected = true;
  }
  bool battery_low = false;
  if (mcu_power_status.power_left < battery_low_power) {
    battery_low = true;
  }
  bool critical_battery = false;
  if (mcu_power_status.power_left <= critical_battery_power) {
    critical_battery = true;
  }

  generate_alarm(LogEventCode_charger_disconnected, charger_disconnected, alarms_manager);
  generate_alarm(LogEventCode_battery_low, battery_low, alarms_manager);
  generate_alarm(LogEventCode_critical_battery, critical_battery, alarms_manager);
}

}  // namespace Pufferfish::Driver::Power
