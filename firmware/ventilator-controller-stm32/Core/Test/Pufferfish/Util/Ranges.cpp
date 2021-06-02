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

SCENARIO("Value is in provided range"){
    GIVEN("A value"){
        u_int8_t input_val = 0x83;
     WHEN("Given Value is in range"){
         u_int8_t min_val = 0x01;
         u_int8_t max_val = 0xff;
        THEN("values are as expected"){
            auto range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(range == true);
         }
     }   
    }
}


SCENARIO("Value is clamped"){
    GIVEN("A value"){
        uint8_t input_val = 0x83;
     WHEN("Value is equal to ceiling"){
         THEN("Returns correct given value"){
             u_int8_t min_val = 0x01;
             u_int8_t max_val = 0xff;
            auto range = PF::Util::clamp(input_val,min_val ,max_val);
             REQUIRE(range == 0x83);
             THEN("Returns true, value is in range"){
            auto within_range = PF::Util::within(input_val,min_val ,max_val);
             REQUIRE(within_range == true);             
         }
         }
     } 
     WHEN("Value is greater than ceiling "){
         THEN("Returns max_val '\x0a', value is greater than max_value"){
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
    GIVEN("A value '\x08' "){
        uint8_t input_val = 0x08;
        WHEN("Value is less than floor"){
            uint8_t min_value =  0x0a;
            uint8_t max_value = 0x83;
            THEN("As expected it returns floor value"){
                auto range = PF::Util::clamp(input_val,min_value,max_value);
                  REQUIRE(range == 0x0a );

            }

        }
     
       WHEN("Value is equal to floor"){
       uint8_t min_value =  0x08;
       uint8_t max_value = 0x83;
       THEN("As expected it returns input value"){
           auto range = PF::Util::clamp(input_val,min_value,max_value);
            REQUIRE("range = 0x08");
       }
       }
    GIVEN("A value"){
           uint8_t input_val = 0x53;
        WHEN("Value is in the range"){
            uint8_t min_value =  0x08;
            uint8_t max_value = 0xff;
            auto range = PF::Util::clamp(input_val, min_value, max_value);
            THEN("As expected input value is returned as its in range "){
                REQUIRE(range == 0x53);
            }
        }

        WHEN("floor is equal to ceiling"){
            uint8_t min_value =  0x08;
            uint8_t max_value = 0x08;
            auto range = PF::Util::clamp(input_val,min_value,max_value);
            THEN("Result is as expected"){
                REQUIRE(range == 0x08 );
                }
            }

           
        }

        WHEN("floor is greater than ceiling"){
           uint8_t min_value =  0x0f;
           uint8_t max_value = 0x08;
           auto range = PF::Util::clamp(input_val,min_value,max_value);
           THEN("Result is as expected"){
               REQUIRE(range == 0x08);
            }

        }



    }
}




