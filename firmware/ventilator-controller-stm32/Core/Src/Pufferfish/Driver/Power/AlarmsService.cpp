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
    const Parameters &parameters,
    PowerManagement &power_management,
    ActiveLogEvents &active_log_events,
    Application::AlarmsManager &alarms_manager) {
  if (!parameters.ventilating) {
    deactivate_alarms(active_log_events, alarms_manager);
    return;
  }

  if (!power_management.charging) {
    alarms_manager.activate_alarm(
        LogEventCode_charger_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_charger_disconnected);
  }
  alarms_manager.transform(active_log_events);
}

void AlarmsService::deactivate_alarms(
    ActiveLogEvents &active_log_events, Application::AlarmsManager &alarms_manager) {
  for (size_t i = 0; i < Application::alarm_codes.max_size(); ++i) {
    alarms_manager.deactivate_alarm(Application::alarm_codes[i]);
  }
  alarms_manager.transform(active_log_events);
}

}  // namespace Pufferfish::Driver::Power
