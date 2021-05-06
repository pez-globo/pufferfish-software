/*
 * Random.h
 *
 *  Created on: May 19, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed random number generation
 */

#pragma once

#include "Pufferfish/HAL/Interfaces/Random.h"
#include "stm32h7xx_hal.h"

namespace Pufferfish::HAL::STM32 {

class Random : public Interfaces::Random {
 public:
  explicit Random(RNG_HandleTypeDef &hrng) : hrng_(hrng) {}

  RandomStatus setup();
  RandomStatus generate(uint32_t &result) override;

 private:
  RNG_HandleTypeDef &hrng_;
};

}  // namespace Pufferfish::HAL::STM32
