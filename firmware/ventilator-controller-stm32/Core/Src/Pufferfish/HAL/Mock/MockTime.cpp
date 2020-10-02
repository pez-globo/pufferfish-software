/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * MockTime.cpp
 */

#include "Pufferfish/HAL/Mock/MockTime.h"

namespace Pufferfish{
namespace HAL {


void MockTime::set_millis(uint32_t input){
  millis_value_ = input;
}

uint32_t MockTime::millis() {
  return millis_value_;
}

void MockTime::delay(uint32_t ms) {
  for(volatile uint32_t value = 0; value < ms; value++ ) {
    /* do nothing */
  }
}

void MockTime::set_micros(uint32_t input){
  micros_value_ = input;
}

uint32_t MockTime::micros() {
  return micros_value_;
}

void MockTime::delay_micros(uint32_t microseconds){
  for(volatile uint32_t value = 0; value < microseconds; value++ ) {
    /* do nothing */
  }
}

}  // namespace HAL
}  // namespace Pufferfish
