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
  void transform(uint32_t current_time, PowerManagement &power_management);
  void transform_charge(PowerManagement &power_management) const;
  void transform_discharge(PowerManagement &power_management) const;

 private:
  const float power_responsiveness = 0.5;
  static const int max_charge = 100;
  static const uint32_t sensor_update_interval = 2;  // ms

  uint32_t current_time_ = 0;   // ms
  uint32_t previous_time_ = 0;  // ms
  uint32_t initial_time_ = 0;   // ms

  void input_clock(uint32_t current_time);
  [[nodiscard]] bool update_needed() const;
};

}  // namespace Pufferfish::Driver::Power
