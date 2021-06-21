/*
 * SensorAlarmsService.h
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

namespace Pufferfish::Driver::BreathingCircuit {

struct SensorConnections {
  bool sfm3019_air_connected;
  bool sfm3019_o2_connected;
};

static constexpr auto alarm_codes = Util::Containers::make_array<LogEventCode>(
    LogEventCode::LogEventCode_sfm3019_air_disconnected,
    LogEventCode::LogEventCode_sfm3019_o2_disconnected);

class SensorAlarmsService {
 public:
  static void transform(
      const SensorConnections &sensor_connections, Application::AlarmsManager &alarms_manager);
};

}  // namespace Pufferfish::Driver::BreathingCircuit
