/*
 * Simulator.cpp
 *
 *  Created on: May 30, 2021
 *      Author: Rohan
 *
 *  Simulator for power management
 */

#include "Pufferfish/Driver/Power/Simulator.h"

namespace Pufferfish::Driver::Power {

void Simulator::input_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time;
  }
  if (update_needed()) {
    step_timer_.reset(current_time_);
  }
  current_time_ = current_time - initial_time_;
}

void Simulator::transform(uint32_t current_time, MCUPowerStatus &mcu_power_status) {
  input_clock(current_time);
  if (charging_) {
    transform_charge(mcu_power_status);
  } else {
    transform_discharge(mcu_power_status);
  }
}

void Simulator::transform_charge(MCUPowerStatus &mcu_power_status) {
  if (!update_needed()) {
    return;
  }
  mcu_power_status.charging = true;
  mcu_power_status.power_left += 1;
  if (mcu_power_status.power_left >= max_charge) {
    mcu_power_status.power_left = max_charge;
    charging_ = false;
  }
}

void Simulator::transform_discharge(MCUPowerStatus &mcu_power_status) {
  if (!update_needed()) {
    return;
  }
  mcu_power_status.charging = false;
  mcu_power_status.power_left -= 1;
  if (mcu_power_status.power_left <= 0) {
    mcu_power_status.power_left = 0;
    charging_ = true;
  }
}

bool Simulator::update_needed() const {
  return !step_timer_.within_timeout(current_time_);
}

}  // namespace Pufferfish::Driver::Power
