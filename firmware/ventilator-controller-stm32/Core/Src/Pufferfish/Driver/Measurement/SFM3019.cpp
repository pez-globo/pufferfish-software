/*
 * SFM3019StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Measurement/SFM3019.h"
#include "Pufferfish/HAL/STM32/Time.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Measurement {

// SFM3019StateMachine

SFM3019StateMachine::State SFM3019StateMachine::state() const {
  return state_;
}

SFM3019StateMachine::Output SFM3019StateMachine::initialize() {
  if (state_ != State::uninitialized) {
    return Output::error_input;
  }

  state_ = State::powering_up;
  wait_start_time_ = current_time_;
  return Output::wait;
}

SFM3019StateMachine::Output SFM3019StateMachine::wait(uint32_t current_time) {
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
      state_ = State::measuring;
      return Output::measure;
    }
    return Output::wait;
  }

  return Output::error_input;
}

SFM3019StateMachine::Output SFM3019StateMachine::reset(uint32_t current_time) {
  state_ = State::powering_up;
  current_time_ = current_time;
  wait_start_time_ = current_time_;
  return Output::wait;
}

SFM3019StateMachine::Output SFM3019StateMachine::check_pn(uint32_t pn) {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
    case State::unrecoverable:
      return Output::error_input;
    default:
      break;
  }

  if (pn != product_number) {
    state_ = State::unrecoverable;
    return Output::error_fail;
  }

  state_ = State::getting_conversion;
  return Output::get_conversion;
}

SFM3019StateMachine::Output SFM3019StateMachine::get_conversion(
    const I2C::SFM3019ConversionFactors &conversion) {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
    case State::unrecoverable:
      return Output::error_input;
    default:
      break;
  }

  state_ = State::configuring_averaging;
  return Output::configure_averaging;
}

SFM3019StateMachine::Output SFM3019StateMachine::configure_averaging() {
  switch (state_) {
    case State::uninitialized:
    case State::powering_up:
    case State::warming_up:
    case State::measuring:
    case State::unrecoverable:
      return Output::error_input;
    default:
      break;
  }

  state_ = State::idle;
  return Output::start_measuring;
}

SFM3019StateMachine::Output SFM3019StateMachine::start_measuring(uint32_t current_time) {
  if (state_ != State::idle) {
    return Output::error_input;
  }

  state_ = State::warming_up;
  current_time_ = current_time;
  wait_start_time_ = current_time_;
  return Output::wait;
}

SFM3019StateMachine::Output SFM3019StateMachine::measure(uint32_t current_time_us) {
  if (state_ != State::measuring) {
    return Output::error_input;
  }

  state_ = State::measuring;
  current_time_us_ = current_time_us;
  wait_start_time_us_ = current_time_us_;
  return Output::wait_us;
}

SFM3019StateMachine::Output SFM3019StateMachine::wait_us(uint32_t current_time_us) {
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

bool SFM3019StateMachine::finished_waiting(uint32_t timeout) const {
  return !Util::within_timeout(wait_start_time_, timeout, current_time_);
}

bool SFM3019StateMachine::finished_waiting_us(uint32_t timeout_us) const {
  return !Util::within_timeout(wait_start_time_us_, timeout_us, current_time_us_);
}

// SFM3019

SFM3019::State SFM3019::update() {
  I2C::SFM3019Sample sample{};

  switch (next_action_) {
    case Action::initialize:
      retry_count_ = 0; // reset retries to 0 for setup
      next_action_ = fsm_.initialize();
      return State::setup;
    case Action::wait:
      next_action_ = fsm_.wait(HAL::millis());
      return State::setup;
    case Action::reset:
      if (low_level_.reset() == I2CDeviceStatus::ok) {
        next_action_ = fsm_.reset(HAL::millis());
        return State::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::check_pn:
      if (low_level_.serial_number(pn_) == I2CDeviceStatus::ok) {
        next_action_ = fsm_.check_pn(pn_);
        return State::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::get_conversion:
      // TODO: implement the conversion retrieval
      next_action_ = fsm_.get_conversion(conversion_);
      return State::setup;
    case Action::configure_averaging:
      // TODO: implement the configuring averaging
      next_action_ = fsm_.configure_averaging();
      return State::setup;
    case Action::start_measuring:
      if (low_level_.start_measure() == I2CDeviceStatus::ok) {
        retry_count_ = 0; // reset retries to 0 for measuring
        next_action_ = fsm_.start_measuring(HAL::millis());
        return State::setup;
      }

      ++retry_count_;
      return check_setup_retry();
    case Action::measure:  // measuring state, so ready to call output()
      if (low_level_.read_sample(
          sample, conversion_.scale_factor, conversion_.offset) == I2CDeviceStatus::ok) {
        retry_count_ = 0; // reset retries to 0 for next measurement
        flow_ = sample.flow;
        next_action_ = fsm_.measure(HAL::micros());
        return State::output;
      }

      ++retry_count_;
      if (retry_count_ > max_retries_output) {
        return State::failed;
      }

      return State::output;
    case Action::wait_us:  // this is also a measuring state
      next_action_ = fsm_.wait_us(HAL::micros());
      return State::output;
    case Action::error_fail:
    case Action::error_input:
      return State::failed;
  }
}

SFM3019::State SFM3019::check_setup_retry() const {
  if (retry_count_ > max_retries_setup) {
    return State::failed;
  }

  return State::setup;
}

}  // namespace Pufferfish::BreathingCircuit
