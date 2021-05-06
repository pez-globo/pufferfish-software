/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/Random.h"

namespace Pufferfish::HAL::STM32 {

RandomStatus Random::setup() {
  uint32_t result;
  generate(result);
}

RandomStatus Random::generate(uint32_t &result) {
  HAL_StatusTypeDef status = HAL_RNG_GenerateRandomNumber(&hrng_, &result);
  switch (status) {
    case HAL_RNG_ERROR_NONE:
      return RandomStatus::ok;
    case HAL_RNG_ERROR_TIMEOUT:
      return RandomStatus::timeout_error;
    case HAL_RNG_ERROR_BUSY:
      return RandomStatus::busy_error;
    case HAL_RNG_ERROR_SEED:
      return RandomStatus::seed_error;
    case HAL_RNG_ERROR_CLOCK:
      return RandomStatus::clock_error;
    default:
      return RandomStatus::error;
  }
}

}  // namespace Pufferfish::HAL::STM32
