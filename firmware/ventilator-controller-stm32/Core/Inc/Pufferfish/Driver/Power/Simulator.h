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
#include "Pufferfish/Driver/I2C/LTC4015/Sensor.h"

namespace Pufferfish::Driver::Power {

class Simulator {
 public:
  explicit Simulator(Driver::I2C::LTC4015::Sensor &sensor) : ltc4015_sensor_(sensor) {}

  void transform(PowerManagement &power_management);
  void transform_charge(PowerManagement &power_management) const;
  void transform_discharge(PowerManagement &power_management) const;

 private:
  const float power_responsiveness = 0.5;
  static const int max_charge = 100;
  Driver::I2C::LTC4015::Sensor ltc4015_sensor_;
};

}  // namespace Pufferfish::Driver::Power
