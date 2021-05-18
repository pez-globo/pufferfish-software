/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/Random.h"

namespace Pufferfish::HAL::STM32 {

RandomStatus Random::setup() {
  uint32_t result = 0;
  return generate(result);
}

RandomStatus Random::generate(uint32_t &result) {
  HAL_StatusTypeDef status = HAL_RNG_GenerateRandomNumber(&hrng_, &result);
  if (status != 0) {
    return RandomStatus::ok;
  }

  return RandomStatus::error;
}

}  // namespace Pufferfish::HAL::STM32
