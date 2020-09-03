/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once


#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace Driver {
namespace Indicators {

/*
 * Class DigitalFunctionGenerator to
 */
class DigitalFunctionGenerator {
 public:
  /**
  * @brief virtual method start
  */
  virtual void start(uint32_t currentTime) = 0;
  /**
  * @brief virtual method update
  */
  virtual void update(uint32_t currentTime) = 0;

  /**
  * @brief virtual method output
  */
  virtual bool output() = 0;
  /**
  * @brief virtual method stop
  */
  virtual void stop() = 0;
};

}  // namespace Indicators
}  // namespace HAL
}  // namespace Pufferfish
