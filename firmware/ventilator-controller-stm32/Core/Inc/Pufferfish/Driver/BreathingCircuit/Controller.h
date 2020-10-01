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

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

struct Actuators {
  float valve_opening;
};

class Controller {
 public:
  Controller(
      const Parameters &parameters,
      const SensorMeasurements &sensor_measurements,
      Actuators &actuators)
      : parameters_(parameters), sensor_measurements_(sensor_measurements), actuators_(actuators) {}

  virtual void update(uint32_t current_time) = 0;

 protected:
  [[nodiscard]] const Parameters &parameters() const;
  [[nodiscard]] const SensorMeasurements &sensor_measurements() const;
  Actuators &actuators();

 private:
  const Parameters &parameters_;
  const SensorMeasurements &sensor_measurements_;
  Actuators &actuators_;
};

class HFNCController : public Controller {
 public:
  HFNCController(
      const Parameters &parameters,
      const SensorMeasurements &sensor_measurements,
      Actuators &actuators)
      : Controller(parameters, sensor_measurements, actuators) {}

  void update(uint32_t current_time) override;

 private:
  static constexpr float p_gain = 0.00001;
  static constexpr float i_gain = 0.0002;
  static constexpr float i_max = 200 * i_gain;
  static constexpr float i_min = 0;

  float error_ = 0;
  float error_integral_ = 0;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
