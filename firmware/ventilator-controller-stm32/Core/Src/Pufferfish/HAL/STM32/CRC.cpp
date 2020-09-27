/*
 * CRC.cpp
 *
 *  Created on: May 19, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/CRC.h"

namespace Pufferfish::HAL {

uint32_t CRC32C::compute(const uint8_t *data, int size) {
  return ~HAL_CRC_Calculate(
      &hcrc_, const_cast<uint32_t *>(reinterpret_cast<const uint32_t *>(data)), size);
}

}  // namespace Pufferfish::HAL
