/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *  Created on: Jul 14, 2020
 *      Author: March Boonyapaluk
 */

#pragma once

#include <cstdint>

namespace Pufferfish {
namespace HAL {

/**
 * An abstract class for time
 */
class Time {
 public:
  /**
   * Returns the number of millisecond since the startup,
   *  will be rolled over every 50 days
   * @returnthe millisecond since start
   */
  virtual uint32_t millis() = 0;

  /**
   * Block the execution for the given number of millisecond
   *  This function provide delay in the range of [ms, ms + 1)
   * @param ms the number of millisecond to delay
   */
  virtual void delay(uint32_t ms) = 0;

  /**
   * Returns the number of microsecond since the startup,
   * will be rolled over every around 67 seconds with 64 MHz system clock
   * @return the number of microsecond
   */
  virtual uint32_t micros() = 0;

  /**
   * Block the execution for the provided microseconds
   *  The delay provided should be relatively accurate [us, us + 3)
   * @param microseconds the number of microseconds to delay,
   *  must be less than 67 seconds with 64 MHz system clock
   */
  virtual void delay_micros(uint32_t microseconds) = 0;
};

} /* namespace HAL */
} /* namespace Pufferfish */
