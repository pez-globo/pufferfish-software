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

SCENARIO("Util:Array make_array function works correctly for uint8, uint16,uint32,int16 and string") {
  GIVEN("Util:Array make_array function") {
    WHEN("An array is created from input type uint8_t as the template parameters and with [0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02]  as the input value") {
      auto data =
          PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02);
      THEN("The array has the expected values [0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02]") {
        std::array<uint8_t, 8> expected = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};
        for (int i = 0; i < 8; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
    }
    WHEN("An array is created from input type uint16_t as the template parameters and with [0xe72b, 0x2b25] as the input value"){
      auto data = PF::Util::Containers::make_array<uint16_t>(0xe72b, 0x2b25 );
      THEN("The array has the expected values [0xe72b, 0x2b25]") {
        std::array<uint16_t, 2> expected = {0xe72b, 0x2b25};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }

    }

    WHEN("An array is created from input type uint32_t as the template parameters and with [0xcf7e59a4, 0xb10ea2ef] input values "){
      auto data =
          PF::Util::Containers::make_array<uint32_t>(0xcf7e59a4, 0xb10ea2ef);
      THEN("The array has the expected values [0xcf7e59a4, 0xb10ea2e]") {
        std::array<uint32_t, 2> expected = {0xcf7e59a4, 0xb10ea2e};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }

    }

    WHEN("An array is created from input type uint32_t as the template parameters and with [1024, 1120] as input values"){
      auto data =
          PF::Util::Containers::make_array<int16_t>(1024, 1120);
      THEN("The array has the expected values[1024, 1120]") {
        std::array<int16_t, 2> expected = {1024, 1120};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }

    }
  
    WHEN("an array is created from 3 strings as the template parameters with [ab, bc, cd]") {
      auto data = PF::Util::Containers::make_array<std::string>("ab", "bc", "cd");
      THEN("the value of an array at every index  is as expected") {
        REQUIRE(data[0] == "ab");
        REQUIRE(data[1] == "bc");
        REQUIRE(data[2] == "cd");
      }
    }
  }
}  
SCENARIO("Util:Array make_array function works properly for const and non-const references"){
  GIVEN("Util:Array make_array function"){

    WHEN("an array is created of const references to two variables with values 0x02 and 0x0f") {
      uint8_t const &ri = 0x02;
      uint8_t const &pi = 0x0f;
      auto data = PF::Util::Containers::make_array<std::reference_wrapper<const uint8_t>>(ri, pi);
      THEN(" the array has the expected values, 0x02 and 0x0f") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x0f);
      }
      THEN("Values of const reference variables can't be changed, The array returns the same values 0x02 and 0x0f") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x0f);
      }
    }


    WHEN("An array is created of non-const references to two variables with values 0x02 and 0x01"){
      uint8_t value0 = 0x02;
      uint8_t value1 = 0x01;      
      uint8_t &ri = value0;
      uint8_t &pi = value1;
      auto result = PF::Util::Containers::make_array<std::reference_wrapper<uint8_t>>(ri, pi);
      ri = 0x09;
      pi  = 0x08;
      THEN("The array has the expected values, 0x02 and 0x01"){
         REQUIRE(result[0] == 0x09);
         REQUIRE(result[1] == 0x08);
        }
      THEN("changing the value of the 0th index of an array to 0x01 and 1st index to 0x86 changes value of referenced variable to 0x01 and 0x86 "){
        result[0].get() = 0x01;
        result[1].get() = 0x86;
        REQUIRE(ri == 0x01);
        REQUIRE(pi == 0x86);
      }             
      }

 }
}

