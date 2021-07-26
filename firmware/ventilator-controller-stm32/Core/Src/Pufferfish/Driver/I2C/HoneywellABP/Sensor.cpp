/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/I2C/HoneywellABP/Sensor.h"

#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::I2C::HoneywellABP {

// StateMachine

StateMachine::Action StateMachine::update(uint32_t current_time) {
  switch (next_action_) {
    case Action::initialize:
      next_action_ = Action::measure;
      break;
    case Action::measure:
      next_action_ = Action::wait_measurement;
      measuring_timer_.reset(current_time);
      break;
    case Action::wait_measurement:
      if (!measuring_timer_.within_timeout(current_time)) {
        next_action_ = Action::measure;
      }
      break;
  }
  return next_action_;
}

// Sensor

InitializableState Sensor::setup() {
  switch (next_action_) {
    case Action::initialize:
      return initialize(time_.millis());
    case Action::measure:
    case Action::wait_measurement:
      return InitializableState::ok;
  }
  return InitializableState::failed;
}

InitializableState Sensor::output(float &output) {
  switch (next_action_) {
    case Action::measure:
      return measure(time_.millis(), output);
    case Action::wait_measurement:
      next_action_ = fsm_.update(time_.millis());
      return InitializableState::ok;
    default:
      break;
  }
  return InitializableState::failed;
}

InitializableState Sensor::initialize(uint32_t current_time) {
  if (retry_count_ > max_retries_setup) {
    return InitializableState::failed;
  }

  retry_count_ = 0;

  // Wait for power-up
  time_.delay(power_up_delay);

  // measure
  while (device_.read_sample(sample_) != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
    if (sample_.status != ABPStatus::no_error && sample_.status != ABPStatus::stale_data) {
      return InitializableState::failed;
    }
    if (!Util::within(sample_.pressure, p_min, p_max)) {
      return InitializableState::failed;
    }
  }

  next_action_ = fsm_.update(current_time);
  retry_count_ = 0;  // reset retries to 0 for measuring
  return InitializableState::setup;
}

InitializableState Sensor::measure(uint32_t current_time, float &output) {
  if (device_.read_sample(sample_) == I2CDeviceStatus::ok) {
    retry_count_ = 0;  // reset retries to 0 for next measurement
    output = sample_.pressure;
    next_action_ = fsm_.update(current_time);
    if (sample_.status != ABPStatus::no_error && sample_.status != ABPStatus::stale_data) {
      return InitializableState::failed;
    }
    return InitializableState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_measure) {
    return InitializableState::failed;
  }

  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::I2C::HoneywellABP
