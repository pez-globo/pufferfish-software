/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include <cstdint>

#include "Controller.h"
#include "ParametersService.h"
#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"
#include "Pufferfish/HAL/Interfaces/PWM.h"

namespace Pufferfish::Driver::BreathingCircuit {

class ControlLoop {
 public:
  virtual void update(uint32_t current_time) = 0;

 protected:
  static const uint32_t update_interval = 2;  // ms

  void advance_step_time(uint32_t current_time);
  [[nodiscard]] uint32_t step_duration(uint32_t current_time) const;
  [[nodiscard]] bool update_needed(uint32_t current_time) const;

 private:
  uint32_t previous_step_time_ = 0;  // ms
};

class HFNCControlLoop : public ControlLoop {
 public:
  HFNCControlLoop(
      const Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      Driver::I2C::SFM3019::Sensor &sfm3019_air,
      Driver::I2C::SFM3019::Sensor &sfm3019_o2,
      HAL::PWM &valve_air,
      HAL::PWM &valve_o2)
      : parameters_(parameters),
        sensor_measurements_(sensor_measurements),
        sfm3019_air_(sfm3019_air),
        sfm3019_o2_(sfm3019_o2),
        valve_air_(valve_air),
        valve_o2_(valve_o2) {}

  void update(uint32_t current_time) override;

  [[nodiscard]] const SensorVars &sensor_vars() const;
  [[nodiscard]] const ActuatorSetpoints &actuator_setpoints() const;
  [[nodiscard]] const ActuatorVars &actuator_vars() const;

 private:
  const Parameters &parameters_;
  SensorMeasurements &sensor_measurements_;

  HFNCController controller_;

  // SensorVars
  SensorVars sensor_vars_{};
  Driver::I2C::SFM3019::Sensor &sfm3019_air_;
  Driver::I2C::SFM3019::Sensor &sfm3019_o2_;

  // Setpoints
  ActuatorSetpoints actuator_setpoints_{};

  // ActuatorVars
  ActuatorVars actuator_vars_{};
  HAL::PWM &valve_air_;
  HAL::PWM &valve_o2_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
