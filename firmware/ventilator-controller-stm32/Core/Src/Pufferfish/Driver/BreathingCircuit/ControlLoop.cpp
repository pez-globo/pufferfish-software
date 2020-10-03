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

void HFNCControlLoop::update(uint32_t current_time) {
  if (!update_needed(current_time)) {
    return;
  }

  if (parameters_.mode != VentilationMode_hfnc) {
    return;
  }

  // Update sensors
  // TODO(lietk12): handle errors from sensors
  sfm3019_.update();

  // Update controller
  controller_.update(current_time);

  // Update actuators
  valve_.set_duty_cycle(actuators_.valve_opening);

  advance_step_time(current_time);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
