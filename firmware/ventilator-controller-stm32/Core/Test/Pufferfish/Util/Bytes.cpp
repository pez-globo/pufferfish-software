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
      THEN("The value at the 0 th  is 0x86") { REQUIRE(value == 0x86); }
    }

    WHEN(
        "the function is called with byte index 1 and input type uint8_t as the template "
        "parameters and with 0x86 as the input value") {
      int value = PF::Util::get_byte<1>(number);
      THEN("The value at the 1st index  is 0x00") { REQUIRE(value == 0x00); }
    }
  }

  GIVEN("The get_byte function") {
    uint16_t number = 0x860a;
    WHEN(
        "the function is called with byte index 0 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      int value1 = PF::Util::get_byte<0>(number);
      THEN("The value at 0th index is 0x0a") { REQUIRE(value1 == 0x0a); }
    }

    WHEN(
        "the function is called with byte index 1 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      int value = PF::Util::get_byte<1>(number);
      THEN("The value at 1st index is 0x86") { REQUIRE(value == 0x86); }
    }

    WHEN(
        "the function is called with byte index 2 and input type uint16_t as the template "
        "parameters and with 0x860a as the input value") {
      int value = PF::Util::get_byte<2>(number);
      THEN("The value at 2nd index is 0x00") { REQUIRE(value == 0x00); }
    }
  }
  GIVEN("The get_byte function") {
    uint32_t number = 0x86010f0a;
    WHEN(
        "the function is called with byte index 0 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      int value = PF::Util::get_byte<0>(number);
      THEN("Result is as expected at 0th index is 0x0a ") { REQUIRE(value == 0x0a); }
    }

    WHEN(
        "the function is called with byte index 1 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      int value1 = PF::Util::get_byte<1>(number);
      THEN("Result is as expected at 1st index is 0x0f") { REQUIRE(value1 == 0x0f); }
    }

    WHEN(
        "the function is called with byte index 2 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      int value2 = PF::Util::get_byte<2>(number);
      THEN("Result is as expected at 2nd index is 0x01") { REQUIRE(value2 == 0x01); }
    }

    WHEN(
        "the function is called with byte index 3 and input type uint32_t as the template "
        "parameters and with 0x86010f0a as the input value") {
      int value3 = PF::Util::get_byte<3>(number);
      THEN("Result is as expected at 3rd index is 0x86") { REQUIRE(value3 == 0x86); }
    }
  }
}

SCENARIO("The  set_byte function correctly sets bytes to the given position") {
  GIVEN("the set_byte function is called on 0x86 for 0th index") {
    const uint8_t number = 0x86;
    WHEN(
        "the function is called with byte index 0 and input type uint8_t as the template "
        "parameters and with 0x83 as the input value ") {
      auto value = PF::Util::set_byte<0, uint8_t>(number);
      THEN("Value is as expected 0x86 ") { REQUIRE(value == 0x86); }
    }

    WHEN(
        "the function is called with byte index 1 and input type uint8_t as the template "
        "parameters and with 0x83 as the input value") {
      auto value = PF::Util::set_byte<1, uint8_t>(number);
      THEN("Value is as expected 0x00") { REQUIRE(value == 0x00); }
    }
  }

  GIVEN("The set_byte function") {
    const uint8_t number = 0x83;
    WHEN(
        "the function is called with byte index 0 and on type uint16_t as the template parameters "
        "and with 0x83 as the input value") {
      auto value = PF::Util::set_byte<0, uint16_t>(number);
      THEN("Value is as expected 0x83") { REQUIRE(value == 0x83); }
    }

    WHEN(
        "the function is called with byte index 1 and on type uint16_t as the template parameters "
        "and with 0x83 as the input value") {
      auto value = PF::Util::set_byte<1, uint16_t>(number);
      THEN("value is as expected 33536") { REQUIRE(value == 0x8300); }
    }

    WHEN(
        "the function is called with byte index 2 and on type uint32_t as the template parameters "
        "and with 0x83 as the input value") {
      auto value = PF::Util::set_byte<2, uint32_t>(number);
      THEN("Result is as expected 8585216") { REQUIRE(value == 0x830000); }
    }

    WHEN(
        "the function is called with byte index 3 and on type uint32_t as the template parameters "
        "and with 0x83 as the input value") {
      auto value = PF::Util::set_byte<3, uint32_t>(number);
      THEN("Result is as expected 2197815296") { REQUIRE(value == 0x83000000); }
    }
  }
}
