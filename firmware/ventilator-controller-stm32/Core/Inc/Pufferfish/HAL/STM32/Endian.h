/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  Endianness conversions between host and network order.
 *  STM32 is little-endian, while network order is big-endian, so
 *  the conversion functions reverse the order of bytes.
 */

#pragma once

#include <stdint.h>

namespace Pufferfish {
namespace HAL {

/**
 * Convert endianness of a 32-bit data in host-order to network-order
 * @param hostValue a 32-bit number in host-order
 * @return the given number in network-order
 */
uint32_t hton(uint32_t hostValue);

/**
 * Convert endianness of a 32-bit data in host-order to network-order
 * @param networkValue a 32-bit number in network-order
 * @return the given number in host-order
 */
uint32_t ntoh(uint32_t networkValue);

}  // namespace HAL
}  // namespace Pufferfish
