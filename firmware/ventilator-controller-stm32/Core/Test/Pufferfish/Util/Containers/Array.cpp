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

SCENARIO(
    "Util:Array make_array function works correctly for uint8, uint16,uint32, int16 and structure "
    "types") {
  GIVEN("Util:Array make_array function") {
    WHEN(
        "An array is created from input type uint8_t as the template parameters and with 8 uint8 "
        "as the input values") {
      auto data =
          PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02);
      THEN("The array has the expected values at every index") {
        std::array<uint8_t, 8> expected = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};
        for (int i = 0; i < 8; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
    }

    WHEN(
        "An array is created from input type uint8_t as the template parameters and with 2 uint16 "
        "as the input values") {
      auto data = PF::Util::Containers::make_array<uint8_t>(0xf7de, 0x5ab6);
      THEN("The array has the expected values of  uint8_t at every index") {
        REQUIRE(data[0] == 0xde);
        REQUIRE(data[1] == 0xb6);
      }
    }
    WHEN(
        "An array is created from input type uint16_t as the template parameters and with 2 uint32 "
        "as the input values") {
      auto data = PF::Util::Containers::make_array<uint16_t>(0x76b5f627, 0x5ab627);
      THEN("The array has the expected values of uint16_t at every index") {
        REQUIRE(data[0] == 0xf627);
        REQUIRE(data[1] == 0xb627);
      }
    }

    WHEN(
        "An array is created from input type uint16_t as the template parameters and with 2 uint16 "
        "as the input values") {
      auto data = PF::Util::Containers::make_array<uint16_t>(0xe72b, 0x2b25);
      THEN("The array has expected values at every index ") {
        std::array<uint16_t, 2> expected = {0xe72b, 0x2b25};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
    }

    WHEN(
        "An array is created from input type uint32_t as the template parameters and with 2 uint32 "
        "input values ") {
      auto data = PF::Util::Containers::make_array<uint32_t>(0xcf7e59a4, 0xb10ea2ef);
      THEN("The array has the expected values at every index") {
        std::array<uint32_t, 2> expected = {0xcf7e59a4, 0xb10ea2e};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
    }
    WHEN(
        "An array is created from input type uint32_t as the template parameters and with 2 "
        "uint8_t input values") {
      auto data = PF::Util::Containers::make_array<uint32_t>(0x0f, 0x36);
      THEN("The array has expected values at every index") {
        REQUIRE(data[0] == 0x0f);
        REQUIRE(data[1] == 0x36);
      }
    }
    WHEN(
        "An array is created from input type uint32_t as the template parameters and with 2 "
        "uint16_t input values") {
      auto data = PF::Util::Containers::make_array<uint32_t>(0xbcf9, 0xe11b);
      THEN("The array has expected values at every index") {
        REQUIRE(data[0] == 0xbcf9);
        REQUIRE(data[1] == 0xe11b);
      }
    }

    WHEN(
        "An array is created from input type int16_t as the template parameters and with 2 int16  "
        "input values") {
      auto data = PF::Util::Containers::make_array<int16_t>(1024, 1120);
      THEN("The array has the expected values at every index") {
        std::array<int16_t, 2> expected = {1024, 1120};
        for (int i = 0; i < 1; i++) {
          REQUIRE(data[i] == expected[i]);
        }
      }
    }
    WHEN(
        "An array is created from two struct instances with internal values of 0xb9, 0xe2, 0xa7 "
        "and  0x31 ") {
      struct Test {
        uint8_t val;
      };

      Test val1{0xb9};
      Test val2{0xe2};
      Test val3{0xa7};
      Test val4{0x31};
      auto data = PF::Util::Containers::make_array<Test>(val1, val2, val3, val4);
      THEN(" the array has the expected values, 0xb9, 0xe2, 0xa7 and 0x31") {
        REQUIRE(data[0].val == 0xb9);
        REQUIRE(data[1].val == 0xe2);
        REQUIRE(data[2].val == 0xa7);
        REQUIRE(data[3].val == 0x31);
      }
    }
  }
}
SCENARIO("Util:Array make_array function works properly for const and non-const references") {
  GIVEN("Util:Array make_array function") {
    WHEN("an array is created from two elements of structure with values 0x02 and 0x0f") {
      struct Test {
        uint8_t val;
      };

      Test val1{0x02};
      Test val2{0x0f};

      auto data = PF::Util::Containers::make_array<std::reference_wrapper<const Test>>(val1, val2);
      THEN(" the array has the expected values, 0x02 and 0x0f") {
        REQUIRE(data[0].get().val == 0x02);
        REQUIRE(data[1].get().val == 0x0f);
      }

      THEN(
          "Element values of structure are changed to 0x03 and 0xde, values of array at index 0 "
          "and at index 1 changes to 0x03 and 0xde") {
        val1.val = 0x03;
        val2.val = 0xde;
        REQUIRE(data[0].get().val == 0x03);
        REQUIRE(data[1].get().val == 0xde);
      }

      THEN("Values of an array can't be changed as elements in array are const reference") {
        // data[0].get().val = 0x01;
        // data[1].get().val = 0x86;
        REQUIRE(val1.val == 0x02);
        REQUIRE(val2.val == 0x0f);
      }
    }

    WHEN("an array is created from two non const elements of structure with values 0x67 and 0x3e") {
      struct Test {
        uint8_t val;
      };

      Test val1{0x67};
      Test val2{0x3e};
      auto result = PF::Util::Containers::make_array<std::reference_wrapper<Test>>(val1, val2);
      THEN("The array has the expected values, 0x67 and 0x3e") {
        REQUIRE(result[0].get().val == 0x67);
        REQUIRE(result[1].get().val == 0x3e);
      }
      THEN(
          "Values of non-const reference elements from structure are changed to 0xa3 and 0xee, "
          "values of array at index 0 "
          "and at index 1 changes to 0xa3 and 0xee") {
        val1.val = 0xa3;
        val2.val = 0xee;
        REQUIRE(result[0].get().val == 0xa3);
        REQUIRE(result[1].get().val == 0xee);
      }
      THEN(
          "changing the value of the 0th index of an array to 0x01 and 1st index to 0x86 changes "
          "value of elements from structure to 0x01 and 0x86 ") {
        result[0].get().val = 0x01;
        result[1].get().val = 0x86;
        REQUIRE(val1.val == 0x01);
        REQUIRE(val2.val == 0x86);
      }
    }
  }
}
