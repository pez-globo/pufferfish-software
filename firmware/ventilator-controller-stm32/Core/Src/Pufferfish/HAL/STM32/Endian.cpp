/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish {
namespace HAL {

uint32_t reverseEndian(uint32_t value) {
  union Uint32Buffer {
    uint8_t bytes[4];
    uint32_t value;
  };
  Uint32Buffer input, output;
  input.value = value;
  output.bytes[0] = input.bytes[3];
  output.bytes[1] = input.bytes[2];
  output.bytes[2] = input.bytes[1];
  output.bytes[3] = input.bytes[0];
  return output.value;
}

uint32_t hton(uint32_t hostValue) {
  return reverseEndian(hostValue);
}

uint32_t ntoh(uint32_t networkValue) {
  return reverseEndian(networkValue);
}

}  // namespace HAL
}  // namespace Pufferfish
