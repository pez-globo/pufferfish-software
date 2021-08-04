/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 */

#pragma once

#include "Device.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish::Driver::I2C::HoneywellABP {

/**
 * State management for HoneyWell ABP, without I/O.
 * This is basically a Moore machine, so the state consists of the
 * next action to take, along with the current time.
 */
class StateMachine {
 public:
  enum class Action { initialize, check_range, measure, wait_measurement };

  [[nodiscard]] Action update(uint32_t current_time);

 private:
  static const uint32_t measuring_duration = 1;  // ms

  Action next_action_ = Action::initialize;
  Util::MsTimer measuring_timer_{measuring_duration, 0};
};

/**
 * High-level driver for HoneyWell ABP
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device, HAL::Interfaces::Time &time) : device_(device), time_(time) {}

  InitializableState setup() override;
  InitializableState output(float &output);

 private:
  using Action = StateMachine::Action;

  static const uint32_t power_up_delay = 3;     // ms
  static const size_t max_retries_setup = 8;    // max retries for setup
  static const size_t max_retries_measure = 8;  // max retries for measuring

  const float p_min = 0.0;  // psi; minimum pressure for abpxxxx001pg2a3
  const float p_max = 1.0;  // psi; maximum pressure for abpxxxx001pg2a3

  size_t retry_count_ = 0;
  Device device_;
  Sample sample_{};
  StateMachine fsm_;
  Action next_action_ = Action::initialize;

  HAL::Interfaces::Time &time_;

  InitializableState initialize(uint32_t current_time);
  InitializableState check_range(uint32_t current_time);
  InitializableState measure(uint32_t current_time, float &output);
};

}  // namespace Pufferfish::Driver::I2C::HoneywellABP
