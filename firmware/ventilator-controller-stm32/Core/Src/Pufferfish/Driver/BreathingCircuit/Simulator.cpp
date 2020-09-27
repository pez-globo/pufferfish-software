/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Simulator.h"

namespace Pufferfish::BreathingCircuit {

Simulator::Simulator(
    const ParametersRequest &parameters_request,
    Parameters &parameters,
    SensorMeasurements &sensor_measurements,
    CycleMeasurements &cycle_measurements)
    : parameters_request_(parameters_request),
      parameters_(parameters),
      sensor_measurements_(sensor_measurements),
      cycle_measurements_(cycle_measurements) {}

void Simulator::update_clock(uint32_t current_time) {
  this->current_time_ = current_time;
}

void Simulator::update_sensors() {
  if (current_time_ - previous_time_ > sensor_update_interval) {
    // Parameters
    if (parameters_request_.rr > 0) {
      parameters_.rr = parameters_request_.rr;
    }
    if (parameters_request_.ie > 0) {
      parameters_.ie = parameters_request_.ie;
    }
    if (parameters_request_.pip > 0) {
      parameters_.pip = parameters_request_.pip;
    }
    parameters_.peep = parameters_request_.peep;
    parameters_.fio2 = parameters_request_.fio2;
    // Timing
    previous_time_ = current_time_;
    sensor_measurements_.time = current_time_;
    cycle_period_ = 60000.0 / parameters_.rr;
    if (current_time_ - cycle_start_time_ > cycle_period_) {
      cycle_start_time_ = current_time_;
      sensor_measurements_.flow = insp_init_flow_rate;
    }
    insp_period_ = cycle_period_ / (1 + 1.0 / parameters_.ie);
    // Airway
    if (current_time_ - cycle_start_time_ < insp_period_) {
      sensor_measurements_.paw += (
          parameters_.pip - sensor_measurements_.paw
      ) * insp_responsiveness / sensor_update_interval;
      sensor_measurements_.flow *= (1 - insp_flow_responsiveness / sensor_update_interval);
    } else {
      sensor_measurements_.paw +=
          (parameters_.peep - sensor_measurements_.paw) * exp_responsiveness / sensor_update_interval;
      if (sensor_measurements_.flow >= 0) {
        sensor_measurements_.flow = exp_init_flow_rate;
      } else {
        sensor_measurements_.flow *= (1 - exp_flow_responsiveness / sensor_update_interval);
      }
    }
    // FiO2
    sensor_measurements_.fio2 += (
        parameters_.fio2 - sensor_measurements_.fio2
    ) * fio2_responsiveness / sensor_update_interval;
    // Cycle Measurements
    cycle_measurements_.time = current_time_;
    cycle_measurements_.rr = parameters_.rr;
    cycle_measurements_.peep = parameters_.peep;
    cycle_measurements_.pip = parameters_.pip;
    cycle_measurements_.ip = sensor_measurements_.paw - parameters_.peep;
  }
}

void Simulator::update_actuators() {}

}  // namespace Pufferfish::BreathingCircuit
