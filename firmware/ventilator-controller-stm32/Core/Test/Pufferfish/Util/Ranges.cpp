/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * COBS.cpp
 *
 *  Created on: 
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of COBS
 *
 */
#include "Pufferfish/Util/Ranges.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("Value is in provided range"){
    GIVEN("A value"){
        u_int8_t input_val = 0x83;
     WHEN("Given Value is in range"){
         THEN("values are as expected"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0xff;
            auto range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(range == true);
         }
     }   
    }
}


SCENARIO("Value is clamped"){
    GIVEN("A value"){
        u_int8_t input_val = 0x83;
     WHEN("Given Value is in range"){
         THEN("Returns correct given value"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0xff;
            auto range = PF::Util::clamp(input_val,min_val ,max_val);
             REQUIRE(range == 0x83);
         }
     } 
     WHEN("Value is not in range"){
         THEN("Returns max_val, value is greater than max_value"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0x0a;
            auto range = PF::Util::clamp(input_val,min_val ,max_val);
             REQUIRE(range == 0x0a);
         }
     }

    }
}