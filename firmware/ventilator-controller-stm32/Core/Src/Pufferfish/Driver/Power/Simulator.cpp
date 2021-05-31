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

namespace Pufferfish::Driver::Power {

std::ranlux24_base prng;
std::uniform_int_distribution<int> uniform_int(1, 100);
static constexpr float uniform_width = 1;
std::uniform_real_distribution<float> uniform(0, uniform_width);
std::uniform_real_distribution<float> uniform_centered(-uniform_width / 2, uniform_width / 2);

void Simulator::transform(PowerManagement &power_management) {
  if (!power_management.charging) {
    transform_charge(power_management);
  } else {
    transform_discharge(power_management);
  }
}

void Simulator::transform_charge(PowerManagement &power_management) const {
  power_management.power_left +=
      (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);
  if (power_management.power_left == 100) {
    power_management.charging = false;
  }
}
void Simulator::transform_discharge(PowerManagement &power_management) const {
  power_management.power_left -=
      (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);
  if (power_management.power_left == 0) {
    power_management.charging = true;
  }
}

}  // namespace Pufferfish::Driver::Power
