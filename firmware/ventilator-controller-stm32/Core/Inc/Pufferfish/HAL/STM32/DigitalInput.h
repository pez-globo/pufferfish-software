/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#pragma once

#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

/**
 * Represents a GPIO input in STM32
 */
class DigitalInput {
 public:
  /**
   * Constructs a new Digital Input
   * @param port  GPIO port of the MCU (A, B, ...)
   * @param pin   GPIO pin of the MCU (1, 2, ...)
   */
  DigitalInput(GPIO_TypeDef &port, const uint16_t pin)
      : port_(port), pin(pin) {}

  /**
   * Reads a digital input from the GPIO pin
   * @return true if the pin state is high, false if low
   */
  bool read();

 private:
  GPIO_TypeDef &port_;
  uint16_t const pin;
};

}  // namespace HAL
}  // namespace Pufferfish
