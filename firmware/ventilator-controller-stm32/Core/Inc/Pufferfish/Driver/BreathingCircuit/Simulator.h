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

class Simulator {
 public:
  Simulator(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements);

  void update_clock(uint32_t current_time);
  void update_sensors();
  void update_actuators();

 private:
  static constexpr float default_cycle_period = 2000;  // ms
  static constexpr float default_insp_period = 1000;   // ms

  const ParametersRequest &parameters_request_;
  Parameters &parameters_;
  SensorMeasurements &sensor_measurements_;
  CycleMeasurements &cycle_measurements_;

  uint32_t current_time_ = 0;                 // ms
  uint32_t previous_time_ = 0;                // ms
  uint32_t cycle_start_time_ = 0;             // ms
  const uint32_t sensor_update_interval = 2;  // ms
  float cycle_period_ = default_cycle_period;
  float insp_period_ = default_insp_period;
  const float insp_responsiveness = 0.05;  // ms
  const float exp_responsiveness = 0.05;   // ms
  const float insp_init_flow_rate = 120;   // L / min
  const float exp_init_flow_rate = -120;   // L / min
  const float insp_flow_responsiveness = 0.02;
  const float exp_flow_responsiveness = 0.02;
  const float fio2_responsiveness = 0.01;  // ms
};

}  // namespace Pufferfish::BreathingCircuit
