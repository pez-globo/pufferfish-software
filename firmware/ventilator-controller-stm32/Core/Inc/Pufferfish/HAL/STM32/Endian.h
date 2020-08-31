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

#include <cstdint>

namespace Pufferfish {
namespace HAL {

/**
 * Convert endianness of a 32-bit data in host-order to network-order
 * @param hostValue a 32-bit number in host-order
 * @return the given number in network-order
 */
uint32_t hton(uint32_t host_value);

/**
 * Convert endianness of a 32-bit data in host-order to network-order
 * @param networkValue a 32-bit number in network-order
 * @return the given number in host-order
 */
uint32_t ntoh(uint32_t network_value);

/**
 * Convert endianness of a 16-bit data in host-order to network-order
 * @param hostValue a 16-bit number in host-order
 * @return the given number in network-order
 */
uint16_t hton(uint16_t host_value);

/**
 * Convert endianness of a 16-bit data in host-order to network-order
 * @param networkValue a 16-bit number in network-order
 * @return the given number in host-order
 */
uint16_t ntoh(uint16_t network_value);

}  // namespace HAL
}  // namespace Pufferfish
