/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/HAL/STM32/DigitalInput.h"

namespace Pufferfish {
namespace HAL {

bool DigitalInput::read() {
  return HAL_GPIO_ReadPin(&port_, pin) == GPIO_PIN_SET;
}

}  // namespace HAL
}  // namespace Pufferfish
