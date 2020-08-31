/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Time.cpp
 *
 *  Created on: Jul 14, 2020
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/HAL/STM32/Time.h"

#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

uint32_t millis() { return HAL_GetTick(); }

void delay(uint32_t ms) { HAL_Delay(ms); }

// magical sequence to turn on the DWT counter
// see:
// https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
bool micros_delay_init() {
  // The following lines suppress Eclipse CDT's warning about C-style casts and
  // unresolvable fields; these come from the STM32 HAL and are false positives

  // TRCENA off, just in case
  CoreDebug       // @suppress("C-Style cast instead of C++ cast")
      ->DEMCR &=  // @suppress("Field cannot be resolved")
      ~CoreDebug_DEMCR_TRCENA_Msk;
  // TRCENA on
  CoreDebug       // @suppress("C-Style cast instead of C++ cast")
      ->DEMCR |=  // @suppress("Field cannot be resolved")
      CoreDebug_DEMCR_TRCENA_Msk;
  // Unlock debugger
  DWT->LAR =  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
      0xC5ACCE55;
  // reset the cycle counter
  DWT->CYCCNT =  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
      0;
  // enable the counter
  DWT->CTRL |=  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
      1;

  // do some random works to check that the cycle counter actually working
  volatile int i = 0;
  for (i = 2; i < 64; i *= i) {
  }

  // verify the cycles
  return DWT->CYCCNT !=  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
         0;
}

uint32_t micros() {
  const uint32_t cycles_per_us = (HAL_RCC_GetHCLKFreq() / 1000000);

  // The following lines suppress Eclipse CDT's warning about C-style casts and
  // unresolvable fields; these come from the STM32 HAL and are false positives
  return DWT->CYCCNT /  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
         cycles_per_us;
}

void delay_micros(uint32_t microseconds) {
  // The following lines suppress Eclipse CDT's warning about C-style casts and
  // unresolvable fields; these come from the STM32 HAL and are false positives
  const uint32_t start =
      DWT->CYCCNT;  // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  microseconds *= HAL_RCC_GetHCLKFreq() / 1000000;
  while (
      DWT->CYCCNT -  // @suppress("Field cannot be resolved") // @suppress("C-Style cast instead of C++ cast")
          start <
      microseconds) {
  }
}

} /* namespace HAL */
} /* namespace Pufferfish */
