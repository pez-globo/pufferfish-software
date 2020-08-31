/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish {
namespace HAL {

// NOLINTNEXTLINE(misc-unused-parameters)
uint32_t reverse_endian(uint32_t value) {
  union Uint32Buffer {
    uint8_t bytes[4];
    uint32_t value;
  };
  Uint32Buffer input{};
  Uint32Buffer output{};
  input.value = value;
  output.bytes[0] = input.bytes[3];
  output.bytes[1] = input.bytes[2];
  output.bytes[2] = input.bytes[1];
  output.bytes[3] = input.bytes[0];
  return output.value;
}

// NOLINTNEXTLINE(misc-unused-parameters)
uint16_t reverse_endian(uint16_t value) {
  union Uint16Buffer {
    uint8_t bytes[2];
    uint16_t value;
  };
  Uint16Buffer input{};
  Uint16Buffer output{};
  input.value = value;
  output.bytes[0] = input.bytes[1];
  output.bytes[1] = input.bytes[0];
  return output.value;
}

uint32_t hton(uint32_t host_value) { return reverse_endian(host_value); }

uint32_t ntoh(uint32_t network_value) { return reverse_endian(network_value); }

uint16_t hton(uint16_t host_value) { return reverse_endian(host_value); }

uint16_t ntoh(uint16_t network_value) { return reverse_endian(network_value); }

}  // namespace HAL
}  // namespace Pufferfish
