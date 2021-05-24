/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Parse.cpp
 *
 *  Created on: 24 May
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of Bytes
 *
 */
#include <cstddef>
#include "Pufferfish/Util/Bytes.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("Get the byte from expected position", "[bytes]") {

    GIVEN("A Byte"){
        const uint8_t number = 0xff;

        WHEN("a byte is fetched from 0th index ") {
            int value = PF::Util::get_byte<0>(number);
            THEN("value is as expected") {
                REQUIRE(value == 255);
            }
        }

    }   
}

SCENARIO("Set byte ") {
    GIVEN("A Byte"){
        const uint8_t number = 0x05;
        WHEN("a byte is set at 0") {
            auto value = PF::Util::set_byte<0, uint16_t>(number);
            THEN("Value is as expected") {
                REQUIRE(value == 5);
            }
        }

        WHEN("A byte is set at 1"){
            auto value = PF::Util::set_byte<1 , uint16_t>(number);
            THEN("Value is as expected") {
                REQUIRE(value == 1280);
            }
        }   
    }

    
} 