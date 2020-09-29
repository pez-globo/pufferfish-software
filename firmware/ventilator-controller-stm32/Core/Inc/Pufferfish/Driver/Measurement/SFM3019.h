/*
 * SFM3019.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  High-level measurement driver for the SFM3019.
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Driver/I2C/SFM3019.h"

namespace Pufferfish::Driver::Measurement {

class SFM3019StateMachine {
 public:
  enum class State {
    starting,
    powering_up,
    checking_pn,
    getting_conversion,
    configuring_averaging,
    idle,
    warming_up,
    measuring,
    unrecoverable
  };
  // Next Actions
  enum class Output {
    initialize,
    wait,
    reset,
    check_pn,
    get_conversion,
    configure_averaging,
    start_measuring,
    measure,
    wait_us,
    error_fail,
    error_input
  };

  State state() const;

  // Input Actions
  Output initialize();
  Output wait(uint32_t current_time);
  Output reset(uint32_t current_time);
  Output check_pn(uint32_t pn);
  Output get_conversion(const I2C::SFM3019ConversionFactors &conversion);
  Output configure_averaging();
  Output start_measuring(uint32_t current_time);
  Output measure(uint32_t current_time_us);
  Output wait_us(uint32_t current_time_us);
  Output stop_measuring();


 private:
  static const uint32_t powering_up_duration = 2;  // ms
  static const uint32_t warming_up_duration = 30;  // ms
  static const uint32_t measuring_duration_us = 500; // us
  static const uint32_t product_number = 0x04020611;

  State state_ = State::starting;
  uint32_t wait_start_time_ = 0;
  uint32_t wait_start_time_us_ = 0;
  uint32_t current_time_ = 0;
  uint32_t current_time_us_ = 0;

  bool finished_waiting(uint32_t timeout) const;
  bool finished_waiting_us(uint32_t timeout_us) const;
};

class SFM3019 {
 public:
  enum class Status { ok, waiting, failed };

  SFM3019(I2C::SFM3019 &low_level)
      : low_level_(low_level) {}

  Status setup();
  Status output(SensorMeasurements &measurements);

 private:
  using Action = SFM3019StateMachine::Output;

  static const size_t max_retries = 8;

  I2C::SFM3019 &low_level_;
  SFM3019StateMachine fsm_;
  Action next_action_ = Action::initialize;
  size_t retry_count = 0;

  uint32_t pn_ = 0;
  I2C::SFM3019ConversionFactors conversion_{};
};

}  // namespace Pufferfish::BreathingCircuit
