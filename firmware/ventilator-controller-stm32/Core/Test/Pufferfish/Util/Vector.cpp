/*
 * Vector.tpp
 * Test1 
 *  Created on: April 07, 2021
 *      Author: Raavi Lagoo
 *
 * */

#pragma once

#include <cstring>

#include "Pufferfish/Util/Vector.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;


//constexpr size_t buffer_size = 252UL;
SCENARIO( " Vectors are resized", "[vector]" ) {
  GIVEN("Vector array of 252UL size "){
  constexpr size_t buffer_size = 252UL;
  PF::Util::Vector<uint8_t,buffer_size> vector1;
      WHEN("Vector is resized to 256UL"){
        THEN("Vector is resized correctly to 256 "){
        auto re_vector = vector1.resize(256);
         REQUIRE("re_vector = PF::IndexStatus::ok");
        }
      }

      WHEN("Vector is resized to 400UL"){
        auto re_vector = vector1.resize(400);
        THEN("Result is as expected"){
          REQUIRE(re_vector == PF::IndexStatus::out_of_bounds);
        }
      }

      WHEN("Util function empty is used on vector"){
        auto empty = vector1.empty();
        THEN("Result is as expected"){
          REQUIRE(empty == true);
        }
      }

      WHEN("Util function avaliable is used on vector"){
        auto avaliable = vector1.available();
        THEN("Result is as expected"){
          REQUIRE(avaliable == 252);

        }
      }

      WHEN("Util function clear is used on vector"){
         vector1.clear();
         THEN("Result is as expected"){
         REQUIRE(vector1.size() == 0);
        }
      }

  }

  GIVEN("Util full function"){
    constexpr size_t buffer_size = 1UL;
    PF::Util::Vector<uint8_t,buffer_size> vector3;
    vector3.push_back(0x05);
      WHEN("Util function full is used on vector"){
        auto full = vector3.full();
        THEN("Result is as expected"){
          REQUIRE(full == true);
        }
      }

    }
}


SCENARIO("Adding bytes to vector"){
  GIVEN("A vector and byte"){
    constexpr size_t buffer_size = 8UL;
    PF::Util::Vector<uint8_t,buffer_size> vector1;
    WHEN("Adding values to vector when index is in bound"){
      THEN("0x08 is added to vector correctly"){
        for(int i =0;i<=6;i++){
          auto push_back = vector1.push_back(i);
        }

        auto push_back = vector1.push_back(0x08);
        REQUIRE(push_back == PF::IndexStatus::ok);
    }
  }
  
  GIVEN("A vector is given"){
   constexpr size_t buffer_size = 10UL ;
   PF::Util::Vector<uint8_t,buffer_size> vector2;
   WHEN("Add values to vector out of index"){
     auto data = std::string("1234567890");
        for (auto& ch : data) {
            vector2.push_back(ch);
        }
        auto push_back = vector2.push_back(0x08);
        THEN("Vector is out of bound"){
          REQUIRE(push_back ==PF::IndexStatus::out_of_bounds);
         
        }
   }
  }
    

}


  GIVEN("A vector and string '\xe3\x03' "){
    constexpr size_t buffer_size = 254UL;
        PF::Util::Vector<uint8_t,buffer_size> buffer;
        auto data = std::string("\xe3\x03");

        for (auto& ch : data) {
            buffer.push_back(ch);
        }
     WHEN("Adding a string \xe3\x03 "){
      THEN("String is added correctly"){
      REQUIRE(buffer[0] == 0xe3);
      REQUIRE(buffer[1] == 0x03);
      }
      }   
  }      
    
}

SCENARIO("Copy one vector to another"){
  GIVEN("Two  vectors with 4UL buffer size"){
    constexpr size_t buffer_size = 4UL;
    PF::Util::Vector<uint8_t,buffer_size> vector1;
        PF::Util::Vector<uint8_t,buffer_size> input_buffer;
        auto data = std::string("\x08\x00\x04\x89");
        for (auto& ch : data) {
            input_buffer.push_back(ch);
        }
   WHEN("Copy one vector of 4UL size  to another of same size"){
            auto vector_copy = vector1.copy_from(
          input_buffer.buffer(), input_buffer.size(), 0);

     THEN("Vector is copied correctly"){
       REQUIRE(vector_copy == PF::IndexStatus::ok);
     }
   }
   WHEN("Vector one of buffer size 4UL is copied to vector of buffer size 2UL"){
    constexpr size_t data_size = 2UL;
    PF::Util::Vector<uint8_t,data_size> vector_copy; 
    auto data_copy = vector_copy.copy_from(
          input_buffer.buffer(), data.size(), 0);
    THEN("First two bytes are copied to vector_copy, index out of bounds, returns 0"){
      REQUIRE(data_copy == PF::IndexStatus::ok);
      for(int i =0;i<=1;i++){
        REQUIRE(vector_copy.operator[](i)== input_buffer.operator[](i) );
      }
    }
    }

    WHEN("Vector 1 is copied to vector 2 of same buffer size "){
        constexpr size_t data_size = 4UL;
        PF::Util::Vector<uint8_t,data_size> vector2; 
        auto data_copy = vector2.copy_from(
          input_buffer.buffer(), data.size(), 0);
          THEN("Result is as expected"){
            REQUIRE(data_copy == PF::IndexStatus::ok );
            for(int i=0;i<=3;i++){
              REQUIRE(vector2.operator[](i) == input_buffer.operator[](i));
            }
          }
  
      }

  }

  GIVEN("Util copy_from function "){
    auto data = PF::Util::make_array<uint8_t>(0x0f, 0x36);
    constexpr size_t data_size = 4UL;
    PF::Util::Vector<uint8_t,data_size> vector2; 
    
    WHEN("Util copy_from is used on array"){
      auto copy = vector2.copy_from(
          data.data(), data.size(), 0);
          THEN("Result is as expected"){
            REQUIRE(copy == PF::IndexStatus::ok);
            REQUIRE(vector2[0]== 0x0f);
            REQUIRE(vector2[1] == 0x36);
          }
    }

    WHEN("Util copy_from is used on array"){
        auto copy = vector2.copy_from(
        data.data(), data.size(), 6);
          THEN("Result is as expected"){
            REQUIRE(copy == PF::IndexStatus::out_of_bounds); 
          }
    }
   
  }

  GIVEN("Util copy_from fucntion"){
      constexpr size_t buffer_size = 4UL;
      PF::Util::Vector<uint8_t,buffer_size> vector1;
      PF::Util::Vector<uint8_t,buffer_size> input_buffer;
      auto data = std::string("\x08\x00\x04\x89");
      for (auto& ch : data) {
            input_buffer.push_back(ch);
        }
      WHEN("Util copy_from is used on array"){
       auto copy = vector1.copy_from(
         input_buffer.buffer(), buffer_size, 0);
         THEN("Result is as expected"){
           REQUIRE(copy == PF::IndexStatus::ok);
          }
      }
  }  



}


SCENARIO("Data is erased from particular index"){
  GIVEN("A vector "){
    constexpr size_t buffer_size = 10UL;
    PF::Util::Vector<uint8_t,buffer_size> vector3;
    auto data = std::string("123456789");
        for (auto& ch : data) {
            vector3.push_back(ch);
        }
        WHEN("Data is erased from 0-2 index"){
        auto erase = vector3.erase(0);
        auto erase8 = vector3.erase(1);
        auto erase9 = vector3.erase(2);

        auto op = vector3.operator[](0);
        auto op8 = vector3.operator[](1);
        auto op9 = vector3.operator[](2);

        THEN("Data is erased from index 0-2"){
          REQUIRE(erase == PF::IndexStatus::ok);
          REQUIRE(erase8 == PF::IndexStatus::ok);
          REQUIRE(erase9 == PF::IndexStatus::ok);
          REQUIRE(op == '2');
          REQUIRE(op8 == '4' );
          REQUIRE(op9 == '6');
        }
        }
        WHEN("Data is erased from 12th index"){
          auto erase = vector3.erase(12);
          auto op = vector3.operator[](12);
          THEN("Out of bounds"){
            REQUIRE(erase == PF::IndexStatus::out_of_bounds);
            REQUIRE(op == 0);
          }
        }

  }
}
