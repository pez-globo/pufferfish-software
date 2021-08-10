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

#include "Pufferfish/Util/Containers/EnumMap.h"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
SCENARIO("The EnumMap is constructed from initializer list works corretly") {
  GIVEN("An EnumMap constructed with capacity 11, has {1, 1}, {2,2}, {3,3}, {4,4} items") {
    enum Test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
    };
    PF::Util::Containers::EnumMap<Test, uint32_t, 11> map{
        {test1, 1}, {test2, 2}, {test3, 3}, {test4, 4}};
    WHEN("The size method is called") {
      auto size = map.size();
      THEN("The size method returns size as 4") { REQUIRE(size == 4); }
    }
    WHEN("The avaliable method is called ") {
      auto avaliable = map.available();
      THEN("The avaliable method reports that 7 items are avlaible ") { REQUIRE(avaliable == 7); }
    }
    WHEN("The full method is called") {
      bool full_status = map.full();
      THEN("The full method reports that the EnumMap is not full") {
        REQUIRE(full_status == false);
      }
    }
    WHEN("The empty method is called") {
      bool empty = map.empty();
      THEN("The empty method reports that the EnumMap is not empty") { REQUIRE(empty == false); }
    }
    WHEN("The has method is called") {
      for (size_t i = 1; i < 5; i++) {
        bool status = map.has(Test(i));
        THEN("The has method returns true for 1-4 keys") { REQUIRE(status == true); }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("Indexing thorugh EnumMap") {
      for (size_t i = 1; i < 5; i++) {
        auto result = map[(Test(i))];
        THEN(
            "Indexing into the containers for keys 1, 2, 3, 4 returns the values 1, 2, 3, 4 "
            "respectively") {
          REQUIRE(result == i);
        }
      }
    }

    WHEN("The input method is called on 5th item with value as 5") {
      auto status = map.input(test5, 5);
      THEN("The input method returns ok status for 5th item") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 5 ") {
        REQUIRE(map.size() == 5);
      }
      THEN("The avaliable method reports that 6 items are avaliable") {
        REQUIRE(map.available() == 6);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-5 keys") {
        for (size_t i = 1; i < 6; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN(
          "After input method, itering through EnumMap returns expected sequence of data for 1-5 "
          "keys") {
        for (size_t i = 1; i < 6; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("The output method is called on 2nd and 4th item") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test2, val);
      auto op_status1 = map.output(test4, val1);
      THEN("The output  method returns ok status for 2nd item") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected value for 2nd key") { REQUIRE(val == 2); }
      THEN("The output  method returns ok status for 4th item") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected values for 4th key") { REQUIRE(val1 == 4); }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 4") {
        REQUIRE(map.size() == 4);
      }
      THEN("The avaliable method reports that 7 items are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-4 keys") {
        for (size_t i = 1; i < 5; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }

      THEN("After output method, iterating through 1-4 keys returns expected data") {
        for (size_t i = 1; i < 5; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on 4th and 5th key") {
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
      THEN("The avaliable method reports that 8 items are avaliable") {
        REQUIRE(map.available() == 8);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-3 keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 4th key") { REQUIRE(map.has(test4) == false); }
      THEN("After erase method, iterating through 1-3 items returns expected data") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The clear method is called on EnumMap") {
      map.clear();
      THEN("The has method returns false for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("The avaliable method reports that 11 items are avaliable") {
        REQUIRE(map.available() == 11);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is empty") { REQUIRE(map.empty() == true); }
    }
  }
}
SCENARIO("EnumMap construected form initializer list gives expected results") {
  GIVEN("An EnumMap constructed with capacity 6, has {1, 1}{2,2}{3,3}{4,4}{5,5}{6,6} items") {
    enum Test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
    };
    PF::Util::Containers::EnumMap<Test, uint32_t, 7> map{
        {test1, 1}, {test2, 2}, {test3, 3}, {test4, 4}, {test5, 5}, {test6, 6}};
    WHEN("The size method is called") {
      auto size = map.size();
      THEN("The size method returns size as 6") { REQUIRE(size == 6); }
    }
    WHEN("The avaliable method is called ") {
      auto avaliable = map.available();
      THEN("The avaliable method reports that 1 item is avlaible ") { REQUIRE(avaliable == 1); }
    }
    WHEN("The full method is called") {
      bool full_status = map.full();
      THEN("The full method reports that the EnumMap is not full") {
        REQUIRE(full_status == false);
      }
    }
    WHEN("The empty method is called") {
      bool empty = map.empty();
      THEN("The empty method reports that the EnumMap is non-empty") { REQUIRE(empty == false); }
    }
    WHEN("The has method is called on 1-6 keys") {
      for (size_t i = 1; i < 7; i++) {
        bool status = map.has(Test(i));
        THEN("The has method returns true for 1-6 keys") { REQUIRE(status == true); }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("Indexing through 0-5 keys") {
      for (size_t i = 0; i < 6; i++) {
        auto result = map.operator[](Test(i));
        THEN(
            "Indexing into the containers for keys 1, 2, 3, 4, 5 and 6 returns the values 1, 2, 3, "
            "4,5 and 6 respectively") {
          REQUIRE(result == i);
        }
      }
    }

    WHEN("The input method is called on 7th item with value as 7") {
      auto status = map.input(test7, 7);
      THEN("The input method returns out of bounds status for 7th item") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 7") { REQUIRE(map.max_size() == 7); }
      THEN("After the input method, the size method reports size as 6 ") {
        REQUIRE(map.size() == 6);
      }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports out of bounds  status for 7th key ") {
        REQUIRE(map.erase(test7) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The has method returns true for 1-6 keys") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("After input method, iterating through 1-6 keys return expected sequence of data") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("The output method is called on 2nd and 4th key") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test2, val);
      auto op_status1 = map.output(test4, val1);
      THEN("The output  method returns ok status for 2nd item") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected value for 2nd key") { REQUIRE(val == 2); }
      THEN("The output  method returns ok status for 4th item") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected values for 4th key") { REQUIRE(val1 == 4); }
      THEN("The max_size method reports, EnumMap has capacity 7") { REQUIRE(map.max_size() == 7); }
      THEN("After the input method, the size method reports size as 6") {
        REQUIRE(map.size() == 6);
      }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 6th key ") {
        REQUIRE(map.erase(test6) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-6 keys") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }

      THEN("After output method, iterating through items 1-6 returns expected data") {
        for (size_t i = 0; i < 6; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on 4th and 7th key") {
      auto status = map.erase(test4);
      auto status1 = map.erase(test7);
      THEN("The erase method returns ok status for 4th key") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds for 7th key") {
        REQUIRE(status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 7") { REQUIRE(map.max_size() == 7); }
      THEN("After the input method, the size method reports size as 5") {
        REQUIRE(map.size() == 5);
      }
      THEN("The avaliable method reports that 2 items are avaliable") {
        REQUIRE(map.available() == 2);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-3 ,5th and 6th  keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
        REQUIRE(map.has(test5) == true);
        REQUIRE(map.has(test6) == true);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 4th key") { REQUIRE(map.has(test4) == false); }
      THEN("After erase method, iterating through 1-3 items and 5-6 items returns expected data") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
        REQUIRE(map[test5] == 5);
        REQUIRE(map[test6] == 6);
      }
    }
    WHEN("The clear method is called") {
      map.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 7") { REQUIRE(map.max_size() == 7); }
      THEN("The avaliable method reports that 7 items are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is empty") { REQUIRE(map.empty() == true); }
    }
  }
}

SCENARIO("The input method in EnumMap works correctly") {
  GIVEN("An empty EnumMap with capacity 11") {
    enum Test {
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
    };
    PF::Util::Containers::EnumMap<Test, uint32_t, 11> map;
    WHEN(
        "The input method is called on given EnumMap for {1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8} "
        "items") {
      for (size_t i = 1; i <= 8; i++) {
        auto in_status = map.input(Test(i), i);

        THEN("The input method returns ok status for 8 items") {
          REQUIRE(in_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 8 ") {
        REQUIRE(map.size() == 8);
      }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status for 9th key ") {
        REQUIRE(map.erase(test9) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The has method returns true for 1-8 keys") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 9th and 10th key") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("Iterating through 1-8 items returns expected sequence of data") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map[Test(i)] == i);
        }
      }
    }
  }
  GIVEN(
      "Partially filled EnumMap with capacity 11 and has {1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8} "
      "items") {
    enum Test {
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
    };
    PF::Util::Containers::EnumMap<Test, uint32_t, 11> map;
    for (size_t i = 1; i <= 8; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }
    WHEN("The input method is called on 7th and 8th item with 10 and 20 as values") {
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
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-8 keys ") {
        for (size_t i = 1; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 9th and 10th keys") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("Iterating through 1-6 items returns expected sequence of data") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
      THEN("Iterating through key 7 and 8 gives expected values as 10 and 20 respectively") {
        REQUIRE(map.operator[](test7) == 10);
        REQUIRE(map.operator[](test8) == 20);
      }
    }
    WHEN(
        "The input method is called on  9th,10th,11th and 12th items with values 9,10,11 and 12 "
        "respectively") {
      auto in_status9 = map.input(test9, 9);
      auto in_status10 = map.input(test10, 10);
      auto in_status = map.input(test11, 11);
      auto in_status1 = map.input(test12, 12);
      THEN("The input method returns ok status for 9th and 10th items") {
        REQUIRE(in_status9 == PF::IndexStatus::ok);
        REQUIRE(in_status10 == PF::IndexStatus::ok);
      }
      THEN("The input method returns out of bounds status for 11th and 12th items") {
        REQUIRE(in_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(in_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 10 ") { REQUIRE(map.size() == 10); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 10th key ") {
        REQUIRE(map.erase(test10) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-10 keys") {
        for (size_t i = 1; i <= 10; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for  11th and 12th key") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("Iterating through 1-10 items returns expected sequence of data") {
        for (size_t i = 1; i < 11; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN(
        "The input method is used on erased 1st and 2nd item with values 20 and 100 "
        "respectively ") {
      auto erase_status = map.erase(test1);
      auto erase_status1 = map.erase(test2);
      auto in_status = map.input(test1, 20);
      auto in_status1 = map.input(test2, 100);
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 8 ") { REQUIRE(map.size() == 8); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-8 keys") {
        for (size_t i = 1; i <= 8; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 1-8 items returns expected sequence of data") {
        for (size_t i = 3; i < 9; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
      THEN("Iterating through key 1 and 2 returns expected data as 20 and 100") {
        REQUIRE(map.operator[](test1) == 20);
        REQUIRE(map.operator[](test2) == 100);
      }
    }
  }
}

SCENARIO("The input method in EnumMap works correctly for struct, ref and weird values") {
  GIVEN("An empty EnumMap with capacity 4") {
    enum Test {
      test1,
      test2,
      test3,
      test4,
    };
    PF::Util::Containers::EnumMap<Test, int32_t, 4> map;
    WHEN(
        "The input method is called thrice on 1,2 and 3 keys for values -10, -100 and -9658 "
        "respectively") {
      auto in_status = map.input(test1, -10);
      auto in_status1 = map.input(test2, -100);
      auto in_status2 = map.input(test3, -9658);
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 1st key") {
        REQUIRE(map.operator[](test1) == -10);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        REQUIRE(map.operator[](test2) == -100);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        REQUIRE(map.operator[](test3) == -9658);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input method, the size method reports size as 3 ") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports out of bounds  status for 4th key ") {
        REQUIRE(map.erase(test4) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN(
        "The input method is called thrice for keys 1,2 and 3 for values 2147483647,-2147483647 "
        "and-32768") {
      auto val1 = 2147483647;
      auto val2 = -2147483647;
      auto val3 = -32768;

      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for 1st key") {
        REQUIRE(map.operator[](test1) == val1);
      }
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        REQUIRE(map.operator[](test2) == val2);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        REQUIRE(map.operator[](test3) == val3);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 3rd key ") {
        REQUIRE(map.erase(test3) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1,2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
  }
}
SCENARIO("The input method in EnumMap works correctly for struct and pointers") {
  GIVEN("An empty EnumMap with capacity 4") {
    struct Result {
      int val;
    };

    Result val1{121};
    Result val2{02};
    Result val3{898};
    Result val4{0};
    Result val5{0};
    Result val6{0};
    Result val7{10};
    Result val8{20};
    Result val9{30};

    enum Test {
      test1,
      test2,
      test3,
    };
    PF::Util::Containers::EnumMap<Test, Result, 4> map;

    WHEN(
        "The input method is called thrice for 1,2 and 3 key, where values are elemets of struct "
        "121, 2 and 898 respectively") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for 1st key") {
        map.output(test1, val4);
        REQUIRE(val4.val == 121);
      }
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        map.output(test2, val5);
        REQUIRE(val5.val == 2);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        map.output(test3, val6);
        REQUIRE(val6.val == 898);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 3rd key ") {
        REQUIRE(map.erase(test3) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1,2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The map values are correctly set by assignment on the [] operator:") {
        map[test1] = val7;
        map[test2] = val8;
        map[test3] = val9;
        REQUIRE(map[test1].val == 10);
        REQUIRE(map[test2].val == 20);
        REQUIRE(map[test3].val == 30);
      }
      THEN("The struct fields are correctly set by direct access using the [] operator:") {
        map[test1].val = 122;
        map[test2].val = 3;
        map[test3].val = 899;
        REQUIRE(map[test1].val == 122);
        REQUIRE(map[test2].val == 3);
        REQUIRE(map[test3].val == 899);
      }
    }
  }
  GIVEN("An empty EnumMap with capacity 4") {
    uint32_t var1 = 10;
    uint32_t* val1 = &var1;
    uint32_t var2 = 56;
    uint32_t* val2 = &var2;
    uint32_t var3 = 28;
    uint32_t* val3 = &var3;
    uint32_t var4 = 120;
    uint32_t* val4 = &var4;
    uint32_t var5 = 122;
    uint32_t* val5 = &var5;
    uint32_t var6 = 100;
    uint32_t* val6 = &var6;

    enum Test {
      test1,
      test2,
      test3,
    };
    PF::Util::Containers::EnumMap<Test, uint32_t*, 4> map;

    WHEN("The input method is called for {1,10}{2,56}{3,28} , where values are pointers") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The operator method returns expected value for 1st key") {
        REQUIRE(map.operator[](test1) == val1);
      }
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 2nd key") {
        REQUIRE(map.operator[](test2) == val2);
        // std::cout << "value1" << val2 << std::endl;
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The operator method returns expected value for 3rd key") {
        REQUIRE(map.operator[](test3) == val3);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, EnumMap has capacity 4") { REQUIRE(map.max_size() == 4); }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 3rd key ") {
        REQUIRE(map.erase(test3) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1,2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The map values are correctly set by assignment on the [] operator:") {
        map[test1] = val4;
        map[test2] = val5;
        map[test3] = val6;
        REQUIRE(map[test1] == val4);
        REQUIRE(map[test2] == val5);
        REQUIRE(map[test3] == val6);
      }
    }
  }
}
SCENARIO("The method output in EnumMap works corretly") {
  GIVEN(
      "Partially filled  EnumMap with capacity 12 , which has "
      "{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8}{9,9} items") {
    enum Test {
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
    };
    PF::Util::Containers::EnumMap<Test, uint32_t, 12> map;
    for (size_t i = 1; i < 10; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }
    WHEN("The output method is used on 1st and 8th item") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;

      auto op_status = map.output(test1, val);
      auto op_status1 = map.output(test8, val1);
      THEN("The output method returns ok status for 1st item") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it returns expected value for 1st key") { REQUIRE(val == 1); }
      THEN("The output method returns ok status for 8th item") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it returns expected value for 8th key") { REQUIRE(val1 == 8); }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 9th key ") {
        REQUIRE(map.erase(test9) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status for 10th key ") {
        REQUIRE(map.erase(test10) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The has method returns true for 1-9 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 11th and 12th keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("Iterating through 1-9 items returns expected sequence of data") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("The output method is used on 11th(capacity-1) and 12th(capacity) item") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test11, val);
      auto op_status1 = map.output(test12, val1);
      THEN(
          "The output method returns out of bounds status for 11th(capacity-1) and 12th(capacity) "
          "item") {
        REQUIRE(op_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(op_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 9th key ") {
        REQUIRE(map.erase(test9) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-9 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 11th and 12th keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 1-9 items returns expected sequence of data") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The output method is used on erased 8th and 9th item") {
      map.erase(test8);
      map.erase(test9);
      uint32_t x = 0;
      uint32_t& val = x;
      auto output_status = map.output(test8, val);
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto output_status1 = map.output(test9, val1);
      THEN("The output method gives out of bounds status for 8th item") {
        REQUIRE(output_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The output method gives out of bounds status for 9th item") {
        REQUIRE(output_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the output method, the size method reports size as 7 ") {
        REQUIRE(map.size() == 7);
      }
      THEN("The avaliable method reports that 5 items are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports out of bounds status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The has method returns true for 1-8 keys") {
        for (size_t i = 1; i < 8; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 8th and 9th keys ") {
        REQUIRE(map.has(test8) == false);
        REQUIRE(map.has(test9) == false);
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 1-7 items returns expected sequence of data") {
        for (size_t i = 1; i < 8; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
  }
}

SCENARIO("The method erase in EnumMap works corretly") {
  GIVEN(
      "Partially filled EnumMap with capacity 12 and has "
      "{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8}{9,9} items") {
    enum Test {
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
    };

    PF::Util::Containers::EnumMap<Test, uint32_t, 12> map;
    for (size_t i = 1; i < 10; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The method erase is called on 1-4 items") {
      for (size_t i = 1; i < 5; i++) {
        // std::cout<< "value" << i << std::endl;
        auto erase_status = map.erase(Test(i));
        THEN("The erase method returns ok status for 1-4 items") {
          REQUIRE(erase_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 5 ") { REQUIRE(map.size() == 5); }
      THEN("The avaliable method reports that 7 items are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 5,6,7,8 and 9 keys") {
        for (size_t i = 5; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 5,6,7,8 and 9 items returns expected sequence of data") {
        for (size_t i = 5; i < 10; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on 11th(capacity-1) and 12th(capacity) items") {
      auto erase_status = map.erase(test11);
      auto erase_status1 = map.erase(test12);
      THEN(
          "The erase method returns out of bounds status for 11th(capacity-1) and 12th(capacity) "
          "items") {
        REQUIRE(erase_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(erase_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-9 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 1-9 items returns expected sequence of data") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
    WHEN("The erase method is called on 8,9 and 10th item") {
      auto erase_status = map.erase(test8);
      auto erase_status1 = map.erase(test9);
      auto erase_status2 = map.erase(test10);
      THEN("The first erase method returns ok status for 8th item") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The second erase method returns ok status for 9th item") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The third erase method returns out of bounds status") {
        REQUIRE(erase_status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 7") { REQUIRE(map.size() == 7); }
      THEN("The avaliable method reports that 5 items are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the EnumMap is not full") { REQUIRE(map.full() == false); }
      THEN("The empty method reports that the EnumMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-5 ") {
        for (size_t i = 1; i < 6; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 8th and 9th key") {
        for (size_t i = 8; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("Iterating through 1-6 items returns expected sequence of data") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.operator[](Test(i)) == i);
        }
      }
    }
  }
}

SCENARIO("The method clear in EnumMap works corretly") {
  GIVEN(
      "An partially filled EnumMap with capacity 12 ,and has "
      "{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8}{9,9}{10,10}{11,11}") {
    enum Test {
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
    };

    PF::Util::Containers::EnumMap<Test, uint32_t, 12> map;
    for (size_t i = 1; i < 11; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The clear method is called") {
      map.clear();
      THEN("The has method returns false for 1-10 keys") {
        for (size_t i = 0; i < 11; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, EnumMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("The avaliable method reports that 12 items are avaliable") {
        REQUIRE(map.available() == 12);
      }
      THEN("The full method reports that the EnumMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the EnumMap is empty") { REQUIRE(map.empty() == true); }
    }
  }
}