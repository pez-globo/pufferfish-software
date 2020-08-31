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
 * Represents a GPIO output in STM32
 */
class DigitalOutput {
 public:
  /**
   * Constructs a new DigitalOutput
   * @param port  GPIO port of the MCU (A, B, ...)
   * @param pin   GPIO pin of the MCU (1, 2, ...)
   */
  DigitalOutput(GPIO_TypeDef &port, const uint16_t pin)
      : port_(port), pin(pin) {}

  /**
   * Writes a digital output to the GPIO pin
   * @param output  true if desired output is HIGH, false if LOW
   */
  void write(bool output);

 private:
  GPIO_TypeDef &port_;
  uint16_t const pin;
};

}  // namespace HAL
}  // namespace Pufferfish
