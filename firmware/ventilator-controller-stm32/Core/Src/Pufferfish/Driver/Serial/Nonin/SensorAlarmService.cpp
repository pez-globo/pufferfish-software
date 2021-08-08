/*
 * SensorAlarmService.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/SensorAlarmService.h"

namespace Pufferfish::Driver::Serial::Nonin {

using Application::LogEventType;
const LogEventType system = LogEventType::LogEventType_system;

void SensorAlarmsService::transform(
    InitializableState status,
    const SensorConnections &sensor_connections,
    Application::AlarmsManager &alarms_manager) {
  status == InitializableState::failed
      ? alarms_manager.activate_alarm(Application::LogEventCode_nonin_disconnected, system)
      : alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);

  sensor_connections.sensor_alarm
      ? alarms_manager.activate_alarm(Application::LogEventCode_nonin_disconnected, system)
      : alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);

  sensor_connections.sensor_disconnected
      ? alarms_manager.activate_alarm(Application::LogEventCode_nonin_disconnected, system)
      : alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);

  sensor_connections.spo2_out_of_track
      ? alarms_manager.activate_alarm(Application::LogEventCode_nonin_disconnected, system)
      : alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);

  sensor_connections.hr_out_of_track
      ? alarms_manager.activate_alarm(Application::LogEventCode_nonin_disconnected, system)
      : alarms_manager.deactivate_alarm(Application::LogEventCode_nonin_disconnected);
}

}  // namespace Pufferfish::Driver::Serial::Nonin
