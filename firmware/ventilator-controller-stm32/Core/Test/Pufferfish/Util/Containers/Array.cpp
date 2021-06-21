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

  GIVEN("The Array function ") {
    uint8_t expected = 0;
    uint8_t array = 0;

    WHEN("an array is created from const references of values, 0x02 and 0x0f") {
      expected = 0x02;
      array = 0x0f;
      uint8_t const &ri = expected;
      uint8_t const &pi = array;
      auto data = PF::Util::Containers::make_array<uint8_t const>(ri, pi);
      THEN("the array is as expected at every index [0x02,0x0f]") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x0f);
        REQUIRE(ri == 0x02);
        REQUIRE(pi == 0x0f);
      }
    }

    WHEN(
        "an array is created from const references, value of reference variable is changed to "
        "-0x0c and 0x0a") {
      expected = 0x0c;
      array = 0x0a;
      uint8_t const &ri = expected;
      uint8_t const &pi = array;
      auto data = PF::Util::Containers::make_array<uint8_t const>(ri, pi);
      THEN("the array is as expected at every index [0x00,0x0c]") {
        REQUIRE(data[0] == 0x0c);
        REQUIRE(data[1] == 0x0a);
      }
      THEN("Returns correct values for reference variables [0x0c, 0x0a]") {
        REQUIRE(ri == 0x0c);
        REQUIRE(pi == 0x0a);
      }
    }
  }

  GIVEN("Array function ") {
    uint8_t expected = 0x00;
    uint8_t array = 0x0c;
    uint8_t &ri = expected;
    uint8_t &pi = array;

    WHEN("Array function make_array is used on non-const references") {
      auto data = PF::Util::Containers::make_array<uint8_t>(ri, pi);
      THEN("returned array is as expected at every index") {
        REQUIRE(data[0] == 0x00);
        REQUIRE(data[1] == 0x0c);
      }
    }

    WHEN(
        "Array function make_array is used on non-const references and values of referenced "
        "variables are changed") {
      uint8_t expected = 0x02;
      uint8_t array = 0x86;
      uint8_t &ri = expected;
      uint8_t &pi = array;
      auto data = PF::Util::Containers::make_array<uint8_t>(ri, pi);
      THEN("returned array is as expected at every index") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x86);
      }
      THEN("Returns correct values for reference variables") {
        REQUIRE(ri == 0x02);
        REQUIRE(pi == 0x86);
      }
    }
  }
}

SCENARIO("Empty an array") {
  GIVEN("Util function Array") {
    auto data = PF::Util::Containers::make_array<uint8_t>(0x02, 0x03, 0x0a);
    WHEN("function empty is used on array") {
      auto empty = data.empty();
      THEN("Result is as expected, array is empty") { REQUIRE(empty == false); }
    }
  }
}
