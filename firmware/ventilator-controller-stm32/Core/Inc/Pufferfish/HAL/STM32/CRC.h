/*
 * CRC.h
 *
 *  Created on: May 19, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed CRC calculation.
 */

#ifndef INC_PUFFERFISH_HAL_CRC_H_
#define INC_PUFFERFISH_HAL_CRC_H_

#include <stdint.h>
#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

class CRC32C {
public:
  static const uint32_t polynomial = 0x1edc6f41;
  static const uint32_t init = 0xffffffff;
  static const bool reflectInput = true;
  static const bool reflectOutput = true;
  static const uint32_t xorOut = 0xffffffff;

  CRC32C(CRC_HandleTypeDef &hcrc);

  uint32_t compute(const uint8_t *data, int size);

protected:
  CRC_HandleTypeDef &hcrc;
};

}
}

#endif /* INC_PUFFERFISH_HAL_CRC_H_ */
