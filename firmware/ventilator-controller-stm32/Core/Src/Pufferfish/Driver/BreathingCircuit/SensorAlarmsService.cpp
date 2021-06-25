/*
 * SensorAlarmsService.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/SensorAlarmsService.h"

namespace Pufferfish::Driver::BreathingCircuit {

void SensorAlarmsService::transform(
    const SensorConnections &sensor_connections, Application::AlarmsManager &alarms_manager) {
  if (!sensor_connections.sfm3019_air_connected) {
    alarms_manager.activate_alarm(
        LogEventCode_sfm3019_air_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_sfm3019_air_disconnected);
  }
  if (!sensor_connections.sfm3019_o2_connected) {
    alarms_manager.activate_alarm(
        LogEventCode_sfm3019_o2_disconnected, LogEventType::LogEventType_system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode_sfm3019_o2_disconnected);
  }
}

}  // namespace Pufferfish::Driver::BreathingCircuit
