/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include <cmath>
#include <random>

#include "Pufferfish/Driver/BreathingCircuit/Simulator.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::BreathingCircuit {

std::ranlux24_base prng; // fixed seed for pseudo-random number generation
std::uniform_real_distribution<float> uniform(0, 1);

// Simulator

void Simulator::input_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time;
  }
  if (update_needed()) {
    previous_time_ = current_time_;
  }
  current_time_ = current_time - initial_time_;
}

uint32_t Simulator::current_time() const {
  return current_time_;
}
void Simulator::transform_fio2(float params_fio2, float &sensor_meas_fio2) {
  sensor_meas_fio2 +=
      (params_fio2 - sensor_meas_fio2) * fio2_responsiveness / sensor_update_interval;
  sensor_meas_fio2 += fio2_noise * uniform(prng);
}

bool Simulator::update_needed() const {
  return !Util::within_timeout(previous_time_, sensor_update_interval, current_time_);
}

// PC-AC Simulator

void PCACSimulator::transform(
    const Parameters &parameters,
    const SensorVars & /*sensor_vars*/,
    SensorMeasurements &sensor_measurements,
    CycleMeasurements &cycle_measurements) {
  if (!update_needed()) {
    return;
  }

  if (!parameters.ventilating || parameters.mode != VentilationMode_pc_ac) {
    return;
  }

  // Timing
  sensor_measurements.time = current_time();
  uint32_t cycle_period = minute_duration / parameters.rr;
  if (!Util::within_timeout(cycle_start_time_, cycle_period, current_time())) {
    init_cycle(cycle_period, parameters, sensor_measurements);
    transform_cycle_measurements(parameters, cycle_measurements);
  }
  if (Util::within_timeout(cycle_start_time_, insp_period_, current_time())) {
    transform_airway_inspiratory(parameters, sensor_measurements);
  } else {
    transform_airway_expiratory(parameters, sensor_measurements);
  }
  transform_fio2(parameters.fio2, sensor_measurements.fio2);
}

void PCACSimulator::init_cycle(
    uint32_t cycle_period, const Parameters &parameters, SensorMeasurements &sensor_measurements) {
  cycle_start_time_ = current_time();
  sensor_measurements.flow = insp_init_flow_rate;
  sensor_measurements.volume = 0;
  insp_period_ = cycle_period / (1 + 1.0 / parameters.ie);
  sensor_measurements.cycle += 1;
}

void PCACSimulator::transform_cycle_measurements(
    const Parameters &parameters, CycleMeasurements &cycle_measurements) {
  cycle_measurements.time = current_time();
  cycle_measurements.rr = parameters.rr + rr_noise * (uniform(prng) - 0.5);
  cycle_measurements.peep = parameters.peep + peep_noise * (uniform(prng) - 0.5);
  cycle_measurements.pip = parameters.pip + pip_noise * (uniform(prng) - 0.5);
}

void PCACSimulator::transform_airway_inspiratory(
    const Parameters &parameters, SensorMeasurements &sensor_measurements) {
  sensor_measurements.paw +=
      (parameters.pip - sensor_measurements.paw) * insp_responsiveness / time_step();
  sensor_measurements.flow *= (1 - insp_flow_responsiveness / time_step());
  sensor_measurements.volume +=
      static_cast<float>(sensor_measurements.flow / min_per_s * time_step());
}

void PCACSimulator::transform_airway_expiratory(
    const Parameters &parameters, SensorMeasurements &sensor_measurements) {
  sensor_measurements.paw +=
      (parameters.peep - sensor_measurements.paw) * exp_responsiveness / time_step();
  if (sensor_measurements.flow >= 0) {
    sensor_measurements.flow = exp_init_flow_rate;
  } else {
    sensor_measurements.flow *= (1 - exp_flow_responsiveness / time_step());
  }
  sensor_measurements.volume += sensor_measurements.flow / min_per_s * time_step();
}

// HFNC Simulator

void HFNCSimulator::transform(
    const Parameters &parameters,
    const SensorVars &sensor_vars,
    SensorMeasurements &sensor_measurements,
    // cycle_measurements is part of the Simulator interface
    // NOLINTNEXTLINE(misc-unused-parameters)
    CycleMeasurements & /*cycle_measurements*/) {
  if (!update_needed()) {
    return;
  }

  if (!parameters.ventilating || parameters.mode != VentilationMode_hfnc) {
    return;
  }

  // Timing
  sensor_measurements.time = current_time();
  transform_flow(parameters.flow, sensor_measurements.flow);
  if (sensor_vars.po2 != 0) {
    // simulate FiO2 from pO2 if pO2 is available
    sensor_measurements.fio2 = sensor_vars.po2 * po2_fio2_conversion;
  } else if (std::abs(sensor_vars.flow_air + sensor_vars.flow_o2) >= 1) {
    // simulate FiO2 from relative flow rates if flow rates are available
    float flow_o2_ratio = sensor_vars.flow_o2 / (sensor_vars.flow_air + sensor_vars.flow_o2);
    float inferred_fio2 =
        allowed_fio2.lower * (1 - flow_o2_ratio) + allowed_fio2.upper * flow_o2_ratio;
    transform_fio2(inferred_fio2, sensor_measurements.fio2);
  } else {
    // simulate FiO2 from params
    transform_fio2(parameters.fio2, sensor_measurements.fio2);
  }
  transform_spo2(sensor_measurements.fio2, sensor_measurements.spo2);
  transform_hr(sensor_measurements.fio2, sensor_measurements.hr);
}

void HFNCSimulator::init_cycle() {
  cycle_start_time_ = current_time();
}

void HFNCSimulator::transform_flow(float params_flow, float &sens_meas_flow) {
  sens_meas_flow += (params_flow - sens_meas_flow) * flow_responsiveness / time_step();
  sens_meas_flow += flow_noise * (uniform(prng) - 0.5);
}

void HFNCSimulator::transform_spo2(float fio2, float &spo2) {
  spo2 += (spo2_fio2_scale * fio2 - spo2) * spo2_responsiveness / time_step();
  spo2 += spo2_noise * (uniform(prng) - 0.5);
  // We don't use clamp because we want to preserve NaNs
  if (spo2 < spo2_min) {
    spo2 = spo2_min;
  }
  if (spo2 > spo2_max) {
    spo2 = spo2_max;
  }
}

void HFNCSimulator::transform_hr(float fio2, float &hr) {
  hr += (hr_fio2_scale * fio2 - hr) * hr_responsiveness / time_step();
  hr += hr_noise * (uniform(prng) - 0.5);
  // We don't use clamp because we want to preserve NaNs
  if (hr < hr_min) {
    hr = hr_min;
  }
  if (hr > hr_max) {
    hr = hr_max;
  }
}

// Simulators

void Simulators::transform(
    uint32_t current_time,
    const Parameters &parameters,
    const SensorVars &sensor_vars,
    SensorMeasurements &sensor_measurements,
    CycleMeasurements &cycle_measurements) {
  switch (parameters.mode) {
    case VentilationMode_pc_ac:
      active_simulator_ = &pc_ac_;
      break;
    case VentilationMode_hfnc:
      active_simulator_ = &hfnc_;
      break;
    default:
      active_simulator_ = nullptr;
      return;
  }

  input_clock(current_time);

  active_simulator_->transform(parameters, sensor_vars, sensor_measurements, cycle_measurements);
}

void Simulators::input_clock(uint32_t current_time) {
  if (active_simulator_ == nullptr) {
    return;
  }

  pc_ac_.input_clock(current_time);
  hfnc_.input_clock(current_time);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
