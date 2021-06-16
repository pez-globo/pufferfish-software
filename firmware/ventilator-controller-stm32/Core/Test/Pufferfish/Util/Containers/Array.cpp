/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Parse.cpp
 *
 *  Created on: 17 May 2021
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Array
 *
 */
#include "Pufferfish/Util/Containers/Array.h"

#include <iostream>

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The Util array function creates array correctly") {
  GIVEN("The Util make_array function") {
    WHEN("an array is created from 8 uint8_t values as input") {
      auto data =
          PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02);
      THEN("the value at every index in an array is as expected") {
        std::array<uint8_t, 8> expected = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};
        for (int i = 0; i < 8; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
      THEN("Size of an array is as expected") { REQUIRE(data.max_size() == 8); }
    }
  }

  GIVEN("The Util Array function") {
    auto data = PF::Util::Containers::make_array<std::string>("ab", "bc", "cd");
    WHEN("an array is created from 3 strings as input parameter") {
      THEN("the value of an array at every index  is as expected") {
        REQUIRE(data[0] == "ab");
        REQUIRE(data[1] == "bc");
        REQUIRE(data[2] == "cd");
      }
      THEN("Size of an array is as expected") { REQUIRE(data.max_size() == 3); }
    }
  }

  GIVEN("The Util Array function ") {
    uint8_t expected = 0x02;
    uint8_t array = 0x0f;

    WHEN("an array is created from const references of values, 0x02 and 0x0f") {
      uint8_t &ri = expected;
      uint8_t &pi = array;
      auto data = PF::Util::Containers::make_array<uint8_t>(ri, pi);
      THEN("the array is as expected at every index [0x02,0x0f]") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x0f);
      }
    }

    WHEN("an array is created from const references of values ,0x00 and 0x0c") {
      uint8_t expected = 0x00;
      uint8_t array = 0x0c;
      uint8_t &ri = expected;
      uint8_t &pi = array;
      auto data = PF::Util::Containers::make_array<uint8_t>(ri, pi);
      THEN("the array is as expected at every index [0x00,0x0c]") {
        REQUIRE(data[0] == 0x00);
        REQUIRE(data[1] == 0x0c);
      }
    }

    WHEN("Array function make_array is used on non-const references") {
      uint8_t expected = 0x00;
      uint8_t array = 0x0c;
      auto data = PF::Util::Containers::make_array<uint8_t>(expected, array);
      THEN("returned array is as expected at every index") {
        REQUIRE(data[0] == 0x00);
        REQUIRE(data[1] == 0x0c);
      }
    }

    WHEN("Array function make_array is used on non-const references") {
      uint8_t expcted = 0x02;
      uint8_t array = 0x86;
      auto data = PF::Util::Containers::make_array<uint8_t>(expected, array);
      THEN("returned array is as expected at every index") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x86);
      }
    }
  }
}

SCENARIO("Empty an array") {
  GIVEN("Util function Array") {
    auto data = PF::Util::Containers::make_array<uint8_t>(0x02, 0x03, 0x0a);
    WHEN("function empty is used on array") {
      auto empty = data.empty();
      THEN("Result is as expected, array is empty") { REQUIRE(empty == 0); }
    }
  }
}
