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

SCENARIO("The  Ranges  within function works correctly"){
    GIVEN("The within function"){
        u_int8_t input_val = 0x83;
     WHEN("floor is '0x01 and ceiling is 0xff , value 0x83 is within range"){
         u_int8_t min_val = 0x01;
         u_int8_t max_val = 0xff;
        THEN("Returns true ,  value 0x83 is in range"){
            auto range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(range == true);
         }
     }   
    }
}


SCENARIO("The Ranges  clamp function works correctly"){
    GIVEN("The clamp function"){
        uint8_t input_val = 0x83;
     WHEN("the function is called with a floor of 0x01, a ceiling of 0xff, and a test value of 0x83"){
         THEN("Returns correct value 0x83"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0xff;
            auto range = PF::Util::clamp(input_val,min_val ,max_val);
             REQUIRE(range == 0x83);
             THEN("Returns true, value is in range"){
             auto within_range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(within_range == true);
             REQUIRE(range == 0x83);             
         }
         }
     } 
     WHEN("The function is called with a floor of 0x01, a ceiling of 0x0a, and a test value of 0x83 "){
         THEN("Returns expected value 0x0a"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0x0a;
            auto range = PF::Util::clamp(input_val,min_val ,max_val);
             REQUIRE(range == 0x0a);
             THEN("Returns true, value is in range"){
            auto within_range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(within_range == false);
             }             

         }
     }
    }
    GIVEN("The clamp function "){
        uint8_t input_val = 0x08;
        WHEN("The function is called with a floor of 0x0a, a ceiling of 0x83, and a test value of 0x08"){
            uint8_t min_value =  0x0a;
            uint8_t max_value = 0x83;
            THEN("As expected it returns floor value 0x0a"){
                auto range = PF::Util::clamp(input_val,min_value,max_value);
                  REQUIRE(range == 0x0a );

            }

        }
     
       WHEN("The function is called with a floor of 0x08, a ceiling of 0x83, and a test value of 0x08'"){
       uint8_t min_value =  0x08;
       uint8_t max_value = 0x83;
       THEN("Returns expected  input value 0x08"){
           auto range = PF::Util::clamp(input_val,min_value,max_value);
            REQUIRE(range == 0x08);
       }
       }
    GIVEN("The clamp function"){
           uint8_t input_val = 0x53;
        WHEN("The function is called with a floor of 0x08, a ceiling of 0xff, and a test value of 0x53"){
            uint8_t min_value =  0x08;
            uint8_t max_value = 0xff;
            auto range = PF::Util::clamp(input_val, min_value, max_value);
            THEN("Returns expected  input value 0x53 "){
                REQUIRE(range == 0x53);
            }
        }

        WHEN("The function is called with a floor of 0x08, a ceiling of 0x08, and a test value of 0x08"){
            uint8_t min_value =  0x08;
            uint8_t max_value = 0x08;
            auto range = PF::Util::clamp(input_val,min_value,max_value);
            THEN("Returns expected value 0x08"){
                REQUIRE(range == 0x08 );
                }
            }

           
        }

        WHEN("The function is called with a floor of 0x0f, a ceiling of 0x08, and a test value of 0x53"){
           uint8_t min_value =  0x0f;
           uint8_t max_value = 0x08;
           auto range = PF::Util::clamp(input_val,min_value,max_value);
           THEN("Result is as expected 0x08"){
               REQUIRE(range == 0x08);
            }

        }



    }
}




