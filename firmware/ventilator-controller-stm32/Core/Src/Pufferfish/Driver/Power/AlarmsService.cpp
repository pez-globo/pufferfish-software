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
    const MCUPowerStatus &mcu_power_status,
    ActiveLogEvents &active_log_events,
    Application::AlarmsManager &alarms_manager) {
  if (!mcu_power_status.charging) {
    alarms_manager.activate_alarm(
        LogEventCode_charger_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_charger_disconnected);
  }
  alarms_manager.transform(active_log_events);
}

}  // namespace Pufferfish::Driver::Power
