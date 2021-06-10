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
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::BreathingCircuit {

struct SensorStates {
  bool sfm3019_air;
  bool sfm3019_o2;
  bool fdo2;
  bool nonin_oem;
};

class Simulator {
 public:
  void input_clock(uint32_t current_time);
  virtual void transform(
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorStates &sensor_states,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements) = 0;

 protected:
  static const uint32_t sensor_update_interval = 2;              // ms
  static constexpr float po2_fio2_conversion = 100.0 / 1013250;  // % FiO2 / dPa O2 at 1 atm
  static constexpr float spo2_min = 21;                          // % SpO2
  static constexpr float spo2_max = 100;                         // % SpO2
  static constexpr float hr_min = 0;                             // bpm
  static constexpr float hr_max = 200;                           // bpm

  static constexpr float fio2_responsiveness = 0.01;  // ms
  static constexpr float fio2_noise = 0.005;          // % FiO2

  [[nodiscard]] constexpr uint32_t time_step() const noexcept {
    return step_timer_.duration(current_time_);
  }
  [[nodiscard]] bool update_needed() const;

  [[nodiscard]] uint32_t current_time() const;
  static void transform_fio2(float params_fio2, float &sensor_meas_fio2);

 private:
  uint32_t current_time_ = 0;  // ms
  uint32_t initial_time_ = 0;  // ms
  Util::MsTimer step_timer_{sensor_update_interval, 0};
};

class PCACSimulator : public Simulator {
 public:
  void transform(
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorStates &sensor_states,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements) override;

 private:
  static const uint32_t default_cycle_period = 2000;  // ms
  static const uint32_t default_insp_period = 1000;   // ms
  static const uint32_t minute_duration = 60000;      // ms
  static constexpr float min_per_s = 60;              // s

  Util::MsTimer cycle_timer_{default_cycle_period, 0};
  Util::MsTimer insp_timer_{default_insp_period, 0};
  const float insp_responsiveness = 0.05;       // ms
  const float exp_responsiveness = 0.05;        // ms
  const float insp_init_flow_rate = 120;        // L / min
  const float exp_init_flow_rate = -120;        // L / min
  const float insp_flow_responsiveness = 0.02;  // ms
  const float exp_flow_responsiveness = 0.02;   // ms
  const float rr_noise = 0.5;                   // breaths/min
  const float peep_noise = 1.0;                 // cm H2O
  const float pip_noise = 1.0;                  // cm H2O

  void init_cycle(
      uint32_t cycle_period, const Parameters &parameters, SensorMeasurements &sensor_measurements);
  void transform_cycle_measurements(
      const Parameters &parameters, CycleMeasurements &cycle_measurements);
  void transform_airway_inspiratory(
      const Parameters &parameters, SensorMeasurements &sensor_measurements);
  void transform_airway_expiratory(
      const Parameters &parameters, SensorMeasurements &sensor_measurements);
};

class HFNCSimulator : public Simulator {
 public:
  void transform(
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorStates &sensor_states,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements & /*cycle_measurements*/) override;

 private:
  static const uint32_t default_cycle_period = 2000;  // ms
  static const uint32_t default_insp_period = 1000;   // ms
  static const uint32_t minute_duration = 60000;      // ms

  uint32_t cycle_start_time_ = 0;            // ms
  const float flow_responsiveness = 0.01;    // ms
  const float flow_noise = 0.05;             // L/min
  const float spo2_fio2_scale = 2.5;         // % SpO2 / % FiO2
  const float spo2_responsiveness = 0.0005;  // ms
  const float spo2_noise = 0.1;              // L/min
  const float hr_fio2_scale = 2;             // bpm / % FiO2
  const float hr_responsiveness = 0.0003;    // ms
  const float hr_noise = 0.5;                // L/min

  void init_cycle();
  static void transform_rr(float params_rr, float &cycle_meas_rr);
  void transform_flow(float params_flow, float &sens_meas_flow);
  void transform_spo2(float fio2, float &spo2);
  void transform_hr(float fio2, float &hr);
};

class Simulators {
 public:
  void transform(
      uint32_t current_time,
      const Parameters &parameters,
      const SensorVars &sensor_vars,
      const SensorStates &sensor_states,
      SensorMeasurements &sensor_measurements,
      CycleMeasurements &cycle_measurements);

 private:
  Simulator *active_simulator_ = nullptr;
  PCACSimulator pc_ac_;
  HFNCSimulator hfnc_;

  void input_clock(uint32_t current_time);
};

}  // namespace Pufferfish::Driver::BreathingCircuit
