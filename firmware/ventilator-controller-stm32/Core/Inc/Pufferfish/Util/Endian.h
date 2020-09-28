/*
 * Endian.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  Utilities for working with arrays.
 */

#pragma once

#include <climits>

#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish::Util {

constexpr void read_ntoh(const uint8_t *network_endian, uint32_t &host_endian) {
  uint32_t network_endian_value = (network_endian[3] << static_cast<uint8_t>(3 * CHAR_BIT)) +
                                  (network_endian[2] << static_cast<uint8_t>(2 * CHAR_BIT)) +
                                  (network_endian[1] << static_cast<uint8_t>(1 * CHAR_BIT)) +
                                  network_endian[0];
  host_endian = HAL::ntoh(network_endian_value);
}

constexpr void read_ntoh(const uint8_t *network_endian, uint16_t &host_endian) {
  uint16_t network_endian_value =
      ((network_endian[1] << static_cast<uint8_t>(CHAR_BIT)) + network_endian[0]);
  host_endian = HAL::ntoh(network_endian_value);
}

constexpr void write_hton(uint32_t host_endian, uint8_t *network_endian) {
  uint32_t network_endian_value = HAL::hton(host_endian);
  network_endian[0] = network_endian_value;
  network_endian[1] = network_endian_value >> static_cast<uint8_t>(1 * CHAR_BIT);
  network_endian[2] = network_endian_value >> static_cast<uint8_t>(2 * CHAR_BIT);
  network_endian[3] = network_endian_value >> static_cast<uint8_t>(3 * CHAR_BIT);
}

constexpr void write_hton(uint16_t host_endian, uint8_t *network_endian) {
  uint16_t network_endian_value = HAL::hton(host_endian);
  network_endian[0] = network_endian_value;
  network_endian[1] = network_endian_value >> static_cast<uint8_t>(CHAR_BIT);
}

}  // namespace Pufferfish::Util
