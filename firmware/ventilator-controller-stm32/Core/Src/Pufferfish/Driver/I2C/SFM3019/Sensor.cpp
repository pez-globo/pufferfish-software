/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"

#include <cmath>

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
  switch (next_action_) {
    case StateMachine::Action::initialize:
      switch (initialize(time_.micros())) {
        case InitializableState::setup:
          next_action_ = fsm_.update(time_.micros());
          return InitializableState::setup;
        case InitializableState::failed:
          return InitializableState::failed;
      }

    case StateMachine::Action::wait_warmup:
      next_action_ = fsm_.update(time_.micros());
      return InitializableState::setup;
    case StateMachine::Action::check_range:
      switch (check_range(time_.micros())) {
        case InitializableState::ok:
          next_action_ = fsm_.update(time_.micros());
          return InitializableState::ok;
        case InitializableState::setup:
          return InitializableState::setup;
        case InitializableState::failed:
          return InitializableState::failed;
      }
    case StateMachine::Action::measure:
    case StateMachine::Action::wait_measurement:
      return InitializableState::ok;
  }
  return InitializableState::failed;
}

InitializableState Sensor::output(float &flow) {
  switch (next_action_) {
    case StateMachine::Action::measure:
      switch (measure(time_.micros(), flow)) {
        case InitializableState::ok:
          next_action_ = fsm_.update(time_.micros());
        case InitializableState::setup:
          return InitializableState::setup;
        case InitializableState::failed:
          return InitializableState::failed;
      }
    case StateMachine::Action::wait_measurement:
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
  // Reset the device
  if (resetter) {
    while (device_.reset() != I2CDeviceStatus::ok) {
      ++retry_count_;
      if (retry_count_ > max_retries_setup) {
        return InitializableState::failed;
      }
    }
  }

  // Wait for power-up
  time_.delay(power_up_delay);
  // Request product_id
  while (device_.request_product_id() != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  // Read product number
  uint32_t pn = 0;
  while (device_.read_product_id(pn) != I2CDeviceStatus::ok || pn != product_number) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  // Request conversion factors
  while (device_.request_conversion_factors() != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  // Read conversion factors
  time_.delay_micros(read_conv_delay_us);
  while (device_.read_conversion_factors(conversion_) != I2CDeviceStatus::ok ||
         conversion_.scale_factor != scale_factor || conversion_.offset != offset ||
         conversion_.flow_unit != flow_unit) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  // Set the averaging window size
  while (device_.set_averaging(averaging_window) != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  // Start continuous measurement
  while (device_.start_measure() != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  retry_count_ = 0;  // reset retries to 0 for measuring
  return InitializableState::setup;
}

InitializableState Sensor::check_range(uint32_t current_time_us) {
  Sample sample{};
  if (device_.read_sample(conversion_, sample) == I2CDeviceStatus::ok && sample.flow >= flow_min &&
      sample.flow <= flow_max) {
    return InitializableState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_setup) {
    return InitializableState::failed;
  }

  return InitializableState::setup;
}

InitializableState Sensor::measure(uint32_t current_time_us, float &flow) {
  Sample sample{};
  if (device_.read_sample(conversion_, sample) == I2CDeviceStatus::ok) {
    retry_count_ = 0;  // reset retries to 0 for next measurement
    flow = sample.flow;
    return InitializableState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_measure) {
    return InitializableState::failed;
  }

  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
