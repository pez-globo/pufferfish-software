/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include <cstdint>

#include "Algorithms.h"
#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

struct SensorVars {
  float flow_air;  // L/min
  float flow_o2;   // L/min
  uint32_t po2;    // dPa
};

struct ActuatorSetpoints {
  float flow_air;
  float flow_o2;
};

struct ActuatorVars {
  float valve_air_opening;
  float valve_o2_opening;
};

static const uint8_t fio2_min = 21;
static const uint8_t fio2_max = 100;

class Controller {
 public:
  virtual void transform(
      uint32_t current_time,
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorMeasurements &sensor_measurements,
      ActuatorSetpoints &actuator_setpoints,
      ActuatorVars &actuator_vars) = 0;
};

class HFNCController : public Controller {
 public:
  void transform(
      uint32_t current_time,
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorMeasurements &sensor_measurements,
      ActuatorSetpoints &actuator_setpoints,
      ActuatorVars &actuator_vars) override;

 private:
  PI valve_o2_{};
  PI valve_air_{};
};

}  // namespace Pufferfish::Driver::BreathingCircuit
