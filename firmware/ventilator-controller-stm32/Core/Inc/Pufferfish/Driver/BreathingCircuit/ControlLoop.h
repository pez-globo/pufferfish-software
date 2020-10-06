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
  ControlLoop() = default;

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
      SensorVars &sensor_vars,
      Driver::I2C::SFM3019::Sensor &sfm3019_air,
      Driver::I2C::SFM3019::Sensor &sfm3019_o2,
      ActuatorVars &actuator_vars,
      HAL::PWM &valve)
      : parameters_(parameters),
        sensor_measurements_(sensor_measurements),
        controller_(parameters, sensor_measurements, actuator_vars),
        sensor_vars_(sensor_vars),
        sfm3019_air_(sfm3019_air),
        sfm3019_o2_(sfm3019_o2),
        actuator_vars_(actuator_vars),
        valve_(valve) {}

  void update(uint32_t current_time) override;

 private:
  const Parameters &parameters_;
  SensorMeasurements &sensor_measurements_;

  HFNCController controller_;

  // SensorVars
  SensorVars &sensor_vars_;
  Driver::I2C::SFM3019::Sensor &sfm3019_air_;
  Driver::I2C::SFM3019::Sensor &sfm3019_o2_;

  // ActuatorVars
  ActuatorVars &actuator_vars_;
  HAL::PWM &valve_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
