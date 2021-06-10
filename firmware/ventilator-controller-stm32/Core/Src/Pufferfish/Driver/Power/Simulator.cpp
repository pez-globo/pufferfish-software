/*
 * Simulator.cpp
 *
 *  Created on: May 30, 2021
 *      Author: Rohan
 *
 *  Simulator for power management
 */

#include "Pufferfish/Driver/Power/Simulator.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Power {

void Simulator::input_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time;
  }
  if (update_needed()) {
    previous_time_ = current_time_;
  }
  current_time_ = current_time - initial_time_;
}

void Simulator::transform(uint32_t current_time, PowerManagement &power_management) {
  input_clock(current_time);
  if (charging_) {
    transform_charge(power_management);
  } else {
    transform_discharge(power_management);
  }
}

void Simulator::transform_charge(PowerManagement &power_management) {
  if (!update_needed()) {
    return;
  }
  power_management.charging = true;
  power_management.power_left += 1;
  if (power_management.power_left >= max_charge) {
    power_management.power_left = max_charge;
    charging_ = false;
  }
}

void Simulator::transform_discharge(PowerManagement &power_management) {
  if (!update_needed()) {
    return;
  }
  power_management.charging = false;
  power_management.power_left -= 1;
  if (power_management.power_left == 0) {
    charging_ = true;
  }
}

bool Simulator::update_needed() const {
  return !Util::within_timeout(previous_time_, sensor_update_interval, current_time_);
}

}  // namespace Pufferfish::Driver::Power
