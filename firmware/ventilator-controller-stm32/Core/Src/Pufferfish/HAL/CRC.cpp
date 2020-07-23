/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 * 
 *      Author: Ethan Li
 */

#include "Pufferfish/HAL/CRC.h"

namespace Pufferfish {
namespace HAL {

uint8_t computeCRC8(const uint8_t *data,
                    int size,
                    const uint8_t polynomial,
                    uint8_t init,
                    bool refIn,
                    bool refOut,
                    uint8_t xorOut) {
  uint8_t crc = init;

  uint8_t byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < size; ++byteCtr) {
    crc ^= refIn ? reflect8(data[byteCtr]) : (data[byteCtr]);
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ polynomial;
      } else {
        crc = (crc << 1);
      }
    }
  }

  if (refOut) {
    crc = reflect8(crc);
  }

  return crc ^ xorOut;
}

uint8_t reflect8(uint8_t num) {
  uint8_t resVal = 0;

  for (int i = 0; i < 8; i++) {
    if ((num & (1 << i)) != 0) {
      resVal |= static_cast<uint8_t>(1 << (7 - i));
    }
  }

  return resVal;
}

}  // namespace HAL
}  // namespace Pufferfish
