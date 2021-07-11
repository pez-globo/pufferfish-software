/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/ControlLoop.h"

namespace Pufferfish::Driver::BreathingCircuit {

// ControlLoop

// HFNC ControlLoop

SensorVars &HFNCControlLoop::sensor_vars() {
  return sensor_vars_;
}

const SensorVars &HFNCControlLoop::sensor_vars() const {
  return sensor_vars_;
}

const ActuatorVars &HFNCControlLoop::actuator_vars() const {
  return actuator_vars_;
}

const SensorConnections &HFNCControlLoop::sensor_connections() const {
  return sensor_connections_;
}

void HFNCControlLoop::update(uint32_t current_time) {
  if (step_timer().within_timeout(current_time)) {
    return;
  }

  if (parameters_.mode != Application::VentilationMode_hfnc) {
    return;
  }

  // Update sensors
  InitializableState air_status = sfm3019_air_.output(sensor_vars_.flow_air);
  InitializableState o2_status = sfm3019_o2_.output(sensor_vars_.flow_o2);
  if (air_status == InitializableState::ok && o2_status == InitializableState::ok) {
    sensor_measurements_.flow = sensor_vars_.flow_air + sensor_vars_.flow_o2;
  }
  // TODO(lietk12): we should probably set flow to NaN otherwise, but for now we do nothing
  // so that we don't overwrite the simulated values if the sensors aren't available

  sensor_connections_.sfm3019_air_connected = air_status == InitializableState::ok;
  sensor_connections_.sfm3019_o2_connected = o2_status == InitializableState::ok;

  // Update controller
  controller_.transform(
      current_time,
      parameters_,
      sensor_vars_,
      sensor_measurements_,
      actuator_setpoints_,
      actuator_vars_);

  // Update actuators
  valve_air_.set_duty_cycle(actuator_vars_.valve_air_opening);
  valve_o2_.set_duty_cycle(actuator_vars_.valve_o2_opening);

  step_timer().reset(current_time);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
