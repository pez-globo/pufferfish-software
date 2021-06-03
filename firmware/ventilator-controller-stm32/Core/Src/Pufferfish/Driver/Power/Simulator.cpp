/*
 * Simulator.cpp
 *
 *  Created on: May 30, 2021
 *      Author: Rohan
 *
 *  Simulator for power management
 */

#include "Pufferfish/Driver/Power/Simulator.h"

#include <cmath>
#include <random>

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Power {

// NOLINTNEXTLINE(cert-msc51-cpp)
std::ranlux24_base prng;
std::uniform_int_distribution<int> uniform_int(1, 100);
static constexpr float uniform_width = 1;
std::uniform_real_distribution<float> uniform_centered(-uniform_width / 2, uniform_width / 2);

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
  transform_charge(power_management);
}

void Simulator::transform_charge(PowerManagement &power_management) const {
  if (!update_needed()) {
    return;
  }
  power_management.power_left +=
      (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);
  power_management.charging = true;
  if (power_management.power_left >= max_charge) {
    power_management.power_left = max_charge;
    power_management.charging = false;
  }
}

void Simulator::transform_discharge(PowerManagement &power_management) const {
  if (!update_needed()) {
    return;
  }
  power_management.power_left -=
      (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);

  power_management.charging = false;
  if (power_management.power_left < 0) {
    power_management.power_left = 0;
  }
  if (power_management.power_left == 0) {
    power_management.charging = true;
  }
}

bool Simulator::update_needed() const {
  return !Util::within_timeout(previous_time_, sensor_update_interval, current_time_);
}

}  // namespace Pufferfish::Driver::Power
