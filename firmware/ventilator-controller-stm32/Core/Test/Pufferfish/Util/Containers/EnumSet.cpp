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
SCENARIO("The method initializer list in Enumset works corretly") {
  GIVEN("An EnumSet constructed with capacity 11, is partially filled with 3 keys") {
    typedef enum _Test {
      Test1,
      Test2,
      Test3,
      Test4,
      Test5,
    } Test;
    PF::Util::Containers::EnumSet<Test, 11> set{Test1, Test2, Test3};
    WHEN("The input method is called on EnumSet") {
      auto status = set.input(Test4);
      THEN("The input method returns ok status for 4th key") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, Enumset has capacity 11") {
        REQUIRE(set.max_size() == 11);
      }
      THEN("After the input method, the size method reports size as 4") {
        REQUIRE(set.size() == 4);
      }
      THEN("The avaliable method reports that 4 key is avaliable") {
        REQUIRE(set.available() == 7);
      }
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The erase method reports ok status for 1st key ") {
        REQUIRE(set.erase(Test1) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 4 keys") {
        for (size_t i = 1; i < 4; i++) {
          std::cout << "set" << i << std::endl;
          REQUIRE(set.has(Test(i)) == true);
        }
        REQUIRE(set.has(Test4) == true);
      }
    }
    WHEN("The erase method is called on EnumSet") {
      auto erase_status = set.erase(Test3);
      auto erase_status1 = set.erase(Test4);
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
        REQUIRE(set.has(Test1) == true);
        REQUIRE(set.has(Test2) == true);
      }
      THEN("The has method returns false for 3rd key") { REQUIRE(set.has(Test3) == false); }
    }
    WHEN("The clear method is used on Enumset") {
      set.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 1; i < 11; i++) {
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
    typedef enum _Test {
      Test1,
      Test2,
      Test3,
      Test4,
      Test5,
      Test6,
      Test7,
      Test8,
      Test9,
      Test10,
      Test11,
      Test12,
      Test13,
      Test14,
    } Test;
    PF::Util::Containers::EnumSet<Test, 12> set;
    WHEN("The input method is called on given Enumset") {
      for (size_t i = 1; i < 12; i++) {
        auto in_status = set.input(Test(i));
        REQUIRE(in_status == PF::IndexStatus::ok);

        THEN("The input method returns ok status for 11 key") {
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
      THEN("The full method reports that the Enumset is not completely filled") {
        REQUIRE(set.full() == false);
      }
      THEN("The empty method reports that the Enumset is non-empty") {
        REQUIRE(set.empty() == false);
      }
      THEN("The erase method reports ok status for 11th key ") {
        REQUIRE(set.erase(Test11) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 13th and 14th keys ") {
        REQUIRE(set.has(Test13) == false);
        REQUIRE(set.has(Test14) == false);
      }
    }
  }
  GIVEN(" An empty EnumSet with capacity 12") {
    typedef enum _Test {
      Test1,
      Test2,
      Test3,
      Test4,
      Test5,
      Test6,
      Test7,
      Test8,
      Test9,
      Test10,
      Test11,
      Test12,
      Test13,
      Test14,
    } Test;
    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The input method is used on 13th and 14th Key") {
      for (size_t i = 1; i < 12; i++) {
        auto in_status = set.input(Test(i));
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      auto in_status = set.input(Test13);
      auto in_status1 = set.input(Test14);
      THEN("The input method returns out of bounds status for 13th key") {
        REQUIRE(in_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The input method returns out of bounds status for 14th key") {
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
      THEN("The erase method reports ok status for 11th key ") {
        REQUIRE(set.erase(Test11) == PF::IndexStatus::ok);
      }
      THEN("The has method returns true for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
      THEN("The has method returns false for 13th and 14th key") {
        REQUIRE(set.has(Test13) == false);
        REQUIRE(set.has(Test14) == false);
      }
    }
    WHEN("The input method is called on erased key ") {
      auto erase_status = set.erase(Test7);
      auto erase_status1 = set.erase(Test8);
      auto in_status = set.input(Test7);
      auto in_status1 = set.input(Test8);
      THEN("The input method returns ok status for 7th key") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
        REQUIRE(in_status == PF::IndexStatus::ok);
      }
      THEN("The input method returns ok status for 8th key") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
        REQUIRE(in_status1 == PF::IndexStatus::ok);
      }
      THEN("The max_size method reports, Enumset has 12 capacity") {
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
      THEN("The has method returns true for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
    }
    WHEN("The input method is called on already existing key in EnumSet") {
      auto in_status = set.input(Test10);
      auto in_status1 = set.input(Test11);
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
    }
  }
}

SCENARIO("The method erase in Enumset works corretly") {
  GIVEN("EnumSet with capacity 12, and it has 11 keys") {
    typedef enum _Test {
      Test1,
      Test2,
      Test3,
      Test4,
      Test5,
      Test6,
      Test7,
      Test8,
      Test9,
      Test10,
      Test11,
      Test12,
      Test13,
      Test14,
    } Test;

    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The method erase is called on given Enumset") {
      for (size_t i = 1; i < 5; i++) {
        auto erase_status = set.erase(Test(i));
        THEN("The erase method returns ok status for 5 keys") {
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
      THEN("The has method returns true for 6 keys") {
        for (size_t i = 5; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
    }
    WHEN("The erase method is called on 13th and 14th key") {
      auto erase_status = set.erase(Test13);
      auto erase_status1 = set.erase(Test14);
      THEN("The erase method returns out of bounds status for 13th key") {
        REQUIRE(erase_status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method returns out of bounds status for 14th key") {
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
      THEN("The has method returns true for 11 keys") {
        for (size_t i = 1; i < 12; i++) {
          REQUIRE(set.has(Test(i)) == true);
        }
      }
    }
    WHEN("The erase method is used thrice on given Enumset") {
      auto erase_status = set.erase(Test11);
      auto erase_status1 = set.erase(Test12);
      auto erase_status2 = set.erase(Test13);
      THEN("The first erase method returns ok status") {
        REQUIRE(erase_status == PF::IndexStatus::ok);
      }
      THEN("The second erase method returns ok status") {
        REQUIRE(erase_status1 == PF::IndexStatus::ok);
      }
      THEN("The third erase method returns out of bounds status") {
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
    }
  }
}

SCENARIO("The method clear in Enumset works corretly") {
  GIVEN("Enumset with capacity 12, and it has 11 keys") {
    typedef enum _Test {
      Test1 = 1,
      Test2 = 2,
      Test3 = 3,
      Test4 = 4,
      Test5 = 5,
      Test6 = 6,
      Test7 = 7,
      Test8 = 8,
      Test9 = 9,
      Test10 = 10,
      Test11 = 11,
      Test12 = 12,
    } Test;

    PF::Util::Containers::EnumSet<Test, 12> set;
    for (size_t i = 1; i < 12; i++) {
      auto in_status = set.input(Test(i));
      REQUIRE(in_status == PF::IndexStatus::ok);
    }

    WHEN("The clear method is used on Enumset") {
      set.clear();
      THEN("The has method returns false for 11 keys") {
        for (size_t i = 0; i < 13; i++) {
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