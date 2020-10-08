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

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

struct SensorVars {
  float flow_air;
  float flow_o2;
};

struct ActuatorVars {
  float valve_opening;
};

class Controller {
 public:
  virtual void transform(
      uint32_t current_time,
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorMeasurements &sensor_measurements,
      ActuatorVars &actuator_vars) = 0;
};

class HFNCController : public Controller {
 public:
  void transform(
      uint32_t current_time,
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorMeasurements &sensor_measurements,
      ActuatorVars &actuator_vars) override;

 private:
  static constexpr float p_gain = 0.00001;
  static constexpr float i_gain = 0.0002;
  static constexpr float i_max = 200 * i_gain;
  static constexpr float i_min = 0;

  float error_ = 0;
  float error_integral_ = 0;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
