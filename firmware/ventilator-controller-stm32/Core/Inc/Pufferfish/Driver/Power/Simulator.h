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
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Power {

using Application::MCUPowerStatus;

class Simulator {
 public:
  void transform(uint32_t current_time, MCUPowerStatus &mcu_power_status);

 private:
  void transform_charge(MCUPowerStatus &mcu_power_status);
  void transform_discharge(MCUPowerStatus &mcu_power_status);

  static const int max_charge = 100;
  static const uint32_t sensor_update_interval = 1000;  // ms
  bool charging_ = true;

  uint32_t current_time_ = 0;  // ms
  uint32_t initial_time_ = 0;  // ms

  void input_clock(uint32_t current_time);
  [[nodiscard]] bool update_needed() const;
  Util::MsTimer step_timer_{sensor_update_interval, 0};
};

}  // namespace Pufferfish::Driver::Power
