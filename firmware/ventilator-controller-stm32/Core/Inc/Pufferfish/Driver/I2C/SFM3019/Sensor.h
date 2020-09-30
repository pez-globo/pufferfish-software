/*
 * SFM3019.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  High-level measurement driver for the SFM3019.
 */

#pragma once

#include <cstddef>

#include "Device.h"
#include "Pufferfish/Types.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

/**
 * State management for Sensirion SFM3019 flow sensor, without I/O
 */
class StateMachine {
 public:
  enum class State {
    uninitialized,
    powering_up,
    checking_pn,
    getting_conversion,
    configuring_averaging,
    idle,
    warming_up,
    checking_range,
    measuring
  };
  // Next Actions
  enum class Output {
    initialize,
    wait,
    reset,
    check_pn,
    get_conversion,
    configure_averaging,
    start_measuring,
    check_range,
    measure,
    wait_us,
    error_fail,
    error_input
  };

  [[nodiscard]] State state() const;

  // Input Actions, returns the preferred next action to run
  Output initialize();
  Output wait(uint32_t current_time);
  Output reset(uint32_t current_time);
  Output check_pn(uint32_t pn);
  Output get_conversion(const ConversionFactors &conversion);
  Output configure_averaging();
  Output start_measuring(uint32_t current_time);
  Output check_range(float flow, uint32_t current_time_us);
  Output measure(uint32_t current_time_us);
  Output wait_us(uint32_t current_time_us);
  Output stop_measuring();

 private:
  static const uint32_t powering_up_duration = 2;     // ms
  static const uint32_t warming_up_duration = 30;     // ms
  static const uint32_t measuring_duration_us = 500;  // us
  static const uint32_t product_number = 0x04020611;
  static constexpr float flow_min = -200;  // TODO(lietk12): needs units
  static constexpr float flow_max = 200;   // TODO(lietk12): needs units

  State state_ = State::uninitialized;
  uint32_t wait_start_time_ = 0;
  uint32_t wait_start_time_us_ = 0;
  uint32_t current_time_ = 0;
  uint32_t current_time_us_ = 0;

  [[nodiscard]] bool finished_waiting(uint32_t timeout) const;
  [[nodiscard]] bool finished_waiting_us(uint32_t timeout_us) const;
};

/**
 * High-level (stateful) driver for Sensirion SFM3019 flow sensor
 */
class Sensor {
 public:
  Sensor(Device &device, float &flow) : device_(device), flow_(flow) {}

  SensorState update();

 private:
  using Action = StateMachine::Output;

  static const size_t max_retries_setup = 8;    // max retries for all setup steps combined
  static const size_t max_retries_measure = 8;  // max retries between valid outputs

  Device &device_;
  StateMachine fsm_;
  Action next_action_ = Action::initialize;
  size_t retry_count_ = 0;

  uint32_t pn_ = 0;
  ConversionFactors conversion_{};
  Sample sample_{};

  // Outputs
  float &flow_;

  [[nodiscard]] SensorState check_setup_retry() const;
};

}  // namespace Pufferfish::Driver::I2C::SFM3019
