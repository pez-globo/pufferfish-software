/*
 * Endian.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 *
 *  Utilities for working with arrays.
 */

#pragma once

#include "Bytes.h"
#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish::Util {

constexpr void read_ntoh(const uint8_t *network_endian, int16_t &host_endian) noexcept {
  uint16_t network_endian_value =
      (set_byte<1, uint16_t>(network_endian[1]) + set_byte<0, uint16_t>(network_endian[0]));
  host_endian = HAL::STM32::ntoh(network_endian_value);
}

constexpr void read_ntoh(const uint8_t *network_endian, uint64_t &host_endian) noexcept {
  uint64_t network_endian_value =
      // NOLINTNEXTLINE(readability-magic-numbers)
      (set_byte<7, uint64_t>(network_endian[7]) + set_byte<6, uint64_t>(network_endian[6]) +
       // NOLINTNEXTLINE(readability-magic-numbers)
       set_byte<5, uint64_t>(network_endian[5]) + set_byte<4, uint64_t>(network_endian[4]) +
       set_byte<3, uint64_t>(network_endian[3]) + set_byte<2, uint64_t>(network_endian[2]) +
       set_byte<1, uint64_t>(network_endian[1]) + set_byte<0, uint64_t>(network_endian[0]));
  host_endian = HAL::STM32::ntoh(network_endian_value);
}

constexpr void read_ntoh(const uint8_t *network_endian, uint32_t &host_endian) noexcept {
  uint32_t network_endian_value =
      (set_byte<3, uint32_t>(network_endian[3]) + set_byte<2, uint32_t>(network_endian[2]) +
       set_byte<1, uint32_t>(network_endian[1]) + set_byte<0, uint32_t>(network_endian[0]));
  host_endian = HAL::STM32::ntoh(network_endian_value);
}

constexpr void read_ntoh(const uint8_t *network_endian, uint16_t &host_endian) {
  uint16_t network_endian_value =
      (set_byte<1, uint16_t>(network_endian[1]) + set_byte<0, uint16_t>(network_endian[0]));
  host_endian = HAL::STM32::ntoh(network_endian_value);
}

constexpr void write_hton(uint32_t host_endian, uint8_t *network_endian) noexcept {
  uint32_t network_endian_value = HAL::STM32::hton(host_endian);
  network_endian[0] = get_byte<0>(network_endian_value);
  network_endian[1] = get_byte<1>(network_endian_value);
  network_endian[2] = get_byte<2>(network_endian_value);
  network_endian[3] = get_byte<3>(network_endian_value);
}

constexpr void write_hton(uint16_t host_endian, uint8_t *network_endian) noexcept {
  uint16_t network_endian_value = HAL::STM32::hton(host_endian);
  network_endian[0] = get_byte<0>(network_endian_value);
  network_endian[1] = get_byte<1>(network_endian_value);
}

}  // namespace Pufferfish::Util
