/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"

#include "Pufferfish/HAL/STM32/Time.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

// StateMachine

StateMachine::State StateMachine::state() const {
  return state_;
}

StateMachine::Output StateMachine::initialize(uint32_t current_time) {
  // This transition is available from every state, as its first step is to reset the sensor
  state_ = State::waiting_warmup;
  current_time_ = current_time;
  wait_start_time_ = current_time_;
  return Output::wait_warmup;
}

StateMachine::Output StateMachine::wait_warmup(uint32_t current_time) {
  if (state_ != State::waiting_warmup) {
    return Output::error_input;
  }

  current_time_ = current_time;
  if (!finished_waiting(warming_up_duration)) {
    return Output::wait_warmup;
  }

  state_ = State::checking_range;
  return Output::check_range;
}

StateMachine::Output StateMachine::check_range(uint32_t current_time_us) {
  if (state_ != State::checking_range) {
    return Output::error_input;
  }

  state_ = State::waiting_measurement;
  current_time_us_ = current_time_us;
  wait_start_time_us_ = current_time_us_;
  return Output::wait_measurement;
}

StateMachine::Output StateMachine::measure(uint32_t current_time_us) {
  if (state_ != State::measuring) {
    return Output::error_input;
  }

  state_ = State::waiting_measurement;
  current_time_us_ = current_time_us;
  wait_start_time_us_ = current_time_us_;
  return Output::wait_measurement;
}

StateMachine::Output StateMachine::wait_measurement(uint32_t current_time_us) {
  if (state_ != State::waiting_measurement) {
    return Output::error_input;
  }

  current_time_us_ = current_time_us;
  if (!finished_waiting_us(measuring_duration_us)) {
    return Output::wait_measurement;
  }

  state_ = State::measuring;
  return Output::measure;
}

bool StateMachine::finished_waiting(uint32_t timeout) const {
  return !Util::within_timeout(wait_start_time_, timeout, current_time_);
}

bool StateMachine::finished_waiting_us(uint32_t timeout_us) const {
  return !Util::within_timeout(wait_start_time_us_, timeout_us, current_time_us_);
}

// Sensor

SensorState Sensor::update() {
  switch (next_action_) {
    case Action::initialize:
      return initialize(HAL::millis());
    case Action::wait_warmup:
      next_action_ = fsm_.wait_warmup(HAL::millis());
      return SensorState::setup;
    case Action::check_range:
      return check_range(HAL::micros());
    case Action::measure:
      return measure(HAL::micros());
    case Action::wait_measurement:
      next_action_ = fsm_.wait_measurement(HAL::micros());
      return SensorState::ok;
    default:
      break;
  }
  return SensorState::failed;
}

SensorState Sensor::initialize(uint32_t current_time) {
  if (retry_count_ > max_retries_setup) {
    return SensorState::failed;
  }

  retry_count_ = 0;
  // Reset the device
  while (device_.reset() != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return SensorState::failed;
    }
  }

  // Wait for power-up
  HAL::delay(2);

  // Read product number
  while (device_.serial_number(pn_) != I2CDeviceStatus::ok || pn_ != product_number) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return SensorState::failed;
    }
  }

  // TODO(lietk12): implement the conversion retrieval

  // TODO(lietk12): implement the configuring averaging

  while (device_.start_measure() != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return SensorState::failed;
    }
  }

  next_action_ = fsm_.initialize(current_time);
  retry_count_ = 0;  // reset retries to 0 for measuring
  return SensorState::setup;
}

SensorState Sensor::check_range(uint32_t current_time_us) {
  if (device_.read_sample(sample_, conversion_.scale_factor, conversion_.offset) ==
          I2CDeviceStatus::ok &&
      sample_.flow >= flow_min && sample_.flow <= flow_max) {
    next_action_ = fsm_.check_range(current_time_us);
    return SensorState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_setup) {
    return SensorState::failed;
  }

  return SensorState::ok;
}

SensorState Sensor::measure(uint32_t current_time_us) {
  if (device_.read_sample(sample_, conversion_.scale_factor, conversion_.offset) ==
      I2CDeviceStatus::ok) {
    retry_count_ = 0;  // reset retries to 0 for next measurement
    flow_ = sample_.flow;
    next_action_ = fsm_.measure(current_time_us);
    return SensorState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_measure) {
    return SensorState::failed;
  }

  return SensorState::ok;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
