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

  inline uint32_t compute(const uint8_t *data, int size) {
    return ~HAL_CRC_Calculate(
        &hcrc_,
        // We need to perform a const cast because the STM32 HAL function is not const-correct,
        // even though the function implementation does not modify the input data.
        // We need to perform a reinterpret cast because the STM32 HAL function asks for it.
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast, cppcoreguidelines-pro-type-reinterpret-cast)
        const_cast<uint32_t *>(reinterpret_cast<const uint32_t *>(data)),
        size);
  }

 private:
  CRC_HandleTypeDef &hcrc_;
};

}  // namespace Pufferfish::HAL
