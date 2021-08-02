/*
 * EnumMap.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 *  A statically-allocated map for a finite, pre-determined set of keys.
 *  Backed by an array of keys and an array of values. Methods use early
 *  returns of status codes instead of exceptions for error handling, for
 *  bounds-checking.
 *  This Map is designed for frequent lookups but doesn't support traversal.
 */

#pragma once

#include "Pufferfish/Util/Containers/EnumMap.h"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
SCENARIO("The method initializer list in EnumMap works corretly") {
  GIVEN("An EnumMap constructed with capacity 11, is partially filled with 4 bytes of data") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 11> map{
        {test1, 1}, {test2, 2}, {test3, 3}, {test4, 4}};
    WHEN("The input method is used on EnumMap") {
      auto status = map.input(test5, 5);
      THEN("The input method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 5 ") {
        REQUIRE(map.size() == 5);
      }
      THEN("The avaliable method reports that 6 key-value pair are avaliable") {
        REQUIRE(map.available() == 6);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 5 keys") {
        for (size_t i = 1; i < 6; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("After input method, operator method returns expected sequence of data") {
        for (size_t i = 1; i < 6; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The output method is called on EnumMap") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test2, val);
      auto op_status1 = map.output(test4, val1);
      THEN("The output  method returns ok status for 2nd key") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected value for 2nd key") { REQUIRE(val == 2); }
      THEN("The output  method returns ok status for 4th key") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected values for 4th key") { REQUIRE(val1 == 4); }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 4") {
        REQUIRE(map.size() == 4);
      }
      THEN("The avaliable method reports that 7 key-value pair are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 4 keys") {
        for (size_t i = 1; i < 5; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("After output method, operator method returns expected data") {
        for (size_t i = 1; i < 5; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on given EnumMap") {
      auto status = map.erase(test4);
      auto status1 = map.erase(test5);
      THEN("The erase method returns ok status for 4th key") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds for 5th key") {
        REQUIRE(status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 3") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 8 key-value pair are avaliable") {
        REQUIRE(map.available() == 8);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 4th key") { REQUIRE(map.has(test4) == false); }
      THEN("After erase method, operator method returns expected data") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The clear method is called on EnumMap") {
      map.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(map.has(test(i)) == false);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the clear method, the size method reports size as 0 ") {
        REQUIRE(map.size() == 0);
      }
      THEN("The avaliable method reports that 11 key-value pair are avaliable") {
        REQUIRE(map.available() == 11);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is empty") { REQUIRE(map.empty() == true); }
    }
  }
}

SCENARIO("The input method in EnumMap works correctly") {
  GIVEN("An empty EnumMap with capacity 11") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
      test8 = 8,
      test9 = 9,
      test10 = 10,
      test11 = 11,
      test12 = 12,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 11> map;
    WHEN("The input method is called on given EnumMap for 8 key-value pair") {
      for (size_t i = 1; i <= 8; i++) {
        auto in_status = map.input(test(i), i);

        THEN("The input method returns ok status for 8 key-value pairs") {
          REQUIRE(in_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 8 ") {
        REQUIRE(map.size() == 8);
      }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 8 keys") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 9th and 10th key") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("The EnumMap has expected 8 sequence of key-value pair") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 11 and has 8 keys") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
      test8 = 8,
      test9 = 9,
      test10 = 10,
      test11 = 11,
      test12 = 12,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 11> map;
    for (size_t i = 1; i <= 8; i++) {
      auto in_status = map.input(test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }
    WHEN("The input method is called on 7th and 8th key-value pair") {
      auto in_status = map.input(test7, 10);
      auto in_status1 = map.input(test8, 20);
      THEN("The input method return ok status for 7th key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method return ok status for 8th key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 8 ") {
        REQUIRE(map.size() == 8);
      }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 8 keys ") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 9th and 10th keys") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("The EnumMap has expected 8 sequence of key-value pair") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
        REQUIRE(map.operator[](test7) == 10);
        REQUIRE(map.operator[](test8) == 20);
      }
    }
    WHEN("The input method is used on  11th and 12th Key-value pair") {
      for (size_t i = 1; i < 9; i++) {
        auto in_status = map.input(test(i), i);
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      auto in_status9 = map.input(test9, 9);
      auto in_status10 = map.input(test10, 10);
      auto in_status = map.input(test11, 11);
      auto in_status1 = map.input(test12, 12);
      THEN("The input method returns ok status for 9th amd 10th key") {
        REQUIRE(in_status9 == PF::IndexStatus::ok);
        REQUIRE(in_status10 == PF::IndexStatus::ok);
      }
      THEN("The input method returns out of bounds status for 11th and 12th key") {
        REQUIRE(in_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(in_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 10 ") { REQUIRE(map.size() == 10); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 10 keys") {
        for (size_t i = 1; i <= 10; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for  11th and 12th key") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The EnumMap has expected 10 sequqnce of key-value pair") {
        for (size_t i = 1; i < 11; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The input method is used on erased key-value pair ") {
      auto erase_status = map.erase(test1);
      auto erase_status1 = map.erase(test2);
      auto in_status = map.input(test1, 20);
      auto in_status1 = map.input(test2, 100);
      THEN("The input method returns ok status for 8th key") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 9th key") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 8 ") { REQUIRE(map.size() == 8); }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 8 keys") {
        for (size_t i = 1; i <= 8; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The EnumMap has expected 8 sequqnce of key-value pair") {
        for (size_t i = 3; i < 9; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
        REQUIRE(map.operator[](test1) == 20);
        REQUIRE(map.operator[](test2) == 100);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 4") {
    typedef enum _test {
      test1,
      test2,
      test3,
    } test;
    PF::Util::Containers::EnumMap<test, int32_t, 4> map;
    WHEN("The input method is called thrice for negative values") {
      auto in_status = map.input(test1, -10);
      auto in_status1 = map.input(test2, -100);
      auto in_status2 = map.input(test3, -9658);
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 1st key") {
        REQUIRE(map.operator[](test1) == -10);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        REQUIRE(map.operator[](test2) == -100);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        REQUIRE(map.operator[](test3) == -9658);
      }
      THEN("The max_size method reports, EnumMap has capacity 10") { REQUIRE(map.max_size() == 4); }
      THEN("After the input method, the size method reports size as 3 ") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
    WHEN("The input method is called thrice for large values") {
      auto val1 = 2147483647;
      auto val2 = -2147483647;
      auto val3 = -32768;

      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for 1st key") {
        REQUIRE(map.operator[](test1) == val1);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        REQUIRE(map.operator[](test2) == val2);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        REQUIRE(map.operator[](test3) == val3);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 4") {
    typedef enum _test {
      test1,
      test2,
      test3,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 4> map;

    WHEN("The input method is called thrice ,where value is passed by const reference") {
      uint8_t const& val1 = 12;
      uint8_t const& val2 = 8;
      uint8_t const& val3 = 23;

      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for first key") {
        REQUIRE(map.operator[](test1) == val1);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for second key") {
        REQUIRE(map.operator[](test2) == val2);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for third key") {
        REQUIRE(map.operator[](test3) == val3);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
    WHEN("The input method is called thrice where value is passed by non-const reference ") {
      uint32_t re1 = 12856;
      uint32_t& val1 = re1;
      uint32_t re2 = 8951;
      uint32_t& val2 = re2;
      uint32_t re3 = 223;
      uint32_t& val3 = re3;
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for first key") {
        REQUIRE(map.operator[](test1) == val1);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for second key") {
        REQUIRE(map.operator[](test2) == val2);
      }
      THEN("The input method returns ok status for second key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for third key") {
        REQUIRE(map.operator[](test3) == val3);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 4") {
    struct Test {
      int val;
    };

    Test val1{121};
    Test val2{02};
    Test val3{898};
    // val1.val = 121;
    // val2.val = 2;
    // val3.val = 65;

    typedef enum _test {
      test1,
      test2,
      test3,
    } test;
    PF::Util::Containers::EnumMap<test, Test, 4> map;

    WHEN("The input method is called, where values are elemets of struct") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for first key") {
        map.output(test1, val1);
        REQUIRE(val1.val == 121);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for second key") {
        map.output(test2, val2);
        REQUIRE(val2.val == 2);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for third key") {
        map.output(test3, val3);
        REQUIRE(val3.val == 898);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 4") {
    uint32_t* val1;
    uint32_t var1 = 10;
    val1 = &var1;
    uint32_t* val2;
    uint32_t var2 = 56;
    val2 = &var2;
    uint32_t* val3;
    uint32_t var3 = 28;
    val3 = &var3;

    typedef enum _test {
      test1,
      test2,
      test3,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t*, 4> map;

    WHEN("The input method is called, where values are pointers") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for first key") {
        map.output(test1, val1);
        REQUIRE(val1 == val1);
      }
      THEN("The input method returns ok status for first key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for second key") {
        map.output(test2, val2);
        REQUIRE(val2 == val2);
      }
      THEN("The input method returns ok status for second key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for third key") {
        map.output(test3, val3);
        REQUIRE(val3 == val3);
      }
      THEN("The input method returns ok status for third key") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
    }
  }
}
SCENARIO("The method output in EnumMap works corretly") {
  GIVEN("An empty EnumMap with capacity 12") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
      test8 = 8,
      test9 = 9,
      test10 = 10,
      test11 = 11,
      test12 = 12,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 12> map;
    for (size_t i = 1; i < 10; i++) {
      auto in_status = map.input(test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }
    WHEN("The output method is used on 1st and 8th key") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;

      auto op_status = map.output(test1, val);
      auto op_status1 = map.output(test8, val1);
      THEN("The output method returns ok status for 1st key") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it returns expected value for 1st key") { REQUIRE(val == 1); }
      THEN("The output method returns ok status for 1st key") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it returns expected value for 8th key") { REQUIRE(val1 == 8); }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 8 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 9th and 10 keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The EnumMap has expected 8 sequqnce of key-value pair") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The output method is used on 11th and 12th key") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test11, val);
      auto op_status1 = map.output(test12, val1);
      THEN("The output method returns out of bounds status for test11 and test12") {
        REQUIRE(op_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(op_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 8 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 11th and 12th keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The EnumMap has expected 9 sequence of key-value pair") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The output method is used on erased key-value pair") {
      map.erase(test8);
      map.erase(test9);
      uint32_t x = 0;
      uint32_t& val = x;
      auto output_status = map.output(test8, val);
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto output_status1 = map.output(test9, val1);
      THEN("The output method gives out of bounds status for 8th key") {
        REQUIRE(output_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The output method gives out of bounds status for 9th key") {
        REQUIRE(output_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the output method, the size method reports size as 7 ") {
        REQUIRE(map.size() == 7);
      }
      THEN("The avaliable method reports that 5 key-value pair are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 11th key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 8 keys") {
        for (size_t i = 1; i < 8; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The has method returns false for 8th and 9th keys ") {
        REQUIRE(map.has(test8) == false);
        REQUIRE(map.has(test9) == false);
      }
      THEN("The EnumMap has expected 7 sequence of key-value pair") {
        for (size_t i = 1; i < 8; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 3") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 4> map;
    auto val1 = 10;
    auto val2 = 20;
    auto val3 = 50;
    map.input(test1, val1);
    map.input(test2, val2);
    map.input(test3, val3);

    WHEN("The output method is called  where value of two keys are changed") {
      val1 = 12;
      val2 = 15;
      val3 = 100;
      uint32_t x = 0;
      uint32_t& ref = x;
      uint32_t y = 0;
      uint32_t& ref1 = y;
      uint32_t z = 0;
      uint32_t& ref2 = z;

      auto op_status = map.output(test1, ref);
      auto op_status1 = map.output(test2, ref1);
      auto op_status2 = map.output(test3, ref2);
      THEN("The output method returns ok status for 1st key") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 1st key ") {
        REQUIRE(ref == 10);
      }
      THEN("The output method returns ok status for 2nd key") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 2nd key ") {
        REQUIRE(ref1 == 20);
      }
      THEN("The output method returns ok status for 3rd key") {
        REQUIRE(op_status2 == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 3rd key ") {
        REQUIRE(ref2 == 50);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the output method, the size method reports size as 3 ") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 3") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
    } test;
    PF::Util::Containers::EnumMap<test, uint32_t, 4> map;
    WHEN("The output method is called , where values of reference variables are changed") {
      uint32_t re1 = 20;
      uint32_t& val1 = re1;
      uint32_t re2 = 50;
      uint32_t& val2 = re2;
      uint32_t re3 = 80;
      uint32_t& val3 = re3;
      map.input(test1, val1);
      map.input(test2, val2);
      map.input(test3, val3);
      re1 = 15;
      re2 = 25;
      re3 = 35;

      THEN("The output method returns ok status for 1st key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        REQUIRE(map.output(test1, ref) == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 1st key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        map.output(test1, ref);
        REQUIRE(ref == 20);
      }
      THEN("The output method returns ok status for 2nd key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        REQUIRE(map.output(test2, ref) == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 2nd key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        map.output(test2, ref);
        REQUIRE(ref == 50);
      }
      THEN("The output method returns ok status for 2nd key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        REQUIRE(map.output(test3, ref) == PF::IndexStatus::ok);
      }
      THEN("After output method, it returns initially pushed value for 3rd key") {
        uint32_t x = 0;
        uint32_t& ref = x;
        map.output(test3, ref);
        REQUIRE(ref == 80);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the output method, the size method reports size as 3 ") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 1 key-value pair are avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
    }
  }
}

SCENARIO("The method erase in EnumMap works corretly") {
  GIVEN("Partially filled EnumMap with capacity 12 and has 9 keys") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
      test8 = 8,
      test9 = 9,
      test10 = 10,
      test11 = 11,
      test12 = 12,
    } test;

    PF::Util::Containers::EnumMap<test, uint32_t, 12> map;
    for (size_t i = 1; i < 10; i++) {
      auto in_status = map.input(test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The method erase is called on 4 keys") {
      for (size_t i = 1; i < 5; i++) {
        // std::cout<< "value" << i << std::endl;
        auto erase_status = map.erase(test(i));
        THEN("The erase method returns ok status for 4 keys") {
          REQUIRE(erase_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 5 ") { REQUIRE(map.size() == 5); }
      THEN("The avaliable method reports that 7 key-value pair are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 9 keys") {
        for (size_t i = 5; i < 10; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The EnumMap has expected 5 sequqnce of key-value pair") {
        for (size_t i = 5; i < 10; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on 11th and 12th key-value pair") {
      auto erase_status = map.erase(test11);
      auto erase_status1 = map.erase(test12);
      THEN("The erase method returns out of bounds status for 11th and 12th key") {
        REQUIRE(erase_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(erase_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 key-value pair are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns 9 keys are present in EnumMap") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(test(i)) == true);
        }
      }
      THEN("The EnumMap has expected 9 sequence of key-value pair") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
    WHEN("The erase method is used thrice on given EnumMap") {
      auto erase_status = map.erase(test8);
      auto erase_status1 = map.erase(test9);
      auto erase_status2 = map.erase(test10);
      THEN("The first erase method returns ok status") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The second erase method returns ok status") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The third erase method returns out of bounds status") {
        REQUIRE(erase_status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 7") { REQUIRE(map.size() == 7); }
      THEN("The avaliable method reports that 5 key-value pair are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 5 keys ") {
        for (size_t i = 1; i < 6; i++) {
          // std::cout<< "value" << i << std::endl;
          REQUIRE(map.has(test(i)) == true);
        }
        for (size_t i = 8; i < 10; i++) {
          REQUIRE(map.has(test(i)) == false);
        }
      }
      THEN("The EnumMap has expected 6 sequence of key-value pair") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.operator[](test(i)) == i);
        }
      }
    }
  }
}

SCENARIO("The method clear in EnumMap works corretly") {
  GIVEN("An partially filled EnumMap with capacity 12 and 11 keys") {
    typedef enum _test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
      test8 = 8,
      test9 = 9,
      test10 = 10,
      test11 = 11,
      test12 = 12,
    } test;

    PF::Util::Containers::EnumMap<test, uint32_t, 12> map;
    for (size_t i = 1; i < 11; i++) {
      auto in_status = map.input(test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The clear method is used on EnumMap") {
      map.clear();
      THEN("The has method returns false for 10 keys") {
        for (size_t i = 0; i < 11; i++) {
          REQUIRE(map.has(test(i)) == false);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 0 ") { REQUIRE(map.size() == 0); }
      THEN("The avaliable method reports that 12 key-value pair are avaliable") {
        REQUIRE(map.available() == 12);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is empty") { REQUIRE(map.empty() == true); }
    }
  }
}