/*
 * CRC.h
 *
 *  Created on: May 19, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed CRC calculation.
 */

#pragma once

#include <cstdint>

#include "stm32h7xx_hal.h"

namespace Pufferfish::HAL {

class CRC32C {
 public:
  static const uint32_t polynomial = 0x1edc6f41;
  static const uint32_t init = 0xffffffff;
  static const bool reflect_input = true;
  static const bool reflect_output = true;
  static const uint32_t xor_out = 0xffffffff;

  explicit CRC32C(CRC_HandleTypeDef &hcrc) : hcrc_(hcrc) {}

  uint32_t compute(const uint8_t *data, int size);

 private:
  CRC_HandleTypeDef &hcrc_;
};

}  // namespace Pufferfish::HAL
