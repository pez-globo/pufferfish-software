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
#include "Pufferfish/Util/Array.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("The Util get_byte function correctly gets bytes from the given position") {

    GIVEN("A Byte"){
        const uint8_t number = 0x86;

        WHEN("the get_byte function is called on 0x86 ") {
            int value = PF::Util::get_byte<0>(number);
            THEN("value is as expected") {
                REQUIRE(value == 134);
            }
        }

    }

    GIVEN("Byte")   {
        uint16_t number = 0x860a;
        WHEN("a byte is fetched from 0th index"){
          int value1 = PF::Util::get_byte<1>(number);
          int value = PF::Util::get_byte<0>(number);
          THEN("Value is as expected for 0th index 10")  {
              REQUIRE(value == 10);
              THEN("Value is as expected for 1st index 134");
              REQUIRE(value1 == 134);
          }

        }

    }
    GIVEN("Byte"){
       uint32_t number = 0x86010f0a;
       WHEN("a byte is fetched from 0-5 indexs"){
         int value = PF::Util::get_byte<0>(number);
         int value1 = PF::Util::get_byte<1>(number);
         int value2 = PF::Util::get_byte<2>(number);
         int value3 = PF::Util::get_byte<3>(number);
         int value4 = PF::Util::get_byte<5>(number);
         THEN("value is as expected at index 0") {
           REQUIRE(value == 10); 
           THEN("value is as expected at index 1") {
              REQUIRE(value1 == 15);
           THEN("value is as expected at index 2") {
              REQUIRE(value2 ==1 );
           THEN("value is as expected at index 3") {
               REQUIRE(value3 == 134) ;
           THEN("Index is out of bound, value is as expected at index 5 ") {
               REQUIRE("value4 == 0");
           }
           }
           }
           }
         }
         
       }

    }
}

SCENARIO("The Util set_byte function correctly sets bytes to the given position") {
    GIVEN("The Util set_byte function"){
        const uint8_t number = 0x86;
        WHEN("a byte is set at 0") {
            auto value = PF::Util::set_byte<0, uint8_t>(number);
            THEN("Value is as expected") {
                REQUIRE(value == 134);
            }
        }

        WHEN("A byte is set at 1"){
            auto value = PF::Util::set_byte<1 , uint8_t>(number);
            THEN("Value is as expected") {
                REQUIRE(value == 0);
            }
        }  
    }

    GIVEN("A byte"){
       const uint8_t number = 0x83;
    
        WHEN("the set_byte function is called on"){
            auto value = PF::Util::set_byte<0,uint16_t>(number);
            THEN("Value is as expected"){
                REQUIRE(value == 131);
            }
        }

        WHEN("the set_byte function is called on 0x83"){
            auto value = PF::Util::set_byte<1,uint16_t>(number);
            THEN("value is as expected"){
                REQUIRE(value == 33536);
            }

        }

        WHEN("the set_byte function is called on 0x83"){
            auto value = PF::Util::set_byte<2,uint32_t>(number);
            THEN("Result is as expected"){
                REQUIRE(value == 8585216);
            }
        }

        WHEN("the set_byte function is called on 0x83"){
            auto value = PF::Util::set_byte<3,uint32_t>(number);
            THEN("Result is as expected"){
                REQUIRE(value == 2197815296);
            }
        }
    }

    GIVEN("A byte"){
         const uint16_t byte = 0x860a;
         WHEN("the set_byte function is called on 0x860a for index 0"){
            auto value = PF::Util::set_byte<uint16_t>(byte, 0);
            THEN("Result is as expected"){
                REQUIRE(value == 10 );
            }
            
        }

        WHEN("the set_byte function is called on 0x860a for index 1"){
            auto value = PF::Util::set_byte<uint16_t>(byte, 1);
            THEN("Result is as expected"){
                REQUIRE(value == 2560);
            }
        }

        WHEN("the set_byte function is called on 0x860a for index 2"){
            auto value = PF::Util::set_byte<uint16_t>(byte, 2);
            THEN("Result is as expected"){
                REQUIRE(value == 0);
            }
        }

        WHEN("the set_byte function is called on 0x860a for index 2"){
            auto setbyte = PF::Util::set_byte<uint32_t>(byte,2);
            THEN("Result is as expected"){
                REQUIRE(setbyte == 655360);
            }

        }

        WHEN("the set_byte function is called on 0x860a for index 3"){
            auto setbyte = PF::Util::set_byte<uint32_t>(byte ,3);
            THEN("Result is as expected"){
                REQUIRE(setbyte == 167772160);
            }
        }

    }
        
} 
    
    

    
