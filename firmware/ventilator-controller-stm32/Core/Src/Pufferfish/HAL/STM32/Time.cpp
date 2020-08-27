/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Time.cpp
 *
 *  Created on: Jul 14, 2020
 *      Author: March Boonyapaluk
 */

#include "stm32h7xx_hal.h"
#include "Pufferfish/HAL/STM32/Time.h"

namespace Pufferfish {
namespace HAL {

uint32_t millis() {
  return HAL_GetTick();
}

void delay(uint32_t ms) {
  HAL_Delay(ms);
}

// magical sequence to turn on the DWT counter
// see: https://stackoverflow.com/questions/36378280/stm32-how-to-enable-dwt-cycle-counter
bool microsDelayInit() {
  // The following lines suppress Eclipse CDT's warning about C-style casts and unresolvable fields;
  // these come from the STM32 HAL and are false positives

  // TRCENA off, just in case
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  // TRCENA on
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  // Unlock debugger
  DWT->LAR = 0xC5ACCE55; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  // reset the cycle counter
  DWT->CYCCNT = 0; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  // enable the counter
  DWT->CTRL |= 1; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")

  // do some random works to check that the cycle counter actually working
  volatile int i;
  for (i = 2; i < 64; i *= i) {
  }

  // verify the cycles
  return DWT->CYCCNT != 0; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
}

uint32_t micros() {
  const uint32_t cyclesPerUs = (HAL_RCC_GetHCLKFreq() / 1000000);

  // The following lines suppress Eclipse CDT's warning about C-style casts and unresolvable fields;
  // these come from the STM32 HAL and are false positives
  return DWT->CYCCNT / cyclesPerUs; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
}

void delayMicros(uint32_t microseconds) {
  // The following lines suppress Eclipse CDT's warning about C-style casts and unresolvable fields;
  // these come from the STM32 HAL and are false positives
  const uint32_t start = DWT->CYCCNT; // @suppress("C-Style cast instead of C++ cast") // @suppress("Field cannot be resolved")
  microseconds *= HAL_RCC_GetHCLKFreq() / 1000000;
  while (DWT->CYCCNT - start < microseconds) { // @suppress("Field cannot be resolved") // @suppress("C-Style cast instead of C++ cast")
  }
}

} /* namespace HAL */
} /* namespace Pufferfish */
