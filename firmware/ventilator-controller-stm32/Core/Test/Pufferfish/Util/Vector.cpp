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
#include "catch2/catch.hpp"

namespace PF = Pufferfish;




//constexpr size_t buffer_size = 252UL;
SCENARIO( " Vectors are resized", "[vector]" ) {
  GIVEN("Vector array of 252UL size "){
  constexpr size_t buffer_size = 252UL;
  PF::Util::Vector<uint8_t,buffer_size> vector1;
      WHEN("Vector is resized to 256UL"){
       auto vector_size = vector1.max_size();
      REQUIRE(vector_size == 252);
        THEN("Vector is resized correctly to 256 ")
        auto re_vector = vector1.resize(256);
         REQUIRE("re_vector = PF::IndexStatus::ok");
}
}
}

SCENARIO("Adding bytes to vector"){
  GIVEN("A vector and byte"){
    constexpr size_t buffer_size = 252UL;
PF::Util::Vector<uint8_t,buffer_size> vector1;
    WHEN("Adding 0x08 to vector"){
      THEN("0x08 is added to vector correctly"){
auto push_back = vector1.push_back(0x08);
REQUIRE(push_back == PF::IndexStatus::ok);
REQUIRE(vector1[0] == 0x08);
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
  GIVEN("Two  vectors with 254UL buffer size"){
    constexpr size_t buffer_size = 254UL;
    PF::Util::Vector<uint8_t,buffer_size> vector1;
        PF::Util::Vector<uint8_t,buffer_size> input_buffer;
        auto data = std::string("123456");
        for (auto& ch : data) {
            input_buffer.push_back(ch);
        }
   WHEN("Copy one vector of 254UL size  to another of same size"){
            auto vector_copy = vector1.copy_from(
          input_buffer.buffer(), input_buffer.size(), 0);

     THEN("Vector is copied correctly"){
       REQUIRE(vector_copy == PF::IndexStatus::ok);
     }
   }
   WHEN("Vector one of buffer size 254UL is copied to vector of buffer size 8UL"){
    constexpr size_t data_size = 8UL;
    PF::Util::Vector<uint8_t,data_size> Vector2; 
    auto data_copy = vector1.copy_from(
          Vector2.buffer(), data.size(), 0);
    THEN("Copy from vector1 to vector2  works correctly"){
      REQUIRE(data_copy == PF::IndexStatus::ok);
    }
    }
  }
}
