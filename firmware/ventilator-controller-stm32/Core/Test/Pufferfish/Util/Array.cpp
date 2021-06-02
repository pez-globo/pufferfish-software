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
#include "Pufferfish/Util/Array.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("Create an array", "[array]") {
    GIVEN("A set of int values") {
        WHEN("An array of given int size is created properly") {
            auto data = PF::Util::make_array<uint8_t>(0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02);
            THEN("Value at every index in array is as expected") { 
                uint8_t expected[8] = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};
              for(int i = 0;i<8;i++) {
                  REQUIRE(data[i] == expected[i]);
              }     
            }
            THEN("Size of an array is as expected"){
                REQUIRE(data.max_size() == 8);
            }
            
        }
    }

    
    GIVEN("A set of string") {
        auto data = PF::Util::make_array<std::string>("ab", "bc", "cd");
        WHEN("An array is created with given set of string correctly") {        
            THEN("Value at every index in array is as expected") {
                REQUIRE(data[0] == "ab");
                REQUIRE(data[1] == "bc");
                REQUIRE(data[2] == "cd");
            }
            THEN("Size of an array is as expected"){
                REQUIRE(data.max_size() == 3);
            }
            
        }
    }


    GIVEN("Two variables ") {
        uint8_t expected = '3'; 
        uint8_t array = '6'; 
        WHEN("Make an array with const references"){
           uint8_t &ri = expected;
           uint8_t &pi = array;
           auto data = PF::Util::make_array<uint8_t>(ri,pi);
           THEN("value at index 0 and 1 is as expected"){
               REQUIRE(data[0]== '3' );
               REQUIRE(data[1]== '6');
           }
        }

    }

}
   
SCENARIO("Empty an array") {

    GIVEN("An array is given"){
        auto data = PF::Util::make_array<uint8_t>(0x02, 0x03, 0x0a);
        WHEN("Given array is emptied"){
            THEN("The results are as expected"){
                auto empty = data.empty();
                REQUIRE(empty == 0);
            }
        }
    }

}



 


