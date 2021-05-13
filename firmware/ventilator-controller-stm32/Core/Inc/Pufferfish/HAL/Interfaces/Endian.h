/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  Endianness conversions between host and network order.
 *  STM32 is little-endian, while network order is big-endian, so
 *  the conversion functions reverse the order of bytes.
 *
 *  NOTE: this implementation relies on builtins provided by GCC/Clang.
 *  You will need to compile using a relatively modern version of either
 *  compiler.
 */

#pragma once

#include <cstdint>

namespace Pufferfish::HAL::Interfaces {

constexpr uint64_t hton(uint64_t host_value); 
constexpr uint64_t ntoh(uint64_t network_value); 
constexpr uint32_t hton(uint32_t host_value);
constexpr uint32_t ntoh(uint32_t network_value); 
constexpr uint16_t hton(uint16_t host_value);
constexpr uint16_t ntoh(uint16_t network_value);

}  // namespace Pufferfish::HAL::Interfaces
