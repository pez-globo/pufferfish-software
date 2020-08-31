/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/CRC.h"

namespace Pufferfish {
namespace HAL {

uint8_t compute_crc8(const uint8_t *data, int size, const uint8_t polynomial,
                     uint8_t init, bool ref_in, bool ref_out, uint8_t xor_out) {
  uint8_t crc = init;

  uint8_t byte_ctr = 0;
  // calculates 8-Bit checksum with given polynomial
  for (byte_ctr = 0; byte_ctr < size; ++byte_ctr) {
    crc ^= ref_in ? reflect8(data[byte_ctr]) : (data[byte_ctr]);
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ polynomial;
      } else {
        crc = (crc << 1);
      }
    }
  }

  if (ref_out) {
    crc = reflect8(crc);
  }

  return crc ^ xor_out;
}

uint8_t reflect8(uint8_t num) {
  uint8_t res_val = 0;

  for (int i = 0; i < 8; i++) {
    if ((num & (1 << i)) != 0) {
      res_val |= static_cast<uint8_t>(1 << (7 - i));
    }
  }

  return res_val;
}

}  // namespace HAL
}  // namespace Pufferfish
