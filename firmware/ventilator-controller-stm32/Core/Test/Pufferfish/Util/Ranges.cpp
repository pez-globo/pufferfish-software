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

SCENARIO("The Util::Ranges within function gives the expected result") {
  GIVEN("The Util::Ranges Within function") {
    // value < floor
    WHEN(
        "The function within is called with a floor of 10, a ceiling of 13, and a test value of "
        "8") {
      uint8_t floor = 10;
      uint8_t ceiling = 13;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    // floor == value
    WHEN("The function within is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    // floor < value < ceiling
    WHEN(
        "the function within is called with a floor of 5, a ceiling of 255, and a test value of "
        "8") {
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      uint8_t value = 8;
      THEN("The Within method returns true") {
        bool within_range = PF::Util::within(value, floor, ceiling);
        REQUIRE(within_range);
      }
    }
    // value == ceiling
    WHEN("The function within is called with floor 15, ceiling of 8, and test value of 8") {
      uint8_t floor = 5;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    // value > ceiling
    WHEN("The function within is called with floor 12, ceiling 120, and test value 200") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
  }
}

SCENARIO("The Util::Ranges within function gives the expected result for floor = ceiling ") {
  GIVEN("The Util::Ranges Within function") {
    // floor == ceiling > value
    WHEN("The function within is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }

    // value < floor == ceiling
    WHEN(
        "The function within is called with a floor of 20, ceiling of 20, and a test value of "
        "10 ") {
      uint8_t floor = 20;
      uint8_t ceiling = 20;
      uint8_t value = 10;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }

    // floor == value == ceiling
    WHEN(
        "The function within is called with a floor of 20, a ceiling of 20, and a test value of "
        "20") {
      uint8_t floor = 20;
      uint8_t ceiling = 20;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    // value > floor == ceiling
    WHEN(
        "The function within is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
  }
}

SCENARIO("The Util::Ranges within function gives the expected result for floor > ceiling") {
  GIVEN("The Util::Ranges Within function") {
    // value < ceiling >floor
    WHEN(
        "The function within is called with a floor of 15, a ceiling of 8, and a test value of "
        "4") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 4;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    // ceiling < floor == value
    WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(within_range); }
    }

    // floor > ceiling == value
    WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 8 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }

    // value > floor > ceiling
    WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 30;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
  }
}
SCENARIO("The Util::Ranges clamp function gives the expected result ") {
  GIVEN("The Util::Ranges clamp function ") {
    // value< floor
    WHEN(
        "The function clamp is called with a floor of 10, a ceiling of 10, and a test value of "
        "8") {
      uint8_t floor = 10;
      uint8_t ceiling = 13;
      uint8_t value = 8;
      THEN("The clamp function returns the floor (10)") {
        auto result = PF::Util::clamp(value, floor, ceiling);
        REQUIRE(result == 10);
      }
    }
    //  floor == value
    WHEN("The function clamp is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      uint8_t value = 8;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(result == 8); }
    }
    // floor < value < ceiling
    WHEN(
        "the function within is called with a floor of 5, a ceiling of 255, and a test value of "
        "8") {
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      uint8_t value = 8;
      THEN("The clamp function returns the test value (8)") {
        auto result = PF::Util::clamp(value, floor, ceiling);
        REQUIRE(result == 8);
      }
    }
    // value == ceiling
    WHEN("The function within is called with floor 15, ceiling of 8, and test value of 8") {
      uint8_t floor = 5;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the test value (8)") { REQUIRE(result == 8); }
    }
    // value > ceiling
    WHEN("The function within is called with floor 12, ceiling 120, and test value 200") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the ceiling (120)") { REQUIRE(result == 120); }
    }
  }
}

SCENARIO("The Util::Ranges clamp function gives the expected result for floor == ceiling") {
  GIVEN("The Util::Ranges clamp function") {
    // value < floor == ceiling
    WHEN("The function clamp is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (120)") { REQUIRE(result == 120); }
    }
    // floor == value == ceiling
    WHEN(
        "The function clamp is called with a floor of 20, a ceiling of 20, and a test value of "
        "20") {
      uint8_t floor = 20;
      uint8_t ceiling = 20;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (20)") { REQUIRE(result == 20); }
    }

    // value > floor == ceiling
    WHEN("The function clamp is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (10)") { REQUIRE(result == 10); }
    }

    // value < floor
    WHEN(
        "The function within is called with a floor of 20, ceiling of 50, and a test value of "
        "10 ") {
      uint8_t floor = 20;
      uint8_t ceiling = 50;
      uint8_t value = 10;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns floor(20)") { REQUIRE(result == 20); }
    }
  }
}

SCENARIO("The Util::Ranges clamp function gives the expected result for floor > ceiling") {
  GIVEN("The Util::Ranges clamp function") {
    // floor > ceiling > value
    WHEN(
        "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of "
        "4") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 4;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(result == 8); }
    }

    // ceiling < floor == value
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (15)") { REQUIRE(result == 15); }
    }

    // floor > ceiling == value
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (15)") { REQUIRE(result == 15); }
    }
    //  value > floor > ceiling
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 30;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (15)") { REQUIRE(result == 15); }
    }
  }
}
