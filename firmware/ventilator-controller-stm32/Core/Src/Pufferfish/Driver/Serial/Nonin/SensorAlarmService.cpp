/*
 * SensorAlarmService.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/SensorAlarmService.h"

namespace Pufferfish::Driver::Serial::Nonin {

using Application::LogEventType;

void SensorAlarmsService::transform(
    const SensorConnections &sensor_connections, Application::AlarmsManager &alarms_manager) {
  if (!sensor_connections.nonin_connected) {
    alarms_manager.activate_alarm(
        Application::LogEventCode_nonin_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);
  }
}

}  // namespace Pufferfish::Driver::Serial::Nonin
