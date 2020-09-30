/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Controller.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::BreathingCircuit {

// Controller

void Controller::update_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time_;
  }
  if (update_needed()) {
    previous_time_ = current_time_;
  }
  current_time_ = current_time - initial_time_;
}

bool Controller::update_needed() const {
  return !Util::within_timeout(previous_time_, update_interval, current_time_);
}

// HFNC Controller

void HFNCController::update_parameters() {
  parameters_.mode = parameters_request_.mode;
  if (parameters_.mode != VentilationMode_hfnc) {
    return;
  }
  if (parameters_request_.flow >= 0) {
    parameters_.flow = parameters_request_.flow;
  }
  if (parameters_request_.fio2 >= fio2_min && parameters_request_.fio2 <= fio2_max) {
    parameters_.fio2 = parameters_request_.fio2;
  }
}

void HFNCController::update_actuators() {
  if (!update_needed()) {
    return;
  }

  if (parameters_.mode != VentilationMode_hfnc) {
    return;
  }

  // PI Controller
  error_ = parameters_.flow - sensor_measurements_.flow;
  error_integral_ += error_;
  if (error_integral_ < i_min) {
    error_integral_ = i_min;
  }
  if (error_integral_ < i_max) {
    error_integral_ = i_max;
  }

  actuators_.valve_opening = error_ * p_gain + error_integral_ * i_gain;
  if (actuators_.valve_opening < 0) {
    actuators_.valve_opening = 0;
  }
  if (actuators_.valve_opening > 1) {
    actuators_.valve_opening = 1;
  }
}

}  // namespace Pufferfish::BreathingCircuit
