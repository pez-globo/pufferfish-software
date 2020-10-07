/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 */

#pragma once

#include "Pufferfish/HAL/Interfaces/Time.h"

namespace Pufferfish {
namespace HAL {

/**
 * An HAL class for time
 */
class HALTime : public Time {
 public:
  /**
   * @brief Constructor for HALTime
   * @param None
   */
  HALTime() = default;

  /**
   * @brief  Returns the millis value
   * @param  None
   * @return millis value updated in millis_value_
   */
  uint32_t millis() override;

  /**
   * @brief  mock delay in milli
   * @param  ms dealy in milli seconds
   * @return None
   */
  void delay(uint32_t ms) override;

  /**
   * @brief  micro delay init function
   * @param  None
   * @return bool TRUE/FALSE on init state
   */
  static bool micros_delay_init();

  /**
   * @brief  Returns the micros value set
   * @param  None
   * @return micros value updated in micros_value_
   */
  uint32_t micros() override;

  /**
   * @brief mock delay in micros
   * @param microseconds delay in micro seconds
   * @return None
   */
  void delay_micros(uint32_t microseconds) override;
};

}  // namespace HAL
}  // namespace Pufferfish
