/*
 * OrderedMap.cpp
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

#include "Pufferfish/Util/Containers/OrderedMap.h"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
SCENARIO("The OrderedMap is constructed from initializer list works corretly") {
  GIVEN("An OrderedMap constructed with capacity 11, has {1, 0}, {2,1}, {3,2} and {4,3} items") {
    enum Test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
    };
    PF::Util::Containers::OrderedMap<Test, uint32_t, 11> map{
        {test1, 0}, {test2, 1}, {test3, 2}, {test4, 3}};
    WHEN("The size method is called on OrderedMap") {
      auto size = map.size();
      THEN("The size method returns size as 4") { REQUIRE(size == 4); }
    }
    WHEN("The avaliable method is called ") {
      auto avaliable = map.available();
      THEN("The avaliable method reports that 7 items are avlaible ") { REQUIRE(avaliable == 7); }
    }
    WHEN("The full method is called") {
      bool full_status = map.full();
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(full_status == false);
      }
    }
    WHEN("The empty method is called") {
      bool empty = map.empty();
      THEN("The empty method reports that the OrderedMap is non-empty") { REQUIRE(empty == false); }
    }
    WHEN("The has method is called") {
      for (size_t i = 1; i < 5; i++) {
        bool status = map.has(Test(i));
        THEN("The has method returns true for 1-4 keys") { REQUIRE(status == true); }
      }
    }
    WHEN("The items method is called") {
      for (size_t i = 0; i < 4; i++) {
        auto result = map.items()[i].second;
        THEN("items method return expected 4 sequence of data") { REQUIRE(result == i); }
      }
    }

    WHEN("The input method is used on 5th item") {
      auto status = map.input(test5, 4);
      THEN("The input method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 5 ") {
        REQUIRE(map.size() == 5);
      }
      THEN("The avaliable method reports that 6 items are avaliable") {
        REQUIRE(map.available() == 6);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
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
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [6]to[1000] keys") {
        for (size_t i = 6; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After input method, items method returns expected 5 sequence of data") {
        for (size_t i = 0; i < 5; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The output method is called on 2nd and 4th items") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test2, val);
      auto op_status1 = map.output(test4, val1);
      THEN("The output  method returns ok status for 2nd item") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected value for 2nd key") { REQUIRE(val == 1); }
      THEN("The output  method returns ok status for 4th item") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected values for 4th key") { REQUIRE(val1 == 3); }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 4") {
        REQUIRE(map.size() == 4);
      }
      THEN("The avaliable method reports that 7 items are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
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
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [5]to[1000] keys") {
        for (size_t i = 5; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After output method, items method returns expected 4 sequence of data") {
        for (size_t i = 0; i < 4; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The erase method is called on 4th and 5th item") {
      auto status = map.erase(test4);
      auto status1 = map.erase(test5);
      THEN("The erase method returns ok status for 4th key") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds for 5th key") {
        REQUIRE(status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 3") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 8 items are avaliable") {
        REQUIRE(map.available() == 8);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-itemms3 keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 4th key") { REQUIRE(map.has(test4) == false); }
      THEN("After erase method, items method returns expected 3 sequence of data") {
        for (size_t i = 0; i < 3; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The clear method is called on OrderedMap") {
      map.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("The avaliable method reports that 11 items are avaliable") {
        REQUIRE(map.available() == 11);
      }
      THEN("The full method reports that the OrderedMap is not completely filled") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is empty") {
        REQUIRE(map.empty() == true);
      }
    }
  }
}
SCENARIO("OrderedMap construected form initializer list gives expected results") {
  GIVEN("An OrderedMap constructed with capacity 11, is completely filled with 6 bytes of data") {
    enum Test {
      test1 = 1,
      test2 = 2,
      test3 = 3,
      test4 = 4,
      test5 = 5,
      test6 = 6,
      test7 = 7,
    };
    PF::Util::Containers::OrderedMap<Test, uint32_t, 6> map{
        {test1, 0}, {test2, 1}, {test3, 2}, {test4, 3}, {test5, 4}, {test6, 5}};
    WHEN("The size method is called on OrderedMap") {
      auto size = map.size();
      THEN("The size method returns size as 11") { REQUIRE(size == 6); }
    }
    WHEN("The avaliable method is called ") {
      auto avaliable = map.available();
      THEN("The avaliable method reports that 1 items are avlaible ") { REQUIRE(avaliable == 0); }
    }
    WHEN("The full method is called") {
      bool full_status = map.full();
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(full_status == true);
      }
    }
    WHEN("The empty method is called") {
      bool empty = map.empty();
      THEN("The empty method reports that the OrderedMap is non-empty") { REQUIRE(empty == false); }
    }
    WHEN("The has method is called") {
      for (size_t i = 1; i < 7; i++) {
        bool status = map.has(Test(i));
        THEN("The has method returns true for 1-6 keys") { REQUIRE(status == true); }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [7]to[1000] keys") {
        for (size_t i = 7; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("The items method is called") {
      for (size_t i = 0; i < 6; i++) {
        auto result = map.items()[i].second;
        REQUIRE(result == i);
        THEN(
            "Indexing into the containers for keys 1, 2, 3, 4,5 and 6 returns the values 0,1,2,3,4 "
            "and 5 respectively") {
          REQUIRE(result == i);
        }
      }
    }

    WHEN("The input method is called on 7th item") {
      auto status = map.input(test7, 7);
      THEN("The input method returns out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 6") {
        REQUIRE(map.max_size() == 6);
      }
      THEN("After the input method, the size method reports size as 6 ") {
        REQUIRE(map.size() == 6);
      }
      THEN("The avaliable method reports that 0 items are avaliable") {
        REQUIRE(map.available() == 0);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == true);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-6 keys") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN(
          "Indexing into the containers for keys 1, 2, 3, 4,5 and 6 returns the values 0,1,2,3,4 "
          "and 5 respectively") {
        for (size_t i = 0; i < 6; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [7]to[1000] keys") {
        for (size_t i = 7; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN("The output method is called on 2nd item and 4th item") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test2, val);
      auto op_status1 = map.output(test4, val1);
      THEN("The output  method returns ok status for 2nd key") {
        REQUIRE(op_status == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected value for 2nd key") { REQUIRE(val == 1); }
      THEN("The output  method returns ok status for 4th key") {
        REQUIRE(op_status1 == PF::IndexStatus::ok);
      }
      THEN("After the output method, it gives expected values for 4th key") { REQUIRE(val1 == 3); }
      THEN("The max_size method reports, OrderedMap has capacity 6") {
        REQUIRE(map.max_size() == 6);
      }
      THEN("After the input method, the size method reports size as 6") {
        REQUIRE(map.size() == 6);
      }
      THEN("The avaliable method reports that 0 items are avaliable") {
        REQUIRE(map.available() == 0);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == true);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(map.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-6 keys") {
        for (size_t i = 1; i < 7; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [7]to[1000] keys") {
        for (size_t i = 7; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }

      THEN(
          "Indexing into the containers for keys 1, 2, 3, 4,5 and 6 returns the values 0,1,2,3,4 "
          "and 5 respectively") {
        for (size_t i = 0; i < 6; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
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
      THEN("The max_size method reports, OrderedMap has capacity 6") {
        REQUIRE(map.max_size() == 6);
      }
      THEN("After the input method, the size method reports size as 5") {
        REQUIRE(map.size() == 5);
      }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1-3 keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 8; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 4th key") { REQUIRE(map.has(test4) == false); }
      THEN("After erase method, operator method returns expected data {1,0}{2,1}{3,2}") {
        for (size_t i = 0; i < 3; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
        REQUIRE(map.items()[3].second == 4);
        REQUIRE(map.items()[4].second == 5);
      }
    }
    WHEN("The clear method is called on OrderedMap") {
      map.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, OrderedMap has capacity 6") {
        REQUIRE(map.max_size() == 6);
      }
      THEN("The avaliable method reports that 6 items are avaliable") {
        REQUIRE(map.available() == 6);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is empty") {
        REQUIRE(map.empty() == true);
      }
    }
  }
}

SCENARIO("The input method in OrderedMap works correctly") {
  GIVEN("An empty OrderedMap with capacity 11") {
    enum Test {
      test0 = 0,
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
      test13 = 13,
      test14 = 14,
    };
    PF::Util::Containers::OrderedMap<Test, uint32_t, 11> map;
    WHEN(
        "The input method is called on given OrderedMap for "
        "{0,0},{1,1},{2,2}{3,3}{4,4}{5,5}{6,6}{7,7} items") {
      for (size_t i = 0; i < 8; i++) {
        auto in_status = map.input(Test(i), i);

        THEN("The input method returns ok status for 8 items") {
          REQUIRE(in_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 8 ") {
        REQUIRE(map.size() == 8);
      }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 7th key ") {
        REQUIRE(map.erase(test7) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-7 keys") {
        for (size_t i = 0; i < 8; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [8]to[1000] keys") {
        for (size_t i = 8; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 9th and 10th key") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("After input method, items method returns expected 8 sequence of data") {
        for (size_t i = 0; i < 8; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
  }
  GIVEN("An OrderedMap with capacity 11 has {0,0},{1,1},{2,2}{3,3}{4,4}{5,5}{6,6}{7,7} items") {
    enum Test {
      test0 = 0,
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
      test13 = 13,
      test14 = 14,
    };
    PF::Util::Containers::OrderedMap<Test, uint32_t, 11> map;
    for (size_t i = 0; i < 8; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The input method is called on 7th and 8th items") {
      auto in_status = map.input(test7, 7);
      auto in_status1 = map.input(test8, 8);
      THEN("The input method return ok status for 7th item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method return ok status for 8th item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 9 ") {
        REQUIRE(map.size() == 9);
      }
      THEN("The avaliable method reports that 2 items are avaliable") {
        REQUIRE(map.available() == 2);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 7th key ") {
        REQUIRE(map.erase(test7) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-7 keys ") {
        for (size_t i = 0; i < 8; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 9th and 10th keys") {
        REQUIRE(map.has(test9) == false);
        REQUIRE(map.has(test10) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [8]to[1000] keys") {
        for (size_t i = 9; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After input method, items method returns expected 8 sequence of data") {
        for (size_t i = 0; i < 8; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The input method is called on 8th, 9th,10th and 11th item") {
      auto in_status9 = map.input(test8, 8);
      auto in_status10 = map.input(test9, 9);
      auto in_status = map.input(test10, 10);
      auto in_status11 = map.input(test11, 11);
      THEN("The input method returns ok status for 8th, 9th and 10th item") {
        REQUIRE(in_status9 == PF::IndexStatus::ok);
        REQUIRE(in_status10 == PF::IndexStatus::ok);
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns out of bounds status for 11th item") {
        REQUIRE(in_status11 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 11 ") { REQUIRE(map.size() == 11); }
      THEN("The avaliable method reports that 0 items are avaliable") {
        REQUIRE(map.available() == 0);
      }
      THEN("The full method reports that the OrderedMap is completely full") {
        REQUIRE(map.full() == true);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 7th (capacity-3) key ") {
        REQUIRE(map.erase(test7) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-10 keys") {
        for (size_t i = 0; i < 11; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [11]to[1000] keys") {
        for (size_t i = 11; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for 13th(capacity+2) key") {
        REQUIRE(map.has(test13) == false);
      }
      THEN("After input method, items method returns expected 11 sequence of data") {
        for (size_t i = 0; i < 11; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The input method is called on erased 6th and 7th item ") {
      map.erase(test6);
      map.erase(test7);
      auto in_status = map.input(test6, 20);
      auto in_status1 = map.input(test7, 100);
      THEN("The input method returns ok status for 6th item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 7th item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, OrderedMap has capacity 11") {
        REQUIRE(map.max_size() == 11);
      }
      THEN("After the input, the size method reports size as 8 ") { REQUIRE(map.size() == 8); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 0-7 keys") {
        for (size_t i = 0; i < 8; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [8]to[1000] keys") {
        for (size_t i = 8; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After input method, items method returns expected 8 sequence of data") {
        for (size_t i = 0; i < 6; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
        auto result = map.items()[6].second;
        REQUIRE(result == 20);
        auto result1 = map.items()[7].second;
        REQUIRE(result1 == 100);
      }
    }
  }
}
SCENARIO("The input method in OrderedMap works correctly for large values and negative values") {
  GIVEN("An empty OrderedMap with capacity 4") {
    enum Test {
      test1,
      test2,
      test3,
    };
    PF::Util::Containers::OrderedMap<Test, int32_t, 4> map;
    WHEN(
        "The input method is called on 1st,2nd and 3rd key with -10, -100 and -9658 as values "
        "respectively") {
      auto in_status = map.input(test1, -10);
      auto in_status1 = map.input(test2, -100);
      auto in_status2 = map.input(test3, -9658);
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 1st key") {
        auto result = map.items()[0].second;
        auto val = -10;
        REQUIRE(result == val);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 2nd key") {
        auto result = map.items()[1].second;
        auto val = -100;
        REQUIRE(result == val);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 3rd key") {
        auto result = map.items()[2].second;
        auto val = -9658;
        REQUIRE(result == val);
      }
      THEN("The max_size method reports, OrderedMap has capacity 4") {
        REQUIRE(map.max_size() == 4);
      }
      THEN("After the input method, the size method reports size as 3 ") {
        REQUIRE(map.size() == 3);
      }
      THEN("The avaliable method reports that 1 items is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1,2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
    WHEN(
        "The input method is called on 1, 2 and 3 key for 2147483647, -2147483647 and -32768 as "
        "values") {
      auto val1 = 2147483647;
      auto val2 = -2147483647;
      auto val3 = -32768;

      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The items method returns expected value for 1st key") {
        auto result = map.items()[0].second;
        REQUIRE(result == val1);
      }
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 2nd key") {
        auto result = map.items()[1].second;
        REQUIRE(result == val2);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 3rd key") {
        auto result = map.items()[2].second;
        REQUIRE(result == val3);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, OrderedMap has capacity 4") {
        REQUIRE(map.max_size() == 4);
      }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
  }
}
SCENARIO("The input method in OrderedMap works correctly for struct and pointers") {
  GIVEN("An empty OrderedMap with capacity 4") {
    struct Result {
      int val;
    };

    Result val1{121};
    Result val2{02};
    Result val3{898};

    enum Test {
      test1,
      test2,
      test3,
    };
    PF::Util::Containers::OrderedMap<Test, Result, 4> map;

    WHEN("The input method is called, where items are {1,121}{2,2}{3,898}") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The items method returns expected value for 1st key") {
        auto result = map.items()[0].second;
        REQUIRE(result.val == 121);
      }
      THEN("The input method returns ok status for 1st item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for second key") {
        auto result = map.items()[1].second;
        REQUIRE(result.val == 2);
      }
      THEN("The input method returns ok status for 2nd item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for 3rd key") {
        auto result = map.items()[2].second;
        REQUIRE(result.val == 898);
      }
      THEN("The input method returns ok status for 3rd item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, OrderedMap has capacity 4") {
        REQUIRE(map.max_size() == 4);
      }
      THEN("After the input, the size method reports size as 3") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1, 2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
  }
  GIVEN("An empty OrderedMap with capacity 4") {
    uint32_t var1 = 10;
    uint32_t* val1 = &var1;
    uint32_t var2 = 56;
    uint32_t* val2 = &var2;
    uint32_t var3 = 28;
    uint32_t* val3 = &var3;

    enum Test {
      test1,
      test2,
      test3,
    };
    PF::Util::Containers::OrderedMap<Test, uint32_t*, 4> map;

    WHEN("The input method is called, where values are pointers {1,10}{2,56}{3,28}") {
      auto in_status = map.input(test1, val1);
      auto in_status1 = map.input(test2, val2);
      auto in_status2 = map.input(test3, val3);
      THEN("The items method returns expected value for first key") {
        auto* result = map.items()[0].second;
        REQUIRE(result == val1);
      }
      THEN("The input method returns ok status for first item") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for second key") {
        auto* result = map.items()[1].second;
        REQUIRE(result == val2);
      }
      THEN("The input method returns ok status for second item") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The items method returns expected value for third key") {
        auto* result = map.items()[2].second;
        REQUIRE(result == val3);
      }
      THEN("The input method returns ok status for third item") {
        REQUIRE(in_status2 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, OrderedMap has capacity 4") {
        REQUIRE(map.max_size() == 4);
      }
      THEN("After the input, the size method reports size as 3 ") { REQUIRE(map.size() == 3); }
      THEN("The avaliable method reports that 1 item is avaliable") {
        REQUIRE(map.available() == 1);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 1,2 and 3 keys") {
        REQUIRE(map.has(test1) == true);
        REQUIRE(map.has(test2) == true);
        REQUIRE(map.has(test3) == true);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
    }
  }
}

SCENARIO("The method output in OrderedMap works corretly") {
  GIVEN("An OrderedMap with capacity 12 has {0,0}{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8} items") {
    enum Test {
      test0 = 0,
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
    PF::Util::Containers::OrderedMap<Test, uint32_t, 12> map;
    for (size_t i = 0; i < 9; i++) {
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
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-8 keys") {
        for (size_t i = 0; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 11th(capacity-1) and 12th(capacity) keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [9]to[1000] keys") {
        for (size_t i = 9; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output method, items method returns expected 9 sequence of data") {
        for (size_t i = 0; i < 9; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The output method is used on 11th(capacity) and 12th(capacity+1) key") {
      uint32_t x = 0;
      uint32_t& val = x;
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto op_status = map.output(test11, val);
      auto op_status1 = map.output(test12, val1);
      THEN(
          "The output method returns out of bounds status for 11th(capacity) and 12th(capacity+1) "
          "items") {
        REQUIRE(op_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(op_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 8th key ") {
        REQUIRE(map.erase(test8) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-8 keys") {
        for (size_t i = 0; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 11th(capacity) and 12th(capacity+1) keys") {
        REQUIRE(map.has(test11) == false);
        REQUIRE(map.has(test12) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [9]to[1000] keys") {
        for (size_t i = 9; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output method, items method returns expected 9 sequence of data") {
        for (size_t i = 0; i < 9; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The output method is used on erased 7th and 8th items") {
      map.erase(test7);
      map.erase(test8);
      uint32_t x = 0;
      uint32_t& val = x;
      auto output_status = map.output(test8, val);
      uint32_t y = 0;
      uint32_t& val1 = y;
      auto output_status1 = map.output(test9, val1);
      THEN("After erase method, the output method gives out of bounds status for 8th item") {
        REQUIRE(output_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("After erase method, the output method gives out of bounds status for 9th item") {
        REQUIRE(output_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the output method, the size method reports size as 7 ") {
        REQUIRE(map.size() == 7);
      }
      THEN("The avaliable method reports that 5 items are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The erase method reports ok status for 2nd key ") {
        REQUIRE(map.erase(test2) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 0-5 keys") {
        for (size_t i = 0; i < 6; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 7th and 8th keys ") {
        REQUIRE(map.has(test8) == false);
        REQUIRE(map.has(test7) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [7]to[1000] keys") {
        for (size_t i = 7; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output method, items method returns expected 7 sequence of data") {
        for (size_t i = 0; i < 7; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
  }
}

SCENARIO("The method erase in OrderedMap works corretly") {
  GIVEN(
      "Partially filled OrderedMap with capacity 12 and has "
      "{0,0}{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8} items") {
    enum Test {
      test0 = 0,
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

    PF::Util::Containers::OrderedMap<Test, uint32_t, 12> map;
    for (size_t i = 0; i < 9; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The method erase is called on 0,1,2 and 3rd keys") {
      for (size_t i = 0; i < 4; i++) {
        auto erase_status = map.erase(Test(i));
        THEN("The erase method returns ok status for 0,1,2 and 3 keys") {
          REQUIRE(erase_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 5 ") { REQUIRE(map.size() == 5); }
      THEN("The avaliable method reports that 7 items are avaliable") {
        REQUIRE(map.available() == 7);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 4,5,6,7 and 8 keys") {
        for (size_t i = 4; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [10]to[1000] keys") {
        for (size_t i = 10; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output methos, items method returns expected 5 sequnce of data") {
        auto result = map.items()[0].second;
        REQUIRE(result == 4);
        auto result1 = map.items()[1].second;
        REQUIRE(result1 == 5);
        auto result2 = map.items()[2].second;
        REQUIRE(result2 == 6);
        auto result3 = map.items()[3].second;
        REQUIRE(result3 == 7);
        auto result4 = map.items()[4].second;
        REQUIRE(result4 == 8);
      }
    }
    WHEN("The erase method is called on 11th(capacity) and 12th(capacity+1) items") {
      auto erase_status = map.erase(test11);
      auto erase_status1 = map.erase(test12);
      THEN(
          "The erase method returns out of bounds status for 11th(capacity) and 12th(capacity+1) "
          "items") {
        REQUIRE(erase_status == PF::IndexStatus::out_of_bounds);
        REQUIRE(erase_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 9 ") { REQUIRE(map.size() == 9); }
      THEN("The avaliable method reports that 3 items are avaliable") {
        REQUIRE(map.available() == 3);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 0-8 keys") {
        for (size_t i = 0; i < 9; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [9]to[1000] keys") {
        for (size_t i = 9; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output methos, items method returns expected 9 sequnce of data") {
        for (size_t i = 0; i < 9; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
    WHEN("The erase method is called on 7,8 and 10th item") {
      auto erase_status = map.erase(test7);
      auto erase_status1 = map.erase(test8);
      auto erase_status2 = map.erase(test10);
      THEN("The erase method returns ok status for 7th item") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns ok status for 8th item") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds status for 10th item") {
        REQUIRE(erase_status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("After the input, the size method reports size as 7") { REQUIRE(map.size() == 7); }
      THEN("The avaliable method reports that 5 item are avaliable") {
        REQUIRE(map.available() == 5);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is non-empty") {
        REQUIRE(map.empty() == false);
      }
      THEN("The has method returns true for 0-4 keys ") {
        for (size_t i = 0; i < 5; i++) {
          REQUIRE(map.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 7-9 keys") {
        for (size_t i = 7; i < 10; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [11]to[1000] keys") {
        for (size_t i = 11; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("After the output method, items method return expected 7 sequnce of data") {
        for (size_t i = 0; i < 7; i++) {
          auto result = map.items()[i].second;
          REQUIRE(result == i);
        }
      }
    }
  }
}

SCENARIO("The method clear in OrderedMap works corretly") {
  GIVEN(
      "An partially filled OrderedMap with capacity 12 "
      "{1,1}{2,2}{3,3}{4,4}{5,5}{6,6}{7,7}{8,8}{9,9}{10,10}") {
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

    PF::Util::Containers::OrderedMap<Test, uint32_t, 12> map;
    for (size_t i = 1; i < 11; i++) {
      auto in_status = map.input(Test(i), i);
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The clear method is called") {
      map.clear();
      THEN("The has method returns false for 10 keys") {
        for (size_t i = 0; i < 11; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(map.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, OrderedMap has capacity 12") {
        REQUIRE(map.max_size() == 12);
      }
      THEN("The avaliable method reports that 12 items are avaliable") {
        REQUIRE(map.available() == 12);
      }
      THEN("The full method reports that the OrderedMap is not full") {
        REQUIRE(map.full() == false);
      }
      THEN("The empty method reports that the OrderedMap is empty") {
        REQUIRE(map.empty() == true);
      }
    }
  }
}