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

SCENARIO("make_array function works correctly") {
  GIVEN("The make_array function") {
    WHEN("an array is created from 8 uint8_t values as input") {
      auto data =
          PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02);
      THEN("the value at every index in an array is as expected") {
        std::array<uint8_t, 8> expected = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};
        for (int i = 0; i < 8; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
      THEN("After the make_array method is called, The max_size method reports size as 8") { REQUIRE(data.max_size() == 8); }
    }
  

    WHEN("an array is created from 3 strings as input parameter") {
      auto data = PF::Util::Containers::make_array<std::string>("ab", "bc", "cd");
      THEN("the value of an array at every index  is as expected") {
        REQUIRE(data[0] == "ab");
        REQUIRE(data[1] == "bc");
        REQUIRE(data[2] == "cd");
      }
      THEN("After the make_array method is called, The max_size method reports size as 3") { REQUIRE(data.max_size() == 3); }
    }
  

    WHEN("an array is created from const references of values, 0x02 and 0x0f") {
      uint8_t const &ri = 0x02;
      uint8_t const &pi = 0x0f;
      auto data = PF::Util::Containers::make_array<std::reference_wrapper<const uint8_t>>(ri, pi);
      THEN("the array is as expected at every index [0x02,0x0f]") {
        REQUIRE(data[0] == 0x02);
        REQUIRE(data[1] == 0x0f);
      }
    }

    WHEN(
        "an array is created from const references, value of reference variable is changed to 0x0c and 0x0a") {
      uint8_t const &ri = 0x02;
      uint8_t const &pi = 0x0f;
      auto result = PF::Util::Containers::make_array<std::reference_wrapper<const uint8_t>>(ri, pi);
      //since ri and pi are const ref, values cant be changed
      THEN("Const reference variables can't be changed, array remains the same") {
        REQUIRE(result[0] == 0x02);
        REQUIRE(result[1] == 0x0f);
      }
    }
  
  }
  
  GIVEN("Array function ") {
    uint8_t expected = 0x02;
    uint8_t array = 0x01;

    WHEN("array values are modified, reference variables change accordingly"){
      uint8_t &ri = expected;
      uint8_t &pi = array;
      auto result = PF::Util::Containers::make_array<std::reference_wrapper<uint8_t>>(ri, pi);
      ri = 0x09;
      pi  = 0x08;
      THEN("After array modification ,references variables are changed as expected"){
         REQUIRE(result[0] == 0x09);
         REQUIRE(result[1] == 0x08);
       }

    }

    WHEN("Array values are modified, reference variables change accordingly"){
       uint8_t &ri = expected;
       uint8_t &pi = array;
       auto result = PF::Util::Containers::make_array<uint8_t>(ri, pi);
       result[0] = 0x01;
       result[1] = 0x86;
// changing array variable does not change values of referenced variables 
      THEN("After modifying array, values of non-reference variable does not change"){
        REQUIRE(ri == 0x02);
        REQUIRE(pi == 0x01);
      }             
      }

 }
}

 SCENARIO("Empty function") {
  GIVEN("Util function Array") {
    auto data = PF::Util::Containers::make_array<uint8_t>(0x02, 0x03, 0x0a);
    WHEN("function empty is used on array") {
      auto empty = data.empty();
      THEN("Then the empty method of the array returns false") { REQUIRE(empty == false); }
    }
  }
}
