/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 */

#pragma once

#include "Device.h"
#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

using Application::MCUPowerStatus;

/**
 * State management for LTC4015, without I/O.
 * This is basically a Moore machine, so the state consists of the
 * next action to take, along with the current time.
 */
class StateMachine {
 public:
  // More actions will be added later
  enum class Action { initialize, measure };

  [[nodiscard]] Action update();

 private:
  Action next_action_ = Action::initialize;
};

/**
 * High-level driver for LTC4015
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device) : device_(device) {}

  InitializableState setup() override;
  // updates the battery power charging field
  InitializableState output(MCUPowerStatus &mcu_power_status);

 private:
  static const size_t max_retries_setup = 8;    // max retries for setup
  static const size_t max_retries_measure = 8;  // max retries for measuring

  Device device_;
  using Action = StateMachine::Action;
  StateMachine fsm_;
  Action next_action_ = Action::initialize;
  size_t retry_count_ = 0;

  InitializableState initialize();
  InitializableState measure(MCUPowerStatus &mcu_power_status);
};

}  // namespace Pufferfish::Driver::I2C::LTC4015
