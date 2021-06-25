/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/Power/AlarmsService.h"

namespace Pufferfish::Driver::Power {

// AlarmsService
void AlarmsService::transform(
    const MCUPowerStatus &mcu_power_status, Application::AlarmsManager &alarms_manager) {
  if (!mcu_power_status.charging) {
    alarms_manager.activate_alarm(
        LogEventCode_charger_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_charger_disconnected);
  }

  if (mcu_power_status.power_left <= critical_battery_power) {
    battery_level_ = BatteryLevel::critical;
  } else if (mcu_power_status.power_left <= battery_low_power) {
    battery_level_ = BatteryLevel::low;
  } else {
    battery_level_ = BatteryLevel::normal;
  }

  switch (battery_level_) {
    case BatteryLevel::critical:
      alarms_manager.activate_alarm(
          LogEventCode_battery_critical, LogEventType::LogEventType_system);
      alarms_manager.deactivate_alarm(LogEventCode_battery_low);
      break;
    case BatteryLevel::low:
      alarms_manager.activate_alarm(LogEventCode_battery_low, LogEventType::LogEventType_system);
      alarms_manager.deactivate_alarm(LogEventCode_battery_critical);
      break;
    case BatteryLevel::normal:
      alarms_manager.deactivate_alarm(LogEventCode_battery_low);
      alarms_manager.deactivate_alarm(LogEventCode_battery_critical);
    default:
      break;
  }
}

}  // namespace Pufferfish::Driver::Power
