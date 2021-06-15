/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Parse.cpp
 *
 *  Created on: 17 May 2021
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Endian.h
 *
 */
#include "Pufferfish/Util/Endian.h"

#include <climits>
#include <cstddef>
#include <cstdint>

#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/Bytes.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("read endian data") {
  GIVEN("Endian: function read_bigend") {
    auto data = PF::Util::make_array<uint8_t>(0x01, 0x0f);
    uint16_t re = 0;
    uint16_t &result = re;

    WHEN("read buffer of 2UL  ") {
      PF::Util::read_bigend(data.data(), result);
      THEN("reads correct value") { REQUIRE(result == 0x010f); }
    }
  }

  GIVEN("Endian: function read_bigend") {
    auto data = PF::Util::make_array<uint8_t>(0x00, 0x05, 0x0f, 0x07);
    uint32_t re = 0;
    uint32_t &result = re;

    WHEN("read buffer ") {
      PF::Util::read_bigend(data.data(), result);
      THEN("reads correct value") { REQUIRE(result == 0x50f07); }
    }
  }

  GIVEN("Endian: function read_bigend") {
    auto data = PF::Util::make_array<uint8_t>(0x02, 0x08, 0x00, 0x0d, 0x0e, 0x03, 0x0c, 0x00);
    uint64_t re = 0;
    uint64_t &result = re;
    WHEN("read buffer ") {
      PF::Util::read_bigend(data.data(), result);
      THEN("reads correct value") { REQUIRE(result == 0x208000d0e030c00); }
    }
  }
}

SCENARIO("Write host endian to network endian value") {
  GIVEN("A buffer") {
    uint16_t data = 0x0803;
    WHEN("Host endian is converted in network endian") {
      uint16_t re = 0;
      uint8_t *network_endian = (uint8_t *)&re;
      PF::Util::write_hton(data, network_endian);
      THEN("Result is as  expected") { REQUIRE(re == 0x0308); }
    }
  }

  GIVEN("Util function write_hton") {
    uint32_t data = 0x03020703;
    WHEN("Util function write_hton on uint32_t") {
      uint32_t re = 0;
      uint8_t *network_endian = (uint8_t *)&re;
      PF::Util::write_hton(data, network_endian);
      THEN("Result is as expected") { REQUIRE(re == 0x03070203); }
    }
  }
}
