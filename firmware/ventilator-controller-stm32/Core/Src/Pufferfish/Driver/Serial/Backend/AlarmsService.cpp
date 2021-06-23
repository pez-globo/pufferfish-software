/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 */

#include "Pufferfish/Driver/Serial/Backend/AlarmsService.h"

namespace Pufferfish::Driver::Serial::Backend {

// AlarmsService
void AlarmsService::transform(
    bool connected,
    Application::AlarmsManager &alarms_manager,
    Application::LogEventsManager &log_manager) {
  using EdgeState = Protocols::Application::EdgeDetector::State;
  EdgeState edge;
  edge_.transform(connected, edge);
  switch (edge) {
    case EdgeState::rising_edge:
      alarms_manager.deactivate_alarm(LogEventCode_mcu_backend_connection_down);
      log_manager.add_event(
          LogEventCode_mcu_backend_connection_up, LogEventType::LogEventType_system);
      break;
    case EdgeState::falling_edge:
      alarms_manager.activate_alarm(
          LogEventCode_mcu_backend_connection_down, LogEventType::LogEventType_system);
      break;
    default:
      return;
  }
}

}  // namespace Pufferfish::Driver::Serial::Backend
