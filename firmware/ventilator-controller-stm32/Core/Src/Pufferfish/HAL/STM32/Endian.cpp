/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish {
namespace HAL {

uint64_t hton(uint64_t host_value) {
  return __builtin_bswap64(host_value);
}

uint64_t ntoh(uint64_t network_value) {
  return __builtin_bswap64(network_value);
}

uint32_t hton(uint32_t host_value) {
  return __builtin_bswap32(host_value);
}

uint32_t ntoh(uint32_t network_value) {
  return __builtin_bswap32(network_value);
}

uint16_t hton(uint16_t host_value) {
  return __builtin_bswap16(host_value);
}

uint16_t ntoh(uint16_t network_value) {
  return __builtin_bswap16(network_value);
}

}  // namespace HAL
}  // namespace Pufferfish
