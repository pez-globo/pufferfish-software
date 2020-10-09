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
    const SensorVars &sensor_vars,
    const SensorMeasurements & /*sensor_measurements*/,
    ActuatorSetpoints &actuator_setpoints,
    ActuatorVars &actuator_vars) {
  if (parameters.mode != VentilationMode_hfnc) {
    return;
  }

  // Open-loop setpoints
  float flow_o2_ratio = (parameters.fio2 - fio2_min) / (fio2_max - fio2_min);
  actuator_setpoints.flow_o2 = flow_o2_ratio * parameters.flow;
  actuator_setpoints.flow_air = parameters.flow - actuator_setpoints.flow_o2;

  // PI Controller
  valve_air_.transform(
      sensor_vars.flow_air, actuator_setpoints.flow_air, actuator_vars.valve_air_opening);
  valve_o2_.transform(
      sensor_vars.flow_o2, actuator_setpoints.flow_o2, actuator_vars.valve_o2_opening);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
