/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Simulator.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::BreathingCircuit {

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
  sensor_measurements_.fio2 +=
      (parameters_.fio2 - sensor_measurements_.fio2) * fio2_responsiveness / sensor_update_interval;
}

bool Simulator::update_needed() const {
  return !Util::within_timeout(previous_time_, sensor_update_interval, current_time_);
}

// PC-AC Simulator

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
  sensor_measurements_.paw +=
      (parameters_.pip - sensor_measurements_.paw) * insp_responsiveness / time_step();
  sensor_measurements_.flow *= (1 - insp_flow_responsiveness / time_step());
  sensor_measurements_.volume +=
      static_cast<float>(sensor_measurements_.flow / min_per_s * time_step());
}

void PCACSimulator::update_airway_expiratory() {
  sensor_measurements_.paw +=
      (parameters_.peep - sensor_measurements_.paw) * exp_responsiveness / time_step();
  if (sensor_measurements_.flow >= 0) {
    sensor_measurements_.flow = exp_init_flow_rate;
  } else {
    sensor_measurements_.flow *= (1 - exp_flow_responsiveness / time_step());
  }
  sensor_measurements_.volume += sensor_measurements_.flow / min_per_s * time_step();
}

// HFNC Simulator

void HFNCSimulator::update_sensors() {
  if (!update_needed()) {
    return;
  }

  if (parameters_.mode != VentilationMode_hfnc) {
    return;
  }

  // Timing
  sensor_measurements_.time = current_time_;
  uint32_t cycle_period = minute_duration / parameters_.rr;
  if (!Util::within_timeout(cycle_start_time_, cycle_period, current_time_)) {
    init_cycle();
    update_cycle_measurements();
  }
  update_flow();
  update_fio2();
  update_spo2();
}

void HFNCSimulator::init_cycle() {
  cycle_start_time_ = current_time_;
}

void HFNCSimulator::update_cycle_measurements() {
  cycle_measurements_.rr = parameters_.rr;
}

void HFNCSimulator::update_flow() {
  sensor_measurements_.flow +=
      (parameters_.flow - sensor_measurements_.flow) * flow_responsiveness / time_step();
}

void HFNCSimulator::update_spo2() {
  sensor_measurements_.spo2 +=
      (spo2_fio2_scale * sensor_measurements_.fio2 - sensor_measurements_.spo2) *
      spo2_responsiveness / time_step();
  if (sensor_measurements_.spo2 < spo2_min) {
    sensor_measurements_.spo2 = spo2_min;
  }
  if (sensor_measurements_.spo2 > spo2_max) {
    sensor_measurements_.spo2 = spo2_max;
  }
}

// Simulators

void Simulators::update_clock(uint32_t current_time) {
  if (active_simulator_ == nullptr) {
    return;
  }

  pc_ac_.update_clock(current_time);
  hfnc_.update_clock(current_time);
}

void Simulators::update_sensors() {
  switch (parameters_.mode) {
    case VentilationMode_pc_ac:
      active_simulator_ = &pc_ac_;
      break;
    case VentilationMode_hfnc:
      active_simulator_ = &hfnc_;
      break;
    default:
      active_simulator_ = nullptr;
      return;
  }

  active_simulator_->update_sensors();
}

}  // namespace Pufferfish::Driver::BreathingCircuit
