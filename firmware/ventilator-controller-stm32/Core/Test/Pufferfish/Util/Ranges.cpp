/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * COBS.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Ranges
 *
 */
#include "Pufferfish/Util/Ranges.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The  Ranges  within function works correctly") {
  GIVEN("The within function") {
    u_int8_t input_val = 131;
    WHEN("floor is '1 and ceiling is 255 , value 131 is within range") {
      u_int8_t min_val = 1;
      u_int8_t max_val = 255;
      THEN("Returns true ,  value 131 is in range") {
        auto range = PF::Util::within(input_val, min_val, max_val);
        REQUIRE(range == true);
      }
    }
  }
}

SCENARIO("The Ranges  clamp and within function works correctly") {
  GIVEN("The clamp function") {
    uint8_t input_val = 131;
    WHEN(
        "the function clamp is called with a floor of 1, a ceiling of 255, and a test value of "
        "131") {
      u_int8_t min_val = 1;
      u_int8_t max_val = 255;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("Returns correct value 131") { REQUIRE(range == 131); }
    }

    WHEN(
        "The function clamp is called with a floor of 1, a ceiling of 10, and a test value of "
        "131 ") {
      u_int8_t min_val = 1;
      u_int8_t max_val = 10;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("Returns expected value 10") { REQUIRE(range == 10); }
    }

    WHEN(
        "The function clamp is called with a floor of 1, a ceiling of 10, and a test value of "
        "131") {
      u_int8_t min_val = 1;
      u_int8_t max_val = 10;
      auto within_range = PF::Util::within(input_val, min_val, max_val);
      THEN("Returns false, value is not in range") { REQUIRE(within_range == false); }
    }
  }
  GIVEN("The clamp function ") {
    uint8_t input_val = 8;
    WHEN(
        "The function clamp is called with a floor of 10, a ceiling of 131, and a test value of "
        "8") {
      uint8_t min_value = 10;
      uint8_t max_value = 131;
      THEN("As expected it returns floor value 0x0a") {
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        REQUIRE(range == 10);
      }
    }

    WHEN(
        "The function within is called with a floor of 10, a ceiling of 131, and a test value of "
        "8") {
      uint8_t min_value = 10;
      uint8_t max_value = 131;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Returns false, value is not in range") { REQUIRE(within_range == false); }
    }

    WHEN(
        "The function clamp is called with a floor of 8, a ceiling of 131, and a test value of 8") {
      uint8_t min_value = 8;
      uint8_t max_value = 131;
      THEN("Returns expected  input value 8") {
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        REQUIRE(range == 8);
      }
    }

    WHEN(
        "The function within is called with a floor of 8, a ceiling of 131, and a test value of "
        "8") {
      uint8_t min_value = 8;
      uint8_t max_value = 131;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Returns true, value is in range") { REQUIRE(within_range == true); }
    }

    GIVEN("The clamp function") {
      uint8_t input_val = 83;
      WHEN(
          "The function clamp is called with a floor of 8, a ceiling of 255, and a test value of "
          "83") {
        uint8_t min_value = 8;
        uint8_t max_value = 255;
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        THEN("Returns expected  input value 83 ") { REQUIRE(range == 83); }
      }

      WHEN(
          "The function clamp is called with a floor of 8, a ceiling of 255, and a test value of "
          "83") {
        uint8_t min_value = 8;
        uint8_t max_value = 255;
        auto within_range = PF::Util::within(input_val, min_value, max_value);
        THEN("Returns true, value is in range") { REQUIRE(within_range == true); }
      }

      WHEN(
          "The function clamp is called with a floor of 8, a ceiling of 8, and a test value of 8") {
        uint8_t min_value = 8;
        uint8_t max_value = 8;
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        THEN("Returns expected value 8") { REQUIRE(range == 8); }
      }
    }

    WHEN("The function within is called with a floor of 8, a ceiling of 8, and a test value of 8") {
      uint8_t min_value = 8;
      uint8_t max_value = 8;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Returns true, value is  in range") { REQUIRE(within_range == true); }
    }

    WHEN(
        "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of 83") {
      uint8_t min_value = 15;
      uint8_t max_value = 8;
      auto range = PF::Util::clamp(input_val, min_value, max_value);
      THEN("Result is as expected 8") { REQUIRE(range == 8); }
    }

    WHEN(
        "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of 83") {
      uint8_t min_value = 15;
      uint8_t max_value = 8;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Returns false, value is not in range") { REQUIRE(within_range == false); }
    }
  }
}
