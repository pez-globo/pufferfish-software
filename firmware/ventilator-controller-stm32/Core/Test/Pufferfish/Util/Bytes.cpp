/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Parse.cpp
 *
 *  Created on: 24 May
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Bytes
 *
 */
#include "Pufferfish/Util/Bytes.h"

#include <cstddef>

#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The get_byte function in Bytes correctly gets bytes from the given position") {
  GIVEN("get_byte function") {
    const uint8_t number = 0x86;

    WHEN(
        "the function is called with byte index 0 and input type uint8_t as the template "
        "parameters and with 0x86 as the input value ") {
      int value = PF::Util::get_byte<0>(number);
      THEN("The value at the 0 th  is 134") { REQUIRE(value == 134); }
    }

    WHEN(
        "the function is called with byte index 1 and input type uint8_t as the template "
        "parameters and with 0x86 as the input value") {
      int value = PF::Util::get_byte<1>(number);
      THEN("The value at the 1st index  is 0") { REQUIRE(value == 0); }
    }
  }

  GIVEN("The get_byte function") {
    uint16_t number = 0x860a;
    WHEN("the get_byte function is called for 0th index on 0x860a") {
      int value1 = PF::Util::get_byte<0>(number);
      THEN("The value at 0th index is 10") { REQUIRE(value1 == 10); }
    }

    WHEN("the get_byte function is called for 1st index on 0x860a") {
      int value = PF::Util::get_byte<1>(number);
      THEN("The value at 0th index is 134") { REQUIRE(value == 134); }
    }

    WHEN("the get_byte function is called for 2nd index on 0x860a") {
      int value = PF::Util::get_byte<2>(number);
      THEN("The value at 2nd index is 0") { REQUIRE(value == 0); }
    }
  }
  GIVEN("The get_byte function") {
    uint32_t number = 0x86010f0a;
    WHEN("the get_byte function is called for 0th index on 0x86010f0a") {
      int value = PF::Util::get_byte<0>(number);
      THEN("Result is as expected at 0th index is 10 ") { REQUIRE(value == 10); }
    }

    WHEN("the get_byte function is called for 1st index on 0x86010f0a") {
      int value1 = PF::Util::get_byte<1>(number);
      THEN("Result is as expected at 1st index is 15") { REQUIRE(value1 == 15); }
    }

    WHEN("the get_byte function is called for 2nd index on 0x86010f0a") {
      int value2 = PF::Util::get_byte<2>(number);
      THEN("Result is as expected at 2nd index is 1") { REQUIRE(value2 == 1); }
    }

    WHEN("the get_byte function is called for 3rd index on 0x86010f0a") {
      int value3 = PF::Util::get_byte<3>(number);
      THEN("Result is as expected at 3rd index is 134") { REQUIRE(value3 == 134); }
    }
  }
}

SCENARIO("The  set_byte function correctly sets bytes to the given position") {
  GIVEN("the set_byte function is called on 0x86 for 0th index") {
    const uint8_t number = 0x86;
    WHEN("a byte is set at 0") {
      auto value = PF::Util::set_byte<0, uint8_t>(number);
      THEN("Value is as expected 134 ") { REQUIRE(value == 134); }
    }

    WHEN("A byte is set at 1") {
      auto value = PF::Util::set_byte<1, uint8_t>(number);
      THEN("Value is as expected") { REQUIRE(value == 0); }
    }
  }

  GIVEN("The set_byte function") {
    const uint8_t number = 0x83;
    WHEN("the set_byte function is called on uint16_t data-type  0x83 for 0th index") {
      auto value = PF::Util::set_byte<0, uint16_t>(number);
      THEN("Value is as expected 131") { REQUIRE(value == 131); }
    }

    WHEN("the set_byte function is called on uint16_t data-type 0x83 for 1st index") {
      auto value = PF::Util::set_byte<1, uint16_t>(number);
      THEN("value is as expected 33536") { REQUIRE(value == 33536); }
    }

    WHEN("the set_byte function is called on uint32_t data-type 0x83 for 2nd index") {
      auto value = PF::Util::set_byte<2, uint32_t>(number);
      THEN("Result is as expected 8585216") { REQUIRE(value == 8585216); }
    }

    WHEN("the set_byte function is called on uint32_t data-type 0x83 for 3rd index") {
      auto value = PF::Util::set_byte<3, uint32_t>(number);
      THEN("Result is as expected 2197815296") { REQUIRE(value == 2197815296); }
    }

    // WHEN("the set_byte function is called on uint32_t data-type 0x83 for 4th index") {
    //   auto value = PF::Util::set_byte<4, uint32_t>(number);
    //   THEN("Result is as expected 131 ") { REQUIRE(value == 131); }
    // }
  }

  // GIVEN("The  set_byte function") {
  //   const uint16_t byte = 0x860a;
  //   WHEN("the set_byte function is called on uint16_t data-type 0x860a for 0th index") {
  //     auto value = PF::Util::set_byte<uint16_t>(byte, 0);
  //     THEN("Result is as expected 10") { REQUIRE(value == 10); }
  //   }

  //   WHEN("the set_byte function is called on uint16_t data-type  0x860a for 1st index") {
  //     auto value = PF::Util::set_byte<uint16_t>(byte, 1);
  //     THEN("Result is as expected 2560") { REQUIRE(value == 2560); }
  //   }

  //   WHEN("the set_byte function is called on uint16_t data-type 0x860a for 2nd index") {
  //     auto value = PF::Util::set_byte<uint16_t>(byte, 2);
  //     THEN("Result is as expected 0") { REQUIRE(value == 0); }
  //   }

  //   WHEN("the set_byte function is called on uint32_t data-type 0x860a for 2nd index") {
  //     auto setbyte = PF::Util::set_byte<uint32_t>(byte, 2);
  //     THEN("Result is as expected 655360") { REQUIRE(setbyte == 655360); }
  //   }

  //   WHEN("the set_byte function is called on uint32_t data-type 0x860a for 3rd index") {
  //     auto setbyte = PF::Util::set_byte<uint32_t>(byte, 3);
  //     THEN("Result is as expected") { REQUIRE(setbyte == 167772160); }
  //   }

  //   WHEN("the set_byte function is called on uint32_t data-type 0x860a for 4th index") {
  //     auto setbyte = PF::Util::set_byte<uint32_t>(byte, 4);
  //     THEN("Result is as expected") { REQUIRE(setbyte == 10); }
  //   }
  // }
}
