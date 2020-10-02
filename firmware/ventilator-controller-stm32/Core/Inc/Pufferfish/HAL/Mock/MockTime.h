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
 * A mock class for time
 */
class MockTime : public Time {
 public:
  /**
   * @brief Constructor for MockTime
   * @param None
   */
  MockTime() {
  }

  /**
   * @brief  Set the millis value into private variable
   * @param  Input the millis value to set into private variable
   * @return None
   */
 void set_millis(uint32_t input);

 /**
  * @brief  Returns the millis value set
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
  * @brief  Set the micros value into private variable
  * @param  Input the micros value to set into private variable
  * @return None
  */
 void set_micros(uint32_t input);

 /**
  * @brief  Returns the micros value set
  * @param  None
  * @return micros value updated in micros_value_
  */
 uint32_t micros() override;

 /**
  * @brief mock delay in micros
  * @param microseconds dealy in micro seconds
  * @return None
  */
 void delay_micros(uint32_t microseconds) override;

 private:
  uint32_t micros_value_;
  uint32_t millis_value_;
};

}  // namespace HAL
}  // namespace Pufferfish
