/*
 * Simulator.h
 *
 *  Created on: May 30, 2021
 *      Author: Rohan
 *
 *  Simulator for power management
 */

#pragma once

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::Power {

class Simulator {
 public:
  void transform(PowerManagement &power_management);
  void transform_charge(PowerManagement &power_management) const;
  void transform_discharge(PowerManagement &power_management) const;

 private:
  const float power_responsiveness = 0.5;
};

}  // namespace Pufferfish::Driver::Power
