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
      Driver::I2C::SFM3019::Sensor &sfm3019,
      Actuators &actuators,
      HAL::PWM &valve)
      : parameters_(parameters),
        controller_(parameters, sensor_measurements, actuators),
        sfm3019_(sfm3019),
        actuators_(actuators),
        valve_(valve) {}

  void update(uint32_t current_time) override;

 private:
  const Parameters &parameters_;
  HFNCController controller_;

  // Sensors
  Driver::I2C::SFM3019::Sensor &sfm3019_;

  // Actuators
  Actuators actuators_;
  HAL::PWM &valve_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
