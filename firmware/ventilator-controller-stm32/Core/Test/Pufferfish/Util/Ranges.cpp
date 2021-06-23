/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * COBS.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Ranges
 *
 */
#include "Pufferfish/Util/Ranges.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The Util, ranges within function gives the expected result"){
  GIVEN("The Within function"){
// value < floor = ceiling    
    WHEN(
        "The function within is called with a floor of 10, a ceiling of 10, and a test value of "
        "8") {
      uint8_t input_val = 8;
      uint8_t min_value = 10;
      uint8_t max_value = 10;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Within method returns true, test value  in range") { REQUIRE(within_range == false); }
    }
// ceiling > floor = value    
    WHEN("The function within is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t input_val = 8;
      uint8_t min_val = 8;
      uint8_t max_val = 10;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns true, test value in range") { REQUIRE(within == true); }
    }
// floor < value < ceiling    
    WHEN("the function within is called with a floor of 5, a ceiling of 255, and a test value of "
      "8") {
      uint8_t input_val = 8;
      uint8_t min_val = 5;
      uint8_t max_val = 255;
      THEN("Within method returns true, test value in range") {
        auto range = PF::Util::within(input_val, min_val, max_val);
        REQUIRE(range == true);
      }
    }
// floor > value = ceiling
    WHEN("The function within is called with floor 15, ceiling if 8, and test value of 8") {
      uint8_t input_val = 8;
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within == false); }
    }
// floor < ceiling < value    
    WHEN("The function within is called with floor 12, ceiling 120, and test value 200") {
      uint8_t min_val = 12;
      uint8_t max_val = 120;
      uint8_t input_val = 200;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within == false); }
    }
  }
  
}

SCENARIO("The util within function works correctly for floor = ceiling "){
  GIVEN("The within function"){
// value < floor = ceiling    
    WHEN("The function within is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t input_val = 20;
      uint8_t min_val = 120;
      uint8_t max_val = 120;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within == false); }
    }
//floor = value = ceiling    
    WHEN(
      "The function within is called with a floor of 20, a ceiling of 20, and a test value of 20") {
      uint8_t input_val = 20;
      uint8_t min_value = 20;
      uint8_t max_value = 20;
      auto within_range = PF::Util::within(input_val, min_value, max_value);
      THEN("Within method returns true, test value in range") { REQUIRE(within_range == true); }
    }
 //value > floor = ceiling   
    WHEN("The function within is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t input_val = 20;
      uint8_t min_val = 10;
      uint8_t max_val = 10;
      auto within_range = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within_range == false); }
    }
  }
}


SCENARIO("Within function works correctly ") {
  GIVEN("The within function"){
//floor > ceiling > value    
    WHEN(
      "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of "
      "4") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;      
      uint8_t input_val = 4;
      auto within_range = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within_range == false); }
    }
// floor > ceiling = value    
    WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 8 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      uint8_t input_val = 8;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within == false); }
    }
 // ceiling < floor = value   
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;      
      uint8_t input_val = 15;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(within == false); }
    }


//value > floor > ceiling  
     WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;

      uint8_t input_val = 30;
      auto within = PF::Util::within(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(within == false); }
    }
   
  }
}
SCENARIO("Util clamp function gives expected results "){
  GIVEN("The clamp and within function ") {
//value< floor = ceiling    
    WHEN(
        "The function clamp is called with a floor of 10, a ceiling of 10, and a test value of "
        "8") {
      uint8_t input_val = 8;    
      uint8_t min_value = 10;
      uint8_t max_value = 10;
      THEN("The clamp method returns 10") {
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        REQUIRE(range == 10);
      }
    }
// ceiling > floor = value 
    WHEN("The function clamp is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t input_val = 8;
      uint8_t min_val = 8;
      uint8_t max_val = 10;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }

    }
//floor < value < ceiling
    WHEN(
      "the function clamp is called with a floor of 5, a ceiling of 255, and a test value of "
      "8") {
      uint8_t input_val = 8;
      uint8_t min_val = 5;
      uint8_t max_val = 255;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }
    }

//ceiling < floor = value
    WHEN("The function clamp is called with floor 15, ceiling of 8, and test value of 15") {
      uint8_t input_val = 15;
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }
    }

// floor > ceiling > value
    WHEN("The function clamp is called with floor 12, ceiling 120, and test value 200") {
      uint8_t min_val = 12;
      uint8_t max_val = 120;
      uint8_t input_val = 200;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 120") { REQUIRE(range == 120); }
    }
  }  
}

SCENARIO("Clamp  works correctly for floor == ceiling"){
  GIVEN("Clamp and within function"){
// value < floor = ceiling    
    WHEN("The function clamp is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t input_val = 20;
      uint8_t min_val = 120;
      uint8_t max_val = 120;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 120") { REQUIRE(range == 120); }
    }
//floor = value = ceiling 
    WHEN(
        "The function clamp is called with a floor of 20, a ceiling of 20, and a test value of "
        "20") {
        uint8_t input_val = 20;
        uint8_t min_value = 20;
        uint8_t max_value = 20;
        auto range = PF::Util::clamp(input_val, min_value, max_value);
        THEN("The clamp method returns 20") { REQUIRE(range == 20); }
        }
// ceiling > floor = value    
    WHEN(
        "The function clamp is called with a floor of 8, a ceiling of 15, and a test value of 8") {
      uint8_t input_val = 15;
      uint8_t min_val = 8;
      uint8_t max_val = 15;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 10") { REQUIRE(range == 15); }
    }
  //value > floor = ceiling   
    WHEN("The function within is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t input_val = 20;
      uint8_t min_val = 10;
      uint8_t max_val = 10;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("Within method returns false, test value not in range") { REQUIRE(range == 10); }
    }
   
  }
}

SCENARIO("Clamp  function works correctly for floor > ceiling"){
  GIVEN("Clamp and within function is given"){
//floor > ceiling > value    
    WHEN(
      "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of "
      "4") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      uint8_t input_val = 4;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8 ") { REQUIRE(range == 8); }
      }

// floor > ceiling = value
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      uint8_t input_val = 15;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }
    }
 // ceiling < floor = value  
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;      
      uint8_t input_val = 15;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }
    }
  //  value > floor > ceiling 
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t min_val = 15;
      uint8_t max_val = 8;
      uint8_t input_val = 30;
      auto range = PF::Util::clamp(input_val, min_val, max_val);
      THEN("The clamp method returns 8") { REQUIRE(range == 8); }
    }
   
  }
}

