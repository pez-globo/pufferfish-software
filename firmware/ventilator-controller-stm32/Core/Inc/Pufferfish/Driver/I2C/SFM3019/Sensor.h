/*
 * SFM3019.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  High-level measurement driver for the SFM3019.
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Device.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::I2C::SFM3019 {

/**
 * State management for Sensirion SFM3019 flow sensor, without I/O.
 * This is basically a Moore machine, so the state consists of the
 * next action to take, along with the current time.
 */
class StateMachine {
 public:
  enum class Action { initialize, wait_warmup, check_range, measure, wait_measurement };

  Action update(uint32_t current_time_us);
  Action output();

 private:
  static const uint32_t warming_up_duration_us = 30000;  // us
  static const uint32_t measuring_duration_us = 500;     // us

  Action next_action_ = Action::initialize;
  Util::UsTimer warmup_timer_{warming_up_duration_us, 0};
  Util::UsTimer measuring_timer_{measuring_duration_us, 0};
};

/**
 * High-level (stateful) driver for Sensirion SFM3019 flow sensor
 */
class Sensor : public Initializable {
 public:
  Sensor(Device &device, bool resetter, HAL::Interfaces::Time &time)
      : resetter(resetter), device_(device), time_(time) {}

  InitializableState setup() override;
  InitializableState output(float &flow);

  StateMachine::Action get_state();

 private:
  static const uint32_t power_up_delay = 2;  // ms
  static const uint32_t product_number = 0x04020611;
  static const uint32_t read_conv_delay_us = 20;  // us
  static const int16_t scale_factor = 170;
  static const int16_t offset = -24576;

  static const uint16_t flow_unit =
      make_flow_unit(UnitPrefix::none, TimeBase::per_min, Unit::standard_liter_20deg);
  static const uint32_t averaging_window = 0;
  static constexpr float flow_min = -200;      // L/min
  static constexpr float flow_max = 200;       // L/min
  static const size_t max_faults_setup = 8;    // max retries for all setup steps combined
  static const size_t max_faults_measure = 8;  // max retries between valid outputs

  const bool resetter;

  Device &device_;
  StateMachine fsm_;
  InitializableState prev_state_ = InitializableState::setup;
  size_t fault_count_ = 0;

  ConversionFactors conversion_{};

  HAL::Interfaces::Time &time_;

  InitializableState initialize();
  InitializableState check_range();
  InitializableState measure(float &flow);
};

}  // namespace Pufferfish::Driver::I2C::SFM3019
