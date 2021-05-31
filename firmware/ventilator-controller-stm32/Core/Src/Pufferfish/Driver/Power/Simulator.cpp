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

// NOLINTNEXTLINE(cert-msc51-cpp)
std::ranlux24_base prng;
std::uniform_int_distribution<int> uniform_int(1, 100);
static constexpr float uniform_width = 1;
std::uniform_real_distribution<float> uniform_centered(-uniform_width / 2, uniform_width / 2);

void Simulator::transform(PowerManagement &power_management) {
  InitializableState status = ltc4015_sensor_.output(power_management);
  if (status != InitializableState::ok) {
    transform_charge(power_management);
  }
}

void Simulator::transform_charge(PowerManagement &power_management) const {
  power_management.power_left +=
      (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);
  power_management.charging = true;
  if (power_management.power_left > max_charge) {
    power_management.power_left = max_charge;
  }
  if (power_management.power_left == max_charge) {
    power_management.charging = false;
  }
}

// void Simulator::transform_discharge(PowerManagement &power_management) const {
//   power_management.power_left -=
//       (1 + (power_responsiveness * uniform_centered(prng))) / uniform_int(prng);

//   if (power_management.power_left < 0) {
//     power_management.power_left = 0;
//   }
//   if (power_management.power_left == 0) {
//     power_management.charging = false;
//   }
// }

}  // namespace Pufferfish::Driver::Power
