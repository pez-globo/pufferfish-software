/*
 * Random.h
 *
 *  Created on: May 19, 2020
 *      Author: Ethan Li
 *
 *  Random number generation
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Statuses.h"

namespace Pufferfish::HAL::Interfaces {

class Random {
 public:
  /**
   * Generates a random number.
   */
  virtual RandomStatus generate(uint32_t &result) = 0;
};

}  // namespace Pufferfish::HAL::Interfaces
