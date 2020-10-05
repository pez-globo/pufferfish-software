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

class Simulator {
 public:
  Simulator(
      const Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements)
      : parameters_(parameters),
        sensor_measurements_(sensor_measurements),
        cycle_measurements_(cycle_measurements) {}

  void update_clock(uint32_t current_time);
  virtual void update_sensors() = 0;

 protected:
  static const uint32_t sensor_update_interval = 2;  // ms
  static constexpr float spo2_min = 21;              // % SpO2
  static constexpr float spo2_max = 100;             // % SpO2

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  const Parameters &parameters_;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  SensorMeasurements &sensor_measurements_;
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  CycleMeasurements &cycle_measurements_;

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t current_time_ = 0;  // ms
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t previous_time_ = 0;  // ms
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  uint32_t initial_time_ = 0;  // ms

  static constexpr float fio2_responsiveness = 0.01;  // ms

  [[nodiscard]] constexpr uint32_t time_step() const { return current_time_ - previous_time_; }
  [[nodiscard]] bool update_needed() const;

  void update_fio2();
};

class PCACSimulator : public Simulator {
 public:
  PCACSimulator(
      const Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements)
      : Simulator(parameters, sensor_measurements, cycle_measurements) {}

  void update_sensors() override;

 private:
  static const uint32_t default_cycle_period = 2000;  // ms
  static const uint32_t default_insp_period = 1000;   // ms
  static const uint32_t minute_duration = 60000;      // ms
  static constexpr float min_per_s = 60;              // s

  uint32_t cycle_start_time_ = 0;  // ms
  uint32_t insp_period_ = default_insp_period;
  const float insp_responsiveness = 0.05;       // ms
  const float exp_responsiveness = 0.05;        // ms
  const float insp_init_flow_rate = 120;        // L / min
  const float exp_init_flow_rate = -120;        // L / min
  const float insp_flow_responsiveness = 0.02;  // ms
  const float exp_flow_responsiveness = 0.02;   // ms

  void init_cycle(uint32_t cycle_period);
  void update_cycle_measurements();
  void update_airway_inspiratory();
  void update_airway_expiratory();
};

class HFNCSimulator : public Simulator {
 public:
  HFNCSimulator(
      const Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements)
      : Simulator(parameters, sensor_measurements, cycle_measurements) {}

  void update_sensors() override;

 private:
  static const uint32_t default_cycle_period = 2000;  // ms
  static const uint32_t default_insp_period = 1000;   // ms
  static const uint32_t minute_duration = 60000;      // ms

  uint32_t cycle_start_time_ = 0;            // ms
  const float flow_responsiveness = 0.01;    // ms
  const float spo2_fio2_scale = 2.5;         // % SpO2 / % FiO2
  const float spo2_responsiveness = 0.0005;  // ms

  void init_cycle();
  void update_cycle_measurements();
  void update_flow();
  void update_spo2();
};

class Simulators {
 public:
  Simulators(
      const Parameters &parameters,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements)
      : parameters_(parameters),
        pc_ac_(parameters, sensor_measurements, cycle_measurements),
        hfnc_(parameters, sensor_measurements, cycle_measurements) {}

  void update_clock(uint32_t current_time);
  void update_sensors();

 private:
  const Parameters &parameters_;
  Simulator *active_simulator_ = nullptr;
  PCACSimulator pc_ac_;
  HFNCSimulator hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
