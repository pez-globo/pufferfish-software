/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#pragma once

#include "Pufferfish/HAL/HAL.h"
#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

/**
 * Represents a GPIO output in STM32
 */
class HALDigitalOutput : public DigitalOutput {
 public:
  /**
   * Constructs a new DigitalOutput
   * @param port  GPIO port of the MCU (A, B, ...)
   * @param pin   GPIO pin of the MCU (1, 2, ...)
   * @param inverted   true if the output should be inverted
   */
  HALDigitalOutput(GPIO_TypeDef &port, const uint16_t pin, const bool inverted =
  false)
      :
      mPort(port),
      mPin(pin),
      mInverted(inverted) {
  }

  void write(bool output) override;
 private:
  GPIO_TypeDef &mPort;
  const uint16_t mPin;
  const bool mInverted;
};

} // namespace HAL
} // namespace Pufferfish
