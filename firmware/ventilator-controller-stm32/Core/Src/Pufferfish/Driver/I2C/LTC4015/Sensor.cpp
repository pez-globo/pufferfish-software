/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/I2C/LTC4015/Sensor.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

void Sensor::trigger_alarm(
    BatteryPower &battery_power,
    ActiveLogEvents &active_log_events,
    Application::AlarmsManager &alarms_manager) {
  // check if charger is connected
  device_.is_charger_enabled(battery_power);

  if (!battery_power.charging_status) {
    alarms_manager.activate_alarm(
        LogEventCode_charger_disconnected, LogEventType::LogEventType_patient);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_charger_disconnected);
  }
  alarms_manager.transform(active_log_events);
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
