/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Simulator.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::BreathingCircuit {

// Simulator

void Simulator::update_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time_;
  }
  if (update_needed()) {
    previous_time_ = current_time_;
  }
  current_time_ = current_time - initial_time_;
}

void Simulator::update_fio2() {
  sensor_measurements_.fio2 += (parameters_.fio2 - sensor_measurements_.fio2) *
                               fio2_responsiveness / sensor_update_interval;
}

bool Simulator::update_needed() const {
  return !Util::within_timeout(
       previous_time_, sensor_update_interval, current_time_);
}

// PC-AC Simulator

void PCACSimulator::update_parameters() {
  parameters_.mode = parameters_request_.mode;
  if (parameters_.mode != VentilationMode_pc_ac) {
    return;
  }
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
}

void PCACSimulator::update_sensors() {
  if (!update_needed()) {
    return;
  }

  if (parameters_.mode != VentilationMode_pc_ac) {
    return;
  }

  // Timing
  sensor_measurements_.time = current_time_;
  uint32_t cycle_period = minute_duration / parameters_.rr;
  if (!Util::within_timeout(cycle_start_time_, cycle_period, current_time_)) {
    init_cycle(cycle_period);
    update_cycle_measurements();
  }
  if (Util::within_timeout(cycle_start_time_, insp_period_, current_time_)) {
    update_airway_inspiratory();
  } else {
    update_airway_expiratory();
  }
  update_fio2();
}

void PCACSimulator::init_cycle(uint32_t cycle_period) {
  cycle_start_time_ = current_time_;
  sensor_measurements_.flow = insp_init_flow_rate;
  sensor_measurements_.volume = 0;
  insp_period_ = cycle_period / (1 + 1.0 / parameters_.ie);
  sensor_measurements_.cycle += 1;
}

void PCACSimulator::update_cycle_measurements() {
  cycle_measurements_.time = current_time_;
  cycle_measurements_.rr = parameters_.rr;
  cycle_measurements_.peep = parameters_.peep;
  cycle_measurements_.pip = parameters_.pip;
}

void PCACSimulator::update_airway_inspiratory() {
  sensor_measurements_.paw += (parameters_.pip - sensor_measurements_.paw) *
                              insp_responsiveness / time_step();
  sensor_measurements_.flow *= (1 - insp_flow_responsiveness / time_step());
  sensor_measurements_.volume += sensor_measurements_.flow / 60.0 * time_step();
}

void PCACSimulator::update_airway_expiratory() {
  sensor_measurements_.paw += (parameters_.peep - sensor_measurements_.paw) *
                              exp_responsiveness / time_step();
  if (sensor_measurements_.flow >= 0) {
    sensor_measurements_.flow = exp_init_flow_rate;
  } else {
    sensor_measurements_.flow *= (1 - exp_flow_responsiveness / time_step());
  }
  sensor_measurements_.volume += sensor_measurements_.flow / 60.0 * time_step();
}

void PCACSimulator::update_actuators() {}

}  // namespace Pufferfish::BreathingCircuit
