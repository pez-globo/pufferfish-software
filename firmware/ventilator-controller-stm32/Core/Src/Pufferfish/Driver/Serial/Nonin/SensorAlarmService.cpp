/*
 * SensorAlarmService.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/SensorAlarmService.h"

namespace Pufferfish::Driver::Serial::Nonin {

using Application::LogEventCode;
using Application::LogEventType;
const LogEventType system = LogEventType::LogEventType_system;

void SensorAlarmsService::transform(
    InitializableState status,
    const SensorConnections &sensor_connections,
    Application::AlarmsManager &alarms_manager) {
  if (status == InitializableState::failed) {
    alarms_manager.activate_alarm(LogEventCode::LogEventCode_nonin_disconnected, system);
    return;
  }

  if (status != InitializableState::failed) {
    alarms_manager.deactivate_alarm(LogEventCode::LogEventCode_nonin_disconnected);
  }

  if (sensor_connections.finger_sensor_disconnected) {
    alarms_manager.activate_alarm(
        LogEventCode::LogEventCode_nonin_finger_sensor_disconnected, system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode::LogEventCode_nonin_finger_sensor_disconnected);
  }

  if (sensor_connections.sensor_alarm) {
    alarms_manager.activate_alarm(LogEventCode::LogEventCode_nonin_sensor_alarm, system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode::LogEventCode_nonin_sensor_alarm);
  }

  if (sensor_connections.out_of_track) {
    alarms_manager.activate_alarm(
        LogEventCode::LogEventCode_nonin_out_of_track_measurements, system);
  } else {
    alarms_manager.deactivate_alarm(LogEventCode::LogEventCode_nonin_out_of_track_measurements);
  }
}

}  // namespace Pufferfish::Driver::Serial::Nonin
