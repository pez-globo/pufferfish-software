/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * TaggedUnion.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior of TaggedUnions
 *
 */
#include "Pufferfish/Util/TaggedUnion.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The TaggedUnion class behaves properly", "[taggedunion]") {
  enum class TestEnum : uint8_t { ok = 0, invalid = 1 };

  GIVEN("A Taggedunion constructed with a union of uint8_t and uint16_t values and an enum class") {
    union TestUnion {
      uint8_t first;
      uint16_t second;
    };

    using TestTaggedUnion = PF::Util::TaggedUnion<TestUnion, TestEnum>;

    WHEN("Two instances of given taggedunion for the uint8_t field are copied by value") {
      TestTaggedUnion tag1{};
      TestTaggedUnion tag2{};
      TestUnion test_union{};
      test_union.first = 10;  // NOLINT(cppcoreguidelines-pro-type-union-access)

      tag1.value = test_union;

      tag2 = tag1;

      THEN("The taggedunion values field are as expected") {
        REQUIRE(tag2.value.first == 10);  // NOLINT(cppcoreguidelines-pro-type-union-access)
      }
    }

    WHEN("Two instances of given taggedunion for the uint16_t field are copied by value") {
      TestTaggedUnion tag1{};
      TestTaggedUnion tag2{};
      TestUnion test_union{};
      test_union.second = 0xffff;  // NOLINT(cppcoreguidelines-pro-type-union-access)

      tag1.value = test_union;

      tag2 = tag1;

      THEN("The taggedunion values field are as expected") {
        REQUIRE(tag2.value.second == 0xffff);  // NOLINT(cppcoreguidelines-pro-type-union-access)
      }
    }
  }
}
