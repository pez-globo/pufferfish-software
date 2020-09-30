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

StateMachine::Output StateMachine::initialize() {
  if (state_ != State::uninitialized) {
    return Output::error_input;
  }

  state_ = State::powering_up;
  wait_start_time_ = current_time_;
  return Output::wait;
}

StateMachine::Output StateMachine::wait(uint32_t current_time) {
  current_time_ = current_time;
  if (state_ == State::powering_up) {
    if (finished_waiting(powering_up_duration)) {
      state_ = State::checking_pn;
      return Output::check_pn;
    }
    return Output::wait;
  }

  if (state_ == State::warming_up) {
    if (finished_waiting(warming_up_duration)) {
      state_ = State::checking_range;
      return Output::check_range;
    }
    return Output::wait;
  }

  return Output::error_input;
}

StateMachine::Output StateMachine::reset(uint32_t current_time) {
  state_ = State::powering_up;
  current_time_ = current_time;
  wait_start_time_ = current_time_;
  return Output::wait;
}

StateMachine::Output StateMachine::check_pn(uint32_t pn) {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
      return Output::error_input;
    default:
      break;
  }

  if (pn != product_number) {
    return Output::error_fail;
  }

  state_ = State::getting_conversion;
  return Output::get_conversion;
}

StateMachine::Output StateMachine::get_conversion(const ConversionFactors& /*conversion*/) {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
      return Output::error_input;
    default:
      break;
  }

  state_ = State::configuring_averaging;
  return Output::configure_averaging;
}

StateMachine::Output StateMachine::configure_averaging() {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
      return Output::error_input;
    default:
      break;
  }

  state_ = State::idle;
  return Output::start_measuring;
}

StateMachine::Output StateMachine::start_measuring(uint32_t current_time) {
  if (state_ != State::idle) {
    return Output::error_input;
  }

  state_ = State::warming_up;
  current_time_ = current_time;
  wait_start_time_ = current_time_;
  return Output::wait;
}

StateMachine::Output StateMachine::check_range(float flow, uint32_t current_time_us) {
  if (state_ != State::checking_range) {
    return Output::error_input;
  }

  if (flow < flow_min || flow > flow_max) {
    return Output::error_fail;
  }

  state_ = State::measuring;
  current_time_us_ = current_time_us;
  wait_start_time_us_ = current_time_us_;
  return Output::wait_us;
}

StateMachine::Output StateMachine::measure(uint32_t current_time_us) {
  if (state_ != State::measuring) {
    return Output::error_input;
  }

  state_ = State::measuring;
  current_time_us_ = current_time_us;
  wait_start_time_us_ = current_time_us_;
  return Output::wait_us;
}

StateMachine::Output StateMachine::wait_us(uint32_t current_time_us) {
  current_time_us_ = current_time_us;

  if (state_ == State::measuring) {
    if (finished_waiting_us(measuring_duration_us)) {
      state_ = State::measuring;
      return Output::measure;
    }
    return Output::wait_us;
  }

  return Output::error_input;
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
      if (device_.reset() == I2CDeviceStatus::ok) {
        retry_count_ = 0;
        next_action_ = fsm_.initialize();
        return SensorState::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::wait:
      next_action_ = fsm_.wait(HAL::millis());
      return SensorState::setup;
    case Action::reset:
      if (device_.reset() == I2CDeviceStatus::ok) {
        next_action_ = fsm_.reset(HAL::millis());
        return SensorState::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::check_pn:
      if (device_.serial_number(pn_) == I2CDeviceStatus::ok) {
        next_action_ =
            fsm_.check_pn(pn_);  // error will be caught on the next update() via next_action_
        return SensorState::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::get_conversion:
      // TODO(lietk12): implement the conversion retrieval
      next_action_ = fsm_.get_conversion(conversion_);
      return SensorState::setup;
    case Action::configure_averaging:
      // TODO(lietk12): implement the configuring averaging
      next_action_ = fsm_.configure_averaging();
      return SensorState::setup;
    case Action::start_measuring:
      if (device_.start_measure() == I2CDeviceStatus::ok) {
        retry_count_ = 0;  // reset retries to 0 for measuring
        next_action_ = fsm_.start_measuring(HAL::millis());
        return SensorState::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::check_range:
      if (device_.read_sample(sample_, conversion_.scale_factor, conversion_.offset) ==
          I2CDeviceStatus::ok) {
        next_action_ = fsm_.check_range(
            sample_.flow,
            HAL::micros());  // error will be caught on the next update() via next_action_
        return SensorState::ok;
      }

      ++retry_count_;
      if (retry_count_ > max_retries_setup) {
        return SensorState::failed;
      }

      return SensorState::ok;
    case Action::measure:
      if (device_.read_sample(sample_, conversion_.scale_factor, conversion_.offset) ==
          I2CDeviceStatus::ok) {
        retry_count_ = 0;  // reset retries to 0 for next measurement
        flow_ = sample_.flow;
        next_action_ = fsm_.measure(HAL::micros());
        return SensorState::ok;
      }

      ++retry_count_;
      if (retry_count_ > max_retries_measure) {
        return SensorState::failed;
      }

      return SensorState::ok;
    case Action::wait_us:
      next_action_ = fsm_.wait_us(HAL::micros());
      return SensorState::ok;
    case Action::error_fail:
    case Action::error_input:
      return SensorState::failed;
  }
}

SensorState Sensor::check_setup_retry() const {
  if (retry_count_ > max_retries_setup) {
    return SensorState::failed;
  }

  return SensorState::setup;
}

}  // namespace Pufferfish::Driver::I2C::SFM3019
