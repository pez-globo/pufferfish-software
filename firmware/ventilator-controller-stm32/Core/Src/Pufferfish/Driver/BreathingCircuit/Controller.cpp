/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Controller.h"

namespace Pufferfish::Driver::BreathingCircuit {

// Controller

const Parameters &Controller::parameters() const {
  return parameters_;
}

const SensorMeasurements &Controller::sensor_measurements() const {
  return sensor_measurements_;
}

ActuatorVars &Controller::actuator_vars() {
  return actuator_vars_;
}

// HFNC Controller

void HFNCController::update(uint32_t /*current_time*/) {
  if (parameters().mode != VentilationMode_hfnc) {
    return;
  }

  // PI Controller
  error_ = parameters().flow - sensor_measurements().flow;
  error_integral_ += error_;
  if (error_integral_ < i_min) {
    error_integral_ = i_min;
  }
  if (error_integral_ < i_max) {
    error_integral_ = i_max;
  }

  actuator_vars().valve_opening = error_ * p_gain + error_integral_ * i_gain;
  if (actuator_vars().valve_opening < 0) {
    actuator_vars().valve_opening = 0;
  }
  if (actuator_vars().valve_opening > 1) {
    actuator_vars().valve_opening = 1;
  }
}

}  // namespace Pufferfish::Driver::BreathingCircuit
