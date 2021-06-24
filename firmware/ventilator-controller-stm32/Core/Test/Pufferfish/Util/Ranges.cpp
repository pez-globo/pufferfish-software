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

SCENARIO("The Util::Ranges within function gives the expected result"){
  GIVEN("The Util::Ranges Within function"){
// value < floor = ceiling    
    WHEN(
        "The function within is called with a floor of 10, a ceiling of 10, and a test value of "
        "8") {
      uint8_t value = 8;
      uint8_t floorue = 10;
      uint8_t ceilingue = 10;
      auto within_range = PF::Util::within(value, floorue, ceilingue);
      THEN("Within method returns true") { REQUIRE(within_range == false); }
    }
// ceiling > floor = value    
    WHEN("The function within is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t value = 8;
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns true") { REQUIRE(within == true); }
    }
// floor < value < ceiling    
    WHEN("the function within is called with a floor of 5, a ceiling of 255, and a test value of "
      "8") {
      uint8_t value = 8;
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      THEN("Within method returns true") {
        auto range = PF::Util::within(value, floor, ceiling);
        REQUIRE(range == true);
      }
    }
// floor > value = ceiling
    WHEN("The function within is called with floor 15, ceiling if 8, and test value of 8") {
      uint8_t value = 8;
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within == false); }
    }
// floor < ceiling < value    
    WHEN("The function within is called with floor 12, ceiling 120, and test value 200") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within == false); }
    }
  }
  
}

SCENARIO("The Util::Ranges within function gives the expected result for floor = ceiling "){
  GIVEN("The Util::Ranges Within function"){
// value < floor = ceiling    
    WHEN("The function within is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t value = 20;
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within == false); }
    }
//floor = value = ceiling    
    WHEN(
      "The function within is called with a floor of 20, a ceiling of 20, and a test value of 20") {
      uint8_t value = 20;
      uint8_t floorue = 20;
      uint8_t ceilingue = 20;
      auto within_range = PF::Util::within(value, floorue, ceilingue);
      THEN("Within method returns true") { REQUIRE(within_range == true); }
    }
 //value > floor = ceiling   
    WHEN("The function within is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t value = 20;
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      auto within_range = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within_range == false); }
    }
  }
}


SCENARIO("The Util::Ranges within function gives the expected result for floor > ceiling") {
  GIVEN("The Util::Ranges Within function"){
//floor > ceiling > value    
    WHEN(
      "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of "
      "4") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;      
      uint8_t value = 4;
      auto within_range = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within_range == false); }
    }
// floor > ceiling = value    
    WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 8 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 8;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("Within method returns false") { REQUIRE(within == false); }
    }
 // ceiling < floor = value   
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;      
      uint8_t value = 15;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(within == false); }
    }


//value > floor > ceiling  
     WHEN("The function within is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;

      uint8_t value = 30;
      auto within = PF::Util::within(value, floor, ceiling);
      THEN("The Within method returns false") { REQUIRE(within == false); }
    }
   
  }
}
SCENARIO("The Util::Ranges clamp function gives the expected result "){
  GIVEN("The Util::Ranges clamp function ") {
//value< floor = ceiling    
    WHEN(
        "The function clamp is called with a floor of 10, a ceiling of 10, and a test value of "
        "8") {
      uint8_t value = 8;    
      uint8_t floorue = 10;
      uint8_t ceilingue = 10;
      THEN("The clamp function returns the floor (10)") {
        auto range = PF::Util::clamp(value, floorue, ceilingue);
        REQUIRE(range == 10);
      }
    }
// ceiling > floor = value 
    WHEN("The function clamp is called with floor of 8, ceiling of 10, and test value of 8 ") {
      uint8_t value = 8;
      uint8_t floor = 8;
      uint8_t ceiling = 10;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }

    }
//floor < value < ceiling
    WHEN(
      "the function clamp is called with a floor of 5, a ceiling of 255, and a test value of "
      "8") {
      uint8_t value = 8;
      uint8_t floor = 5;
      uint8_t ceiling = 255;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the value (8)") { REQUIRE(range == 8); }
    }

//ceiling < floor = value
    WHEN("The function clamp is called with floor 15, ceiling of 8, and test value of 15") {
      uint8_t value = 15;
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
    }

// floor > ceiling > value
    WHEN("The function clamp is called with floor 12, ceiling 120, and test value 200") {
      uint8_t floor = 12;
      uint8_t ceiling = 120;
      uint8_t value = 200;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the ceiling (120)") { REQUIRE(range == 120); }
    }
  }  
}

SCENARIO("The Util::Ranges clamp function gives the expected result for floor == ceiling"){
  GIVEN("The Util::Ranges clamp function"){
// value < floor = ceiling    
    WHEN("The function clamp is called with floor 120, ceiling 120, test value as 20 ") {
      uint8_t value = 20;
      uint8_t floor = 120;
      uint8_t ceiling = 120;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (120)") { REQUIRE(range == 120); }
    }
//floor = value = ceiling 
    WHEN(
        "The function clamp is called with a floor of 20, a ceiling of 20, and a test value of "
        "20") {
        uint8_t value = 20;
        uint8_t floorue = 20;
        uint8_t ceilingue = 20;
        auto range = PF::Util::clamp(value, floorue, ceilingue);
        THEN("The clamp function returns the floor (20)") { REQUIRE(range == 20); }
        }
// ceiling > floor = value    
    WHEN(
        "The function clamp is called with a floor of 8, a ceiling of 15, and a test value of 8") {
      uint8_t value = 8;
      uint8_t floor = 8;
      uint8_t ceiling = 15;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
    }
  //value > floor = ceiling   
    WHEN("The function clamp is called with a floor of 10, ceiling of 10, and a test value of 20") {
      uint8_t value = 20;
      uint8_t floor = 10;
      uint8_t ceiling = 10;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (10)") { REQUIRE(range == 10); }
    }
   
  }
}

SCENARIO("The Util::Ranges clamp function gives the expected result for floor > ceiling"){
  GIVEN("The Util::Ranges clamp function"){
//floor > ceiling > value    
    WHEN(
      "The function clamp is called with a floor of 15, a ceiling of 8, and a test value of "
      "4") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 4;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
      }

// floor > ceiling = value
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 15;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
    }
 // ceiling < floor = value  
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 15 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;      
      uint8_t value = 15;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
    }
  //  value > floor > ceiling 
    WHEN("The function clamp is called with floor of 15, ceiling of 8, and test value of 30 ") {
      uint8_t floor = 15;
      uint8_t ceiling = 8;
      uint8_t value = 30;
      auto range = PF::Util::clamp(value, floor, ceiling);
      THEN("The clamp function returns the floor (8)") { REQUIRE(range == 8); }
    }
   
  }
}

