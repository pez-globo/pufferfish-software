/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/I2C/LTC4015/Sensor.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

// StateMachine

StateMachine::Action StateMachine::update(uint32_t current_time_us) {
  current_time_us_ = current_time_us;
  switch (next_action_) {
    case Action::initialize:
      next_action_ = Action::measure;
      start_waiting();
      break;
    case Action::measure:
      next_action_ = Action::wait_measurement;
      start_waiting();
      break;
    case Action::wait_measurement:
      if (finished_waiting(measuring_duration_us)) {
        next_action_ = Action::measure;
      }
      break;
  }
  return next_action_;
}

void StateMachine::start_waiting() {
  wait_start_time_us_ = current_time_us_;
}

bool StateMachine::finished_waiting(uint32_t timeout_us) const {
  return !Util::within_timeout(wait_start_time_us_, timeout_us, current_time_us_);
}

// Sensor

InitializableState Sensor::setup() {
  switch (next_action_) {
    case Action::initialize:
      return initialize(time_.micros());
    case Action::measure:
    case Action::wait_measurement:
      return InitializableState::ok;
  }
  return InitializableState::failed;
}

InitializableState Sensor::output(PowerManagement &power_management) {
  switch (next_action_) {
    case Action::measure:
      return measure(time_.micros(), power_management);
    case Action::wait_measurement:
      next_action_ = fsm_.update(time_.micros());
      return InitializableState::ok;
    default:
      break;
  }
  return InitializableState::failed;
}

InitializableState Sensor::initialize(uint32_t current_time_us) {
  if (retry_count_ > max_retries_setup) {
    return InitializableState::failed;
  }

  retry_count_ = 0;
  // Check if device is ready
  bool charging_status = false;
  while (device_.read_charging_status(charging_status) != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  next_action_ = fsm_.update(current_time_us);
  retry_count_ = 0;  // reset retries to 0 for measuring
  return InitializableState::setup;
}

InitializableState Sensor::measure(uint32_t current_time_us, PowerManagement &power_management) {
  // check if charger is connected
  bool charging_status = false;
  if (device_.read_charging_status(charging_status) == I2CDeviceStatus::ok) {
    retry_count_ = 0;  // reset retries to 0 for next measurement
    power_management.charging = charging_status;
    next_action_ = fsm_.update(current_time_us);
    return InitializableState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_measure) {
    return InitializableState::failed;
  }

  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
