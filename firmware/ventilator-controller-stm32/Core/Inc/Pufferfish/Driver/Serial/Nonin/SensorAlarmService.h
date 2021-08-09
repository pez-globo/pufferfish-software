/*
 * SensorAlarmService.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Alarms for sensor disconnections
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "Sensor.h"

namespace Pufferfish::Driver::Serial::Nonin {

using Application::LogEventCode;

static constexpr auto alarm_codes = Util::Containers::make_array<LogEventCode>(
    LogEventCode::LogEventCode_nonin_disconnected,
    LogEventCode::LogEventCode_nonin_finger_sensor_disconnected,
    LogEventCode::LogEventCode_nonin_sensor_alarm,
    LogEventCode::LogEventCode_nonin_out_of_track_measurements);

class SensorAlarmsService {
 public:
  static void transform(
      InitializableState status,
      const SensorConnections &sensor_connections,
      Application::AlarmsManager &alarms_manager);
};

}  // namespace Pufferfish::Driver::Serial::Nonin
