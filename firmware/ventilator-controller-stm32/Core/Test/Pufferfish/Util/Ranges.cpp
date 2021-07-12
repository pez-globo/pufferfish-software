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

SCENARIO("The Util::Ranges within and clamp function gives the expected result") {
  GIVEN("A floor of 10 , a ceiling of 13 and test value of 8 as the function inputs") {
    // value < floor < ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 10;
      uint8_t ceiling = 13;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 10;
      uint8_t ceiling = 13;
      uint8_t value = 8;
      THEN("The clamp function returns the floor (10)") {
        auto result = PF::Util::clamp(value, floor, ceiling);
        REQUIRE(result == 10);
      }
    }
  }
  GIVEN("A floor of 8 , a ceiling of 10 and test value of 8 as the function inputs") {
    // floor == value < ceiling
    WHEN("The within function is called with the function inputs ") {
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      uint8_t value = 8;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(result == 8); }
    }
  }
  GIVEN("A floor of 5 , a ceiling of 255 and test value of 8 as the function inputs") {
    // floor < value < ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      uint8_t value = 8;
      THEN("The Within method returns true") {
        bool within_range = PF::Util::within(value, floor, ceiling);
        REQUIRE(within_range);
      }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      uint8_t value = 8;
      THEN("The clamp function returns the test value (8)") {
        auto result = PF::Util::clamp(value, floor, ceiling);
        REQUIRE(result == 8);
      }
    }
  }
  GIVEN("A floor of 5 , a ceiling of 8 and test value of 8 as the function inputs") {
    // floor < value == ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 5;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 5;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the test value (8)") { REQUIRE(result == 8); }
    }
  }
  GIVEN("A floor of 12 , a ceiling of 120 and test value of 200 as the function inputs") {
    // value > ceiling > floor
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }

    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the ceiling (120)") { REQUIRE(result == 120); }
    }
  }
}

SCENARIO(
    "The Util::Ranges within and clamp function gives the expected result for floor = ceiling ") {
  GIVEN("A floor of 120 , a ceiling of 120 and test value of 20 as the function inputs") {
    // value < floor == ceiling
    WHEN("The within function is called with the function inputs ") {
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (120)") { REQUIRE(result == 120); }
    }
  }
  GIVEN("A floor of 20 , a ceiling of 20 and test value of 20 as the function inputs") {
    // floor == value == ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 20;
      uint8_t ceiling = 20;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }

    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 20;
      uint8_t ceiling = 20;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (20)") { REQUIRE(result == 20); }
    }
  }
  GIVEN("A floor of 10 , a ceiling of 10 and test value of 20 as the function inputs") {
    // value > floor == ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      uint8_t value = 20;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }

    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      uint8_t value = 20;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (10)") { REQUIRE(result == 10); }
    }
  }
}

SCENARIO(
    "The Util::Ranges within and clamp function gives the expected result for floor > ceiling") {
  GIVEN("A floor of 15 , a ceiling of 8 and test value of 4 as the function inputs") {
    // floor > ceiling > value
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 4;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 4;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the ceiling (8)") { REQUIRE(result == 8); }
    }
  }
  GIVEN("A floor of 15 , a ceiling of 8 and test value of 15 as the function inputs") {
    // ceiling < floor == value
    WHEN("The within function is called with the function inputs ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(within_range); }
    }
    WHEN("The clamp function is called with the function inputs ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (15)") { REQUIRE(result == 15); }
    }
  }
  GIVEN("A floor of 15 , a ceiling of 8 and test value of 8 as the function inputs") {
    // floor > ceiling == value
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the ceiling (15)") { REQUIRE(result == 15); }
    }
  }
  GIVEN("A floor of 15 , a ceiling of 8 and test value of 30 as the function inputs") {
    // value > floor > ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 30;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(!within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 30;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (15)") { REQUIRE(result == 15); }
    }
  }
  GIVEN("A floor of 50 , a ceiling of 20 and test value of 30 as the function inputs") {
    //    floor > value > ceiling
    WHEN("The within function is called with the function inputs") {
      uint8_t floor = 50;
      uint8_t ceiling = 20;
      uint8_t value = 30;
      bool within_range = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns true") { REQUIRE(within_range); }
    }
    WHEN("The clamp function is called with the function inputs") {
      uint8_t floor = 50;
      uint8_t ceiling = 20;
      uint8_t value = 30;
      auto result = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the test value (30)") { REQUIRE(result == 30); }
    }
  }
}
