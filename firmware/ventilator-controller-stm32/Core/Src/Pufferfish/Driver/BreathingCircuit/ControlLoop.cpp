/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/ControlLoop.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::BreathingCircuit {

// ControlLoop

void ControlLoop::advance_step_time(uint32_t current_time) {
  previous_step_time_ = current_time;
}

uint32_t ControlLoop::step_duration(uint32_t current_time) const {
  return current_time - previous_step_time_;
}

bool ControlLoop::update_needed(uint32_t current_time) const {
  return !Util::within_timeout(previous_step_time_, update_interval, current_time);
}

// HFNC ControlLoop

const SensorVars &HFNCControlLoop::sensor_vars() const {
  return sensor_vars_;
}

const ActuatorVars &HFNCControlLoop::actuator_vars() const {
  return actuator_vars_;
}

void HFNCControlLoop::update(uint32_t current_time) {
  if (!update_needed(current_time)) {
    return;
  }

  if (parameters_.mode != VentilationMode_hfnc) {
    return;
  }

  // Update sensors
  // TODO(lietk12): handle errors from sensors
  sfm3019_air_.output(sensor_vars_.flow_air);
  sfm3019_o2_.output(sensor_vars_.flow_o2);
  sensor_measurements_.flow = sensor_vars_.flow_air + sensor_vars_.flow_o2;

  // Update controller
  controller_.transform(
      current_time, parameters_, sensor_vars_, sensor_measurements_, actuator_vars_);

  // Update actuators
  valve_.set_duty_cycle(actuator_vars_.valve_opening);

  advance_step_time(current_time);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
