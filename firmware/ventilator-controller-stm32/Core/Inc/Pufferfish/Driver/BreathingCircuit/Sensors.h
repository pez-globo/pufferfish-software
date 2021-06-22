/*
 * SensorAlarmsService.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Alarms for sensor disconnections
 */

#pragma once

namespace Pufferfish::Driver::BreathingCircuit {

struct SensorConnections {
  bool sfm3019_air_connected;
  bool sfm3019_o2_connected;
};

}  // namespace Pufferfish::Driver::BreathingCircuit