/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 */

#pragma once

#include "Device.h"
#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

/**
 * State management for LTC4015, without I/O.
 * This is basically a Moore machine, so the state consists of the
 * next action to take, along with the current time.
 */
class StateMachine {
 public:
  // More actions will be added later
  enum class Action { initialize, measure, wait_measurement };

  [[nodiscard]] Action update(uint32_t current_time_us);

 private:
  static const uint32_t measuring_duration_us = 500;  // us

  Action next_action_ = Action::initialize;
  uint32_t wait_start_time_us_ = 0;
  uint32_t current_time_us_ = 0;

  void start_waiting();
  [[nodiscard]] bool finished_waiting(uint32_t timeout_us) const;
};

/**
 * High-level driver for LTC4015
 */
class Sensor : public Initializable {
 public:
  Sensor(Device &device, HAL::Time &time) : device_(device), time_(time) {}

  InitializableState setup() override;
  // updates the battery power charging field
  InitializableState output(PowerManagement &power_management);

 private:
  static const size_t max_retries_setup = 8;    // max retries for setup
  static const size_t max_retries_measure = 8;  // max retries for measuring

  Device device_;
  using Action = StateMachine::Action;
  StateMachine fsm_;
  Action next_action_ = Action::initialize;
  size_t retry_count_ = 0;

  HAL::Time &time_;

  InitializableState initialize(uint32_t current_time);
  InitializableState measure(uint32_t current_time_us, PowerManagement &power_management);
};

}  // namespace Pufferfish::Driver::I2C::LTC4015
