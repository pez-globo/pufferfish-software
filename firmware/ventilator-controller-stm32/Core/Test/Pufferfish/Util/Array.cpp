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
            THEN("Value at index 5 in array is as expected") {
                REQUIRE(data[5] == 0x01);
            THEN("Size of an array is as expected"){
                REQUIRE(data.max_size() == 8);
            }
            }
        }
    }
     GIVEN("set of values"){
        WHEN("An array of 4UL size with   given values is created properly") {
            auto data = PF::Util::make_array<uint8_t>(0x01, 0x81, 0x01, 0x00);
            THEN("Value at index 3 in array is as expected") {
                REQUIRE(data[3] == 0x00);
            THEN("Size of array is as expected"){
                REQUIRE(data.max_size() == 4);
            }
            }
        }
       

    }
    
    GIVEN("A set of string") {
        auto data = PF::Util::make_array<std::string>("ab", "bc", "cd");

        WHEN("An array is created with given set of string correctly") {        
            THEN("Value at 0 index in array is as expected") {
                REQUIRE(data[0] == "ab");
            THEN("Size of an array is as expected"){
                REQUIRE(data.max_size() == 3);
            }
            }
        }
    }

}
   
   SCENARIO("Create an array and check data and size of array") {

    GIVEN("An array ") {
        auto data = PF::Util::make_array<uint8_t>(0x01, 0x08, 0x0f , 0x0a);
        WHEN("Array is created with given value correctly  "){
         THEN("Value at 2 index in array is as expected"){
           REQUIRE(data[2] == 0x0f);
         THEN("Size of an array is as expected"){
           REQUIRE(data.max_size() == 4);   
        }
         }
        }
        
    }
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


