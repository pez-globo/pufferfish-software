/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"

#include <cmath>
#include <iostream>

#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

// StateMachine

StateMachine::Action StateMachine::update(uint32_t current_time_us) {
  switch (next_action_) {
    case Action::initialize:
      next_action_ = Action::wait_warmup;
      warmup_timer_.reset(current_time_us);
      break;
    case Action::wait_warmup:
      if (!warmup_timer_.within_timeout(current_time_us)) {
        next_action_ = Action::check_range;
      }
      break;
    case Action::check_range:
    case Action::measure:
      next_action_ = Action::wait_measurement;
      measuring_timer_.reset(current_time_us);
      break;
    case Action::wait_measurement:
      if (!measuring_timer_.within_timeout(current_time_us)) {
        next_action_ = Action::measure;
      }
      break;
  }
  return next_action_;
}

StateMachine::Action StateMachine::output() {
  return next_action_;
}

// Sensor

StateMachine::Action Sensor::get_state() {
  return fsm_.output();
}

InitializableState Sensor::setup() {
  if (prev_state_ != InitializableState::setup) {
    return prev_state_;
  }

  switch (fsm_.output()) {
    case StateMachine::Action::initialize:
      switch (initialize()) {
        case InitializableState::setup:
          fsm_.update(time_.micros());
          prev_state_ = InitializableState::setup;
          return prev_state_;
        case InitializableState::ok:
        case InitializableState::failed:
          prev_state_ = InitializableState::failed;
          return prev_state_;
      }

    case StateMachine::Action::wait_warmup:
      fsm_.update(time_.micros());
      prev_state_ = InitializableState::setup;
      return prev_state_;
    case StateMachine::Action::check_range:
      switch (check_range()) {
        case InitializableState::ok:
          fsm_.update(time_.micros());
          prev_state_ = InitializableState::ok;
          return prev_state_;
        case InitializableState::setup:
          prev_state_ = InitializableState::setup;
          return prev_state_;
        case InitializableState::failed:
          prev_state_ = InitializableState::failed;
          return prev_state_;
      }
    case StateMachine::Action::measure:
    case StateMachine::Action::wait_measurement:
      prev_state_ = InitializableState::ok;
      return prev_state_;
  }
  prev_state_ = InitializableState::failed;
  return prev_state_;
}

InitializableState Sensor::output(float &flow) {
  if (prev_state_ != InitializableState::ok) {
    return prev_state_;
  }

  switch (fsm_.output()) {
    case StateMachine::Action::measure:
      switch (measure(flow)) {
        case InitializableState::ok:
          fsm_.update(time_.micros());
          prev_state_ = InitializableState::ok;
          return prev_state_;
        case InitializableState::setup:
        case InitializableState::failed:
          prev_state_ = InitializableState::failed;
          return prev_state_;
      }
    case StateMachine::Action::wait_measurement:
      fsm_.update(time_.micros());
      prev_state_ = InitializableState::ok;
      return prev_state_;
    default:
      break;
  }
  prev_state_ = InitializableState::failed;
  return prev_state_;
}

InitializableState Sensor::initialize() {
  if (fault_count_ > max_faults_setup) {
    return InitializableState::failed;
  }

  fault_count_ = 0;
  // Reset the device
  if (resetter) {
    while (device_.reset() != I2CDeviceStatus::ok) {
      ++fault_count_;
      if (fault_count_ > max_faults_setup) {
        return InitializableState::failed;
      }
    }
  }

  // Wait for power-up
  time_.delay(power_up_delay);
  // Request product_id
  while (device_.request_product_id() != I2CDeviceStatus::ok) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }

  // Read product number
  uint32_t pn = 0;
  while (device_.read_product_id(pn) != I2CDeviceStatus::ok || pn != product_number) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }

  // Request conversion factors
  while (device_.request_conversion_factors() != I2CDeviceStatus::ok) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }
  // Read conversion factors
  time_.delay_micros(read_conv_delay_us);
  while (device_.read_conversion_factors(conversion_) != I2CDeviceStatus::ok ||
         conversion_.scale_factor != scale_factor || conversion_.offset != offset ||
         conversion_.flow_unit != flow_unit) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }

  // Set the averaging window size
  while (device_.set_averaging(averaging_window) != I2CDeviceStatus::ok) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }

  // Start continuous measurement
  while (device_.start_measure() != I2CDeviceStatus::ok) {
    ++fault_count_;
    if (fault_count_ > max_faults_setup) {
      return InitializableState::failed;
    }
  }

  fault_count_ = 0;  // reset retries to 0 for measuring
  return InitializableState::setup;
}

InitializableState Sensor::check_range() {
  Sample sample{};
  if (device_.read_sample(conversion_, sample) == I2CDeviceStatus::ok && sample.flow >= flow_min &&
      sample.flow <= flow_max) {
    return InitializableState::ok;
  }

  ++fault_count_;
  if (fault_count_ > max_faults_setup) {
    return InitializableState::failed;
  }
  fault_count_ = 0;
  return InitializableState::setup;
}

InitializableState Sensor::measure(float &flow) {
  Sample sample{};
  if (device_.read_sample(conversion_, sample) == I2CDeviceStatus::ok) {
    fault_count_ = 0;  // reset retries to 0 for next measurement
    flow = sample.flow;
    return InitializableState::ok;
  }

  ++fault_count_;
  if (fault_count_ > max_faults_setup) {
    return InitializableState::failed;
  }
  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
