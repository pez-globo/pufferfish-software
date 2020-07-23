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
   * @param m_port  GPIO port of the MCU (A, B, ...)
   * @param m_pin   GPIO pin of the MCU (1, 2, ...)
   */
  DigitalInput(GPIO_TypeDef &m_port, const uint16_t m_pin) : mPort(m_port), mPin(m_pin) {}

  /**
   * Reads a digital input from the GPIO pin
   * @return true if the pin state is high, false if low
   */
  bool read();
 private:
  GPIO_TypeDef &mPort;
  uint16_t const mPin;
};

} // namespace HAL
} // namespace Pufferfish