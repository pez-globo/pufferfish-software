/*
 * EnumSet.h
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 *  A statically-allocated set for a finite, pre-determined set of keys.
 *  Backed by an array of pairs. Methods use early returns of status codes instead
 *  of exceptions for error handling, for bounds-checking.
 *  This Set is designed for frequent lookups but doesn't support traversal.
 */

#include "Pufferfish/Util/Containers/EnumSet.h"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

#include "Pufferfish/Statuses.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
SCENARIO("The EnumSet constructed from  initializer list works corretly") {
  GIVEN("A EnumSet constructed with an initializer list with keys 1, 2 and 3") {
    enum Test {
      test1,
      test2,
      test3,
      test4,
      test5,
    };
    // using test = enum Test;
    PF::Util::Containers::EnumSet<Test, 11> set{test1, test2, test3};
    WHEN("The size method is called on EnumSet") {
      auto size = set.size();
      THEN("The size method returns size as 3") { REQUIRE(size == 3); }
    }
    WHEN("The avaliable method is called ") {
      auto avaliable = set.available();
      THEN("The avaliable method reports that 8 keys are avlaible ") { REQUIRE(avaliable == 8); }
    }
    WHEN("The full method is called") {
      bool full_status = set.full();
      THEN("The full method reports that the EnumSet is not full") {
        REQUIRE(full_status == false);
      }
    }
    WHEN("The empty method is called") {
      bool empty = set.empty();
      THEN("The empty method reports that the EnumSet is non-empty") { REQUIRE(empty == false); }
    }
    WHEN("The has method is called") {
      for (size_t i = 1; i < 5; i++) {
        bool status = set.has(Test(i));
        THEN("The has method returns true for 1-4 keys") { REQUIRE(status == true); }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [6]to[1000] keys") {
        for (size_t i = 6; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The input method is called on EnumSet") {
      auto status = set.input(test4);
      THEN("The input method returns ok status for 4th key") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, Enumset has capacity 11") {
        REQUIRE(set.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 4") {
        REQUIRE(set.size() == 4);
      }
      THEN("The avaliable method reports that 7 key is avaliable") {
        REQUIRE(set.available() == 7);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(set.erase(test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-4 keys") {
        for (size_t i = 1; i < 4; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
        REQUIRE(set.has(test4) == true);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [4]to[1000] keys") {
        for (size_t i = 4; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The erase method is called on 3rd and 4th key") {
      auto erase_status = set.erase(test3);
      auto erase_status1 = set.erase(test4);
      THEN("The erase method returns ok status for 3rd key") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds status for 4th key") {
        REQUIRE(erase_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, Enumset has 11 capacity") {
        REQUIRE(set.max_size() == 11);
      }
      THEN("After the erase method, the size method reports size as 2") {
        REQUIRE(set.size() == 2);
      }
      THEN("The avaliable method reports that 9 keys are avaliable") {
        REQUIRE(set.available() == 9);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for first 2 keys") {
        REQUIRE(set.has(test1) == true);
        REQUIRE(set.has(test2) == true);
      }
      THEN("The has method returns false for 3rd key") { REQUIRE(set.has(test3) == false); }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [6]to[1000] keys") {
        for (size_t i = 3; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The clear method is called") {
      set.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 11; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }

      THEN("The max_size method reports, Enumset has 11 capacity") {
        REQUIRE(set.max_size() == 11);
      }
      THEN("The avaliable method reports that 11 keys are avaliable") {
        REQUIRE(set.available() == 11);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is empty") { REQUIRE(set.empty() == true); }
    }
  }
}

SCENARIO("The input method in Enumset works correctly") {
  GIVEN(" An empty EnumSet with capacity 12") {
    enum Test {
      test1,
      test2,
      test3,
      test4,
      test5,
      test6,
      test7,
      test8,
      test9,
      test10,
      test11,
      test12,
      test13,
      test14,
    };
    //    using test = enum Test;
    PF::Util::Containers::EnumSet<Test, 12> set;
    WHEN("The input method is called on given Enumset for 1-11 keys") {
      for (size_t i = 1; i < 12; i++) {
        auto in_status = set.input(Test(i));
        REQUIRE(in_status == PF::IndexStatus::ok);

        THEN("The input method returns ok status for 1-11 keys") {
          REQUIRE(in_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, Enumset has capacity 12") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the input method, the size method reports size as 11 ") {
        REQUIRE(set.size() == 11);
      }
      THEN("The avaliable method reports that 1 key is avaliable") {
        REQUIRE(set.available() == 1);
      }
      THEN("The full method reports that the Enumset is not full") { REQUIRE(set.full() == false); }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The erase method reports ok status for 11th(capacity-1) key ") {
        REQUIRE(set.erase(test11) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 13 and 14 keys ") {
        REQUIRE(set.has(test13) == false);
        REQUIRE(set.has(test14) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
  }
  GIVEN(" An partially filled  EnumSet with capacity 12, which has 1-11 keys") {
    enum Test {
      test1,
      test2,
      test3,
      test4,
      test5,
      test6,
      test7,
      test8,
      test9,
      test10,
      test11,
      test12,
      test13,
      test14,
    };
    // using test = enum Test;
    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The input method is used on 13th(capacity+1) and 14th(capacity+2) Key") {
      for (size_t i = 1; i < 12; i++) {
        auto in_status = set.input(Test(i));
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      auto in_status = set.input(test13);
      auto in_status1 = set.input(test14);
      THEN("The input method returns out of bounds status for 13th(capacity+1) key") {
        REQUIRE(in_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The input method returns out of bounds status for 14th(capacity+2) key") {
        REQUIRE(in_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, Enumset has capacity 12") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the input method, the size method reports size as 11 ") {
        REQUIRE(set.size() == 11);
      }
      THEN("The avaliable method reports that 1 key are avaliable") {
        REQUIRE(set.available() == 1);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The erase method reports ok status for 11th(capacity-1) key ") {
        REQUIRE(set.erase(test11) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 13th(capacity+1) and 14th(capacity+2) key") {
        REQUIRE(set.has(test13) == false);
        REQUIRE(set.has(test14) == false);
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The input method is called on erased 7th and 8th key ") {
      auto erase_status = set.erase(test7);
      auto erase_status1 = set.erase(test8);
      auto in_status = set.input(test7);
      auto in_status1 = set.input(test8);
      THEN("The erase method returns ok status for 7th key") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 7th key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns ok status for 8th key") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 8th key") {
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the input method, the size method reports size as 11 ") {
        REQUIRE(set.size() == 11);
      }
      THEN("The avaliable method reports that 1 key is avaliable") {
        REQUIRE(set.available() == 1);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The input method is called on already existing 10th and 11th key in EnumSet") {
      auto in_status = set.input(test10);
      auto in_status1 = set.input(test11);
      THEN("The input method returns ok status for 10th and 11th key") {
        REQUIRE(in_status == PF::IndexStatus::ok);
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the input method, the size method reports size as 11 ") {
        REQUIRE(set.size() == 11);
      }
      THEN("The avaliable method reports that 1 key is avaliable") {
        REQUIRE(set.available() == 1);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
  }
}

SCENARIO("The method erase in Enumset works corretly") {
  GIVEN("EnumSet with capacity 12, and it has 1-11 keys") {
    enum Test {
      test1,
      test2,
      test3,
      test4,
      test5,
      test6,
      test7,
      test8,
      test9,
      test10,
      test11,
      test12,
      test13,
      test14,
    };
    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The method erase is called on 1,2,3 and 4 keys ") {
      for (size_t i = 1; i < 5; i++) {
        auto erase_status = set.erase(Test(i));
        THEN("The erase method returns ok status for 1,2,3 and 4 keys") {
          REQUIRE(erase_status == PF::IndexStatus::ok);
        }
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the erase method, the size method reports size as 7 ") {
        REQUIRE(set.size() == 7);
      }
      THEN("The avaliable method reports that 5 key are avaliable") {
        REQUIRE(set.available() == 5);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for 5-11 keys") {
        for (size_t i = 5; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The erase method is called on 13th(capacity+1) and 14th(capacity+2) key") {
      auto erase_status = set.erase(test13);
      auto erase_status1 = set.erase(test14);
      THEN("The erase method returns out of bounds status for 13th(capacity+1) key") {
        REQUIRE(erase_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method returns out of bounds status for 14th(capacity+2) key") {
        REQUIRE(erase_status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the erase method, the size method reports size as 11 ") {
        REQUIRE(set.size() == 11);
      }
      THEN("The avaliable method reports that 1 key is avaliable") {
        REQUIRE(set.available() == 1);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [12]to[1000] keys") {
        for (size_t i = 12; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
    WHEN("The erase method is called on 11(capacity-1),12(capacity) and 13th(capacity+1) key") {
      auto erase_status = set.erase(test11);
      auto erase_status1 = set.erase(test12);
      auto erase_status2 = set.erase(test13);
      THEN("The erase method returns ok status for 11(capacity-1) key") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns ok status for 12th(capacity)key") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds status for 13th(capacity+1) key") {
        REQUIRE(erase_status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The max_size method reports, Enumset has capacity 12") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("After the erase method, the size method reports size as 9 ") {
        REQUIRE(set.size() == 9);
      }
      THEN("The avaliable method reports that 3 keys are avaliable") {
        REQUIRE(set.available() == 3);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The has method returns true for 9 keys") {
        for (size_t i = 1; i < 10; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for [-1000]to[0] keys") {
        for (size_t i = -1000; i < 1; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [10]to[1000] keys") {
        for (size_t i = 10; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
    }
  }
}

SCENARIO("The method clear in Enumset works corretly") {
  GIVEN("Enumset with capacity 12, and it has 1-11 keys") {
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
    // using test = enum Test;

    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The clear method is used on Enumset") {
      set.clear();
      THEN("The has method returns false for 1-11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The has method returns false for [-1000]to[1000] keys") {
        for (size_t i = -1000; i < 1000; i++) {
          REQUIRE(set.has(Test(i)) == false);
        }
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
        REQUIRE(set.max_size() == 12);
      }
      THEN("The avaliable method reports that 12 keys are avaliable") {
        REQUIRE(set.available() == 12);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is empty") { REQUIRE(set.empty() == true); }
    }
  }
}