/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * STWD100.h
 *
 *      Author: Laxmanrao R
 *
 *  A driver for the STWD100 External watchdog
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Pufferfish/HAL/Interfaces/DigitalOutput.h"
#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish {
namespace Driver {
namespace WatchDog {

/**
 * An abstract class for STWD100 WatchDog module
 */

class STWD100 {
 public:
  /**
   * Constructor for STWD100 external WatchDog driver
   * @param enablePin for STWD100
   * @param inputPin for STWD100
   */
  STWD100(HAL::Interfaces::DigitalOutput &enable_pin, HAL::Interfaces::DigitalOutput &input_pin, HAL::Interfaces::Time &time)
      : enable_pin_(enable_pin), input_pin_(input_pin), time_(time) {}

  /**
   * Enable watchdog timer
   */
  void enable();
  /**
   * Disable watchdog timer
   */
  void disable();
  /**
   * Reset watchdog timer when controller program experienced a freeze or hang
   */
  void toggle_input();

 private:
  static const uint32_t reset_time = 2;  /// TBD: resetTime as to be fixed
  HAL::Interfaces::DigitalOutput &enable_pin_;
  HAL::Interfaces::DigitalOutput &input_pin_;

  HAL::Interfaces::Time &time_;
};

}  // namespace WatchDog
}  // namespace Driver
}  // namespace Pufferfish
