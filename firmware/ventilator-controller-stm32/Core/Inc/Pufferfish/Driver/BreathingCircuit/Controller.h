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

namespace Pufferfish::BreathingCircuit {

struct Actuators {
  float valve_opening;
};

class Controller {
 public:
  Controller(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      Actuators &actuators)
      : parameters_request_(parameters_request),
        parameters_(parameters),
        sensor_measurements_(sensor_measurements),
        actuators_(actuators) {}

  virtual void update_parameters() = 0;
  void update_clock(uint32_t current_time);
  virtual void update_actuators() = 0;

 protected:
  static const uint32_t update_interval = 2;  // ms
  static constexpr float fio2_min = 21;              // % FiO2
  static constexpr float fio2_max = 100;             // % FiO2

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  const ParametersRequest &parameters_request_;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  Parameters &parameters_;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  SensorMeasurements &sensor_measurements_;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  Actuators &actuators_;

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t current_time_ = 0;  // ms
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t previous_time_ = 0;  // ms
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t initial_time_ = 0;  // ms

  [[nodiscard]] constexpr uint32_t time_step() const { return current_time_ - previous_time_; }
  [[nodiscard]] bool update_needed() const;

};

class HFNCController : public Controller {
 public:
  HFNCController(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      Actuators &actuators)
      : Controller(parameters_request, parameters, sensor_measurements, actuators) {}

  void update_parameters() override;
  void update_actuators() override;

 private:
  static constexpr float p_gain = 0.00001;
  static constexpr float i_gain = 0.0002;
  static constexpr float i_max = 200 * i_gain;
  static constexpr float i_min = 0;

  float error_ = 0;
  float error_integral_ = 0;
};

}  // namespace Pufferfish::BreathingCircuit
