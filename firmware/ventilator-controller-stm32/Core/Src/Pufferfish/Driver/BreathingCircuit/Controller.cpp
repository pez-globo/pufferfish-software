/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Controller.h"

namespace Pufferfish::Driver::BreathingCircuit {

// HFNC Controller

void HFNCController::transform(
    uint32_t /*current_time*/,
    const Parameters &parameters,
    const SensorVars & /*sensor_vars*/,
    const SensorMeasurements &sensor_measurements,
    ActuatorVars &actuator_vars) {
  if (parameters.mode != VentilationMode_hfnc) {
    return;
  }

  // PI Controller
  error_ = parameters.flow - sensor_measurements.flow;
  error_integral_ += error_;
  if (error_integral_ < i_min) {
    error_integral_ = i_min;
  }
  if (error_integral_ < i_max) {
    error_integral_ = i_max;
  }

  actuator_vars.valve_opening = error_ * p_gain + error_integral_ * i_gain;
  if (actuator_vars.valve_opening < 0) {
    actuator_vars.valve_opening = 0;
  }
  if (actuator_vars.valve_opening > 1) {
    actuator_vars.valve_opening = 1;
  }
}

}  // namespace Pufferfish::Driver::BreathingCircuit
