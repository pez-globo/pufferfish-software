/*
 * Vector.tpp
 * Test1
 *  Created on: April 07, 2021
 *      Author: Raavi Lagoo
 *
 * */

//#pragma once

#include "Pufferfish/Util/Containers/Vector.h"

#include <cstring>

#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

// constexpr size_t buffer_size = 252UL;
SCENARIO(" The function resize in  Vectors work correctly") {
  GIVEN("Vector with empty internal buffer with 256 buffer size") {
    constexpr size_t buffer_size = 100UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f\x0a\xff\x83");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }

    WHEN("Partially filled vector is resized to smaller buffer size") {
      auto re_vector = vector1.resize(5);
      THEN("Vector is resized correctly to 5 ") { REQUIRE(re_vector == PF::IndexStatus::ok); }
      THEN("Returns correct data for every indices [0xe3,0x03,0x08,0x0f,0x0a]") {
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
        REQUIRE(vector1[4] == 0x0a);
      }

      THEN("Returns 95 for avaliable function") { REQUIRE(vector1.available() == 95); }
      THEN("Vector size is 5 for size function") { REQUIRE(vector1.size() == 5); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }

      THEN("Returns ok for erase") { REQUIRE(vector1.erase(0) == PF::IndexStatus::ok); }
      THEN("Returns out of bounds for , for out of bound index") {
        REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("A partially filled vector is passed to resize beyond its maximum capacity") {
      auto re_vector = vector1.resize(400);
      THEN("Returns  out of bounds") { REQUIRE(re_vector == PF::IndexStatus::out_of_bounds); }
      THEN("Result is as expected at every index,[0xe3, 0x03, 0x08, 0x0f, 0x0a, 0xff ,0x83]") {
        REQUIRE(re_vector == PF::IndexStatus::out_of_bounds);
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
        REQUIRE(vector1[4] == 0x0a);
        REQUIRE(vector1[5] == 0xff);
        REQUIRE(vector1[6] == 0x83);
      }
      THEN("Returns 7 for size function") { REQUIRE(vector1.size() == 7); }
      THEN("Returns 93 for avaliable function") { REQUIRE(vector1.available() == 93); }
      THEN("Returns false, for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns ok for erase at index 4") { REQUIRE(vector1.erase(4) == PF::IndexStatus::ok); }
      THEN("Returns out of bounds, for index 22") {
        REQUIRE(vector1.erase(22) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }

    WHEN(
        "Partially filled vector is passed as input for resize function longer than original "
        "size") {
      auto re_vector = vector1.resize(25);
      THEN("Resize returns ok ") { REQUIRE(re_vector == PF::IndexStatus::ok); }
      THEN("Returns correct avaliable size 75 for avaliable function") {
        REQUIRE(vector1.available() == 75);
        ;
      }
      THEN("Returns 25 s for size function") { REQUIRE(vector1.size() == 25); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returned vector has correct data in every index[0xe3,0x03,0x08,0x0f,0x0a,0xff,0x83]") {
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
        REQUIRE(vector1[4] == 0x0a);
        REQUIRE(vector1[5] == 0xff);
        REQUIRE(vector1[6] == 0x83);
      }

      THEN("Returns ok for erase function for index 1") {
        REQUIRE(vector1.erase(1) == PF::IndexStatus::ok);
      }
      THEN("Returns out of bound for erase function for index 20") {
        REQUIRE(vector1.erase(30) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO("The function in Vector push_back works correctly") {
  GIVEN("Vector with an empty internal buffer with capacity 8") {
    constexpr size_t buffer_size = 8UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f\x0a\xff");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }

    WHEN(
        "partially filled vector with buffer size 8 is passed as input for adding data at index "
        "6 ") {
      auto push_back = vector1.push_back(0x08);
      auto push_back1 = vector1.push_back(0x01);
      THEN("Returns ok for push_back function at index 6 and 7") {
        REQUIRE(push_back == PF::IndexStatus::ok);
        REQUIRE(push_back1 == PF::IndexStatus::ok);
      }
      THEN("Returns expected data at every index [0xe3,0x03,0x08, 0x0f,0x0a, 0xff, 0x08, 0x01]") {
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
        REQUIRE(vector1[4] == 0x0a);
        REQUIRE(vector1[5] == 0xff);
        REQUIRE(vector1[6] == 0x08);
        REQUIRE(vector1[7] == 0x01);
      }

      THEN("Returns expected size 8 for size function") { REQUIRE(vector1.size() == 8); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Returns expected true for full function") { REQUIRE(vector1.full() == true); }
      THEN("Returns ok for erase function used on index 5 ") {
        REQUIRE(vector1.erase(5) == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for erase function used on index 10") {
        REQUIRE(vector1.erase(10) == PF::IndexStatus::out_of_bounds);
      }

      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }

  GIVEN("Partially filled vector with capacity 5") {
    constexpr size_t buffer_size = 5UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN(
        "partially filled vector with buffer size 5 is passed as input for adding data at index 5 "
        "and 7") {
      auto push_back = vector1.push_back(0x0a);
      auto push_back1 = vector1.push_back(0x00);
      THEN("Returns ok for push_back function at index 5") {
        REQUIRE(push_back == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for push_back function at index 7") {
        REQUIRE(push_back1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returned data is as expected at every index (0xe3, 0x03, 0x08, 0x0f, 0x0a)") {
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
        REQUIRE(vector1[4] == 0x0a);
      }
      THEN("Returns expected size 5 for size function") { REQUIRE(vector1.size() == 5); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Returns expected true for full function") { REQUIRE(vector1.full() == true); }
      THEN("Returns ok for erase function used on index 5") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for erase function used on index 10") {
        REQUIRE(vector1.erase(10) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }

  GIVEN("Completely filled vector with vector size 4") {
    constexpr size_t buffer_size = 4UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN("completely filled vector with buffer size 4 is passed to add data at index 5") {
      auto push_back = vector1.push_back(0x00);
      THEN("Returns out of bounds, vector is completly filled") {
        REQUIRE(push_back == PF::IndexStatus::out_of_bounds);
      }

      THEN("Returned data is as expected at every index (0xe3, 0x03, 0x08, 0x0f)") {
        REQUIRE(vector1[0] == 0xe3);
        REQUIRE(vector1[1] == 0x03);
        REQUIRE(vector1[2] == 0x08);
        REQUIRE(vector1[3] == 0x0f);
      }
      THEN("Returns expected size 4 for size function") { REQUIRE(vector1.size() == 4); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Returns expected true for full function") { REQUIRE(vector1.full() == true); }
      THEN("Returns ok for erase function used on index 2") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for erase function used on index 7") {
        REQUIRE(vector1.erase(7) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO("The function in Vector: copy_from works correctly on vector") {
  GIVEN("Vector with completely filled buffer with 4UL capacity") {
    constexpr size_t buffer_size = 4UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    PF::Util::Containers::Vector<uint8_t, buffer_size> input_buffer;
    auto data = std::string("\x08\x01\x04\x02");
    for (auto& ch : data) {
      input_buffer.push_back(ch);
    }
    WHEN("Vector of 4UL is passed as input and output is called") {
      auto vector_copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size(), 0);

      THEN("Vector is copied correctly") { REQUIRE(vector_copy == PF::IndexStatus::ok); }
      THEN("Returns correct values at every index [0x08, 0x01, 0x04, 0x02]") {
        REQUIRE(vector1.operator[](0) == 0x08);
        REQUIRE(vector1.operator[](1) == 0x01);
        REQUIRE(vector1.operator[](2) == 0x04);
        REQUIRE(vector1.operator[](3) == 0x02);
      }
      THEN("Returns as expected 4 for size function") { REQUIRE(vector1.size() == 4); }
      THEN("Returns as expected 0 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Returns true for full function") { REQUIRE(vector1.full() == true); }
      THEN("Returns false for empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector of 6UL is passed as input and output is called for dest index 3") {
      constexpr size_t buffer_size = 6UL;
      PF::Util::Containers::Vector<uint8_t, buffer_size> vector;

      auto copy = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 2);
      THEN("Returns ok for copy_from function ") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct values at every index[0,0,0x08,0x01,0x04,0x02]") {
        REQUIRE(vector.operator[](0) == 0);
        REQUIRE(vector.operator[](1) == 0);
        REQUIRE(vector.operator[](2) == 0x08);
        REQUIRE(vector.operator[](3) == 0x01);
        REQUIRE(vector.operator[](4) == 0x04);
        REQUIRE(vector.operator[](5) == 0x02);
      }
      THEN("Returns expected 1, for size function") { REQUIRE(vector.size() == 6); }
      THEN("Returns expected 3 for avaliable function") { REQUIRE(vector.available() == 0); }
      THEN("Returns ok for erase on 3rd index") {
        REQUIRE(vector.erase(9) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Rrturns out of bounds if index is out of bounds 5") {
        REQUIRE(vector.erase(2) == PF::IndexStatus::ok);
      }

      THEN("Returns false for empty function") { REQUIRE(vector.empty() == false); }
    }

    WHEN("Vector of 4UL is passed as input for  copy_from function at dest_start_index 5") {
      auto vector_copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size(), 3);
      THEN("Returns out of bound, vector copy fails") {
        REQUIRE(vector_copy == PF::IndexStatus::out_of_bounds);
      }
//      THEN("Returns 4 for size function") { REQUIRE(vector1.size() == 0); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns 4 for avaliable function") { REQUIRE(vector1.available() == 4); }
      THEN("Returns true for empty function") { REQUIRE(vector1.empty() == true); }
    }

    WHEN("Vector of 4UL is passed as input for copy_from function, output is 4UL vector") {
      constexpr size_t data_size = 4UL;
      PF::Util::Containers::Vector<uint8_t, data_size> vector_copy;
      auto data_copy = vector_copy.copy_from(input_buffer.buffer(), data.size(), 0);
      THEN("Returns okay for copy_from") { REQUIRE(data_copy == PF::IndexStatus::ok); }
      THEN("Returns correct data at every index") {
        for (int i = 0; i <= 3; i++) {
          REQUIRE(vector_copy.operator[](i) == input_buffer.operator[](i));
        }
      }
      THEN("Returns 2 for size function") { REQUIRE(vector_copy.size() == 4); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector_copy.available() == 0); }
      THEN("Returns true for full function") { REQUIRE(vector_copy.full() == true); }
      THEN("Returns false for empty function") { REQUIRE(vector_copy.empty() == false); }
    }
  }
  GIVEN(" uint16 Vector with completely filled buffer with 1UL capacity") {
    constexpr size_t buffer_size = 2UL;
    PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
    PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
    input_buffer.push_back(0x0201);
    WHEN("uint16 vector is passed as an input for copy_from function") {
      auto copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size());
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data 0x0201") { REQUIRE(vector1[0] == 0x0201); }
      THEN("Returns 1 as size function") { REQUIRE(vector1.size() == 1); }
      THEN("Returns 1 for avaliable function") { REQUIRE(vector1.available() == 1); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO("Copy_from function for array works correctly") {
  GIVEN("Vector with completely filled buffer with 4UL capacity") {
    auto data = PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36);
    constexpr size_t data_size = 4UL;
    PF::Util::Containers::Vector<uint8_t, data_size> vector2;

    WHEN("An array of 2UL size is passed as input for copy_from function ") {
      auto copy = vector2.copy_from(data.data(), data.size(), 0);
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value at every index") {
        REQUIRE(vector2[0] == 0x0f);
        REQUIRE(vector2[1] == 0x36);
      }
      THEN("Returns correct size") { REQUIRE(vector2.size() == 2); }
      THEN("Returns correct avaliable buffers") { REQUIRE(vector2.available() == 2); }
      THEN("Returns false for full") { REQUIRE(vector2.full() == false); }
      THEN("Retuns ok for erase at index 2") { REQUIRE(vector2.erase(1) == PF::IndexStatus::ok); }
      THEN("retuns out of bounds for erase used on out of bound index") {
        REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
      }
    }
    WHEN("An array of 2UL size is passed as input to copy_from function for dest index 1") {
      auto copy = vector2.copy_from(data.data(), data.size(), 1);
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value for every index[0, 0x0f]") {
        REQUIRE(vector2.operator[](0) == 0);
        REQUIRE(vector2.operator[](1) == 0x0f);
      }
      THEN("Returns 2 for size function") { REQUIRE(vector2.size() == 3); }
      THEN("Returns false for full function") { REQUIRE(vector2.full() == false); }
      THEN("Returns 1 for avaliable function") { REQUIRE(vector2.available() == 1); }
      THEN("Returns false for empty function") { REQUIRE(vector2.empty() == false); }
    }

    WHEN("An array of 2UL size is passed as input to copy_from function for dest index 6") {
      auto copy = vector2.copy_from(data.data(), data.size(), 6);
      THEN("Result is out of bounds for index 6") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
//      THEN("Returns 2 for size function") { REQUIRE(vector2.size() == 0); }
      THEN("Returns false for full function") { REQUIRE(vector2.full() == false); }
      THEN("Returns 1 for avaliable function") { REQUIRE(vector2.available() == 4); }
      THEN("Returns true for empty function") { REQUIRE(vector2.empty() == true); }
    }

    WHEN("Vector of uint16 is passed as input for copy_from function") {
      auto data = PF::Util::Containers::make_array<uint16_t>(0x0f02);
      constexpr size_t data_size = 4UL;
      PF::Util::Containers::Vector<uint16_t, data_size> vector2;
      auto copy = vector2.copy_from(data.data(), data.size(), 0);
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value 0x0f02") { REQUIRE(vector2[0] == 0x0f02); }
      THEN("Returns 1 for size function") { REQUIRE(vector2.size() == 1); }
      THEN("Returns 3 for avaliable function") { REQUIRE(vector2.available() == 3); }
      THEN("Returns false for full function") { REQUIRE(vector2.full() == false); }
      THEN("Returns false for empty") { REQUIRE(vector2.empty() == false); }
    }
  }
}

SCENARIO("The function in Vector: copy_from works correctly") {
  GIVEN("Vector with completely filled buffer with 4UL capacity") {
    constexpr size_t buffer_size = 2UL;
    constexpr size_t size = 2UL;
    PF::Util::Containers::Vector<uint8_t, size> vector1;
    PF::Util::Containers::Vector<uint8_t ,buffer_size> input_buffer;
    uint8_t* re = input_buffer.buffer();
    auto data = std::string("\x08\x0a");
    for (auto& ch : data) {
      input_buffer.push_back(ch);
    }
    WHEN("Vector of 2UL is passed as input for copy_from function") {
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data for every index [0x08, 0x0a]") {
        REQUIRE(vector1.operator[](0) == 0x08);
        REQUIRE(vector1.operator[](1) == 0x0a);
      }
      THEN("Returns 2 for size function") { REQUIRE(vector1.size() == 2); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == true); }
      THEN("Retuns 0 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Erase returns ok, index in in bounds") {
        REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase returns out of bounds , index is out of bounds") {
        REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector of 10UL is passed as input for copy_from function for dest index 2") {
      constexpr size_t size = 5UL;
      PF::Util::Containers::Vector<uint8_t, size> vector;
//      uint8_t* re = input_buffer.buffer();
      auto copy = vector.copy_from(re, buffer_size, 3);
      THEN("Returns ok for copy_from function at 3 dest index") {
        REQUIRE(copy == PF::IndexStatus::ok);
      }
      THEN("Returns correct data for every index [0, 0x08, 0x0a]") {
        REQUIRE(vector.operator[](0) == 0);
        REQUIRE(vector.operator[](3) == 0x08);
        REQUIRE(vector.operator[](4) == 0x0a);
      }
      THEN("Returns 2 for size function") { REQUIRE(vector.size() == 5); }
      THEN("Returns false for full function") { REQUIRE(vector.full() == true); }
      THEN("Retuns 0 for avaliable function") { REQUIRE(vector.available() == 0); }
      THEN("Erase returns ok, index in in bounds") {
        REQUIRE(vector.erase(3) == PF::IndexStatus::ok);
      }
      THEN("Erase returns out of bounds , index is out of bounds") {
        REQUIRE(vector.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns false for empty function") { REQUIRE(vector.empty() == false); }
    }

    WHEN("Vector of 2UL is passed as input and output is called for dest index 3") {
      auto copy = vector1.copy_from(re, buffer_size, 3);
      THEN("Returns out of bounds for copy_from function for index 3") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
//      THEN("Returns 2 for size function") { REQUIRE(vector1.size() == 0); }
      THEN("Returns 2 for avaliable function") { REQUIRE(vector1.available() == 2); }
      THEN("Returns true for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns true for empty function") { REQUIRE(vector1.empty() == true); }
    }

    WHEN("uint16 vector is passed as input for copy_from function") {
      constexpr size_t buffer_size = 1UL;
      PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
      PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
      uint16_t* re = input_buffer.buffer();
      input_buffer.push_back(0x0201);
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("Returns ok for copy_from function") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data 0x0201") { REQUIRE(vector1[0] == 0x0201); }
      THEN("Returns 1 as size function") { REQUIRE(vector1.size() == 1); }
      THEN("Returns 1 for avaliable function") { REQUIRE(vector1.available() == 0); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == true); }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO("The function in Vector:Element *buffer() works correctly") {
  GIVEN("Vector with partially filled with buffer size 100UL") {
    constexpr size_t buffer_size = 100UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\x08\x0a\x83\x02");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN("Elent *buffer() function is used on vector") {
      vector1.resize(4);
      auto *buffer = vector1.buffer();
      THEN("Returned vector is as expected at every index (0x08,0x0a,0x83,0x02)") {
        REQUIRE(buffer[0] == 0x08);
        REQUIRE(buffer[1] == 0x0a);
        REQUIRE(buffer[2] == 0x83);
        REQUIRE(buffer[3] == 0x02);
      }
      THEN("Returns 4 for size function") { REQUIRE(vector1.size() == 4); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector1.available() == 96); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector is resized to 2, Element *buffer() gives correct values") {
      vector1.resize(2);
      auto size = vector1.size();
      auto *buffer = vector1.buffer();
      THEN("Return vector size is 2 as expected") { REQUIRE(size == 2); }
      THEN("Return vector has expected data (0x08,0x0a)") {
        REQUIRE(buffer[0] == 0x08);
        REQUIRE(buffer[1] == 0x0a);
      }
      THEN("Returns 4 for size function") { REQUIRE(vector1.size() == 2); }
      THEN("Returns 0 for avaliable function") { REQUIRE(vector1.available() == 98); }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO("The function in vector: erase works correctly") {
  GIVEN("Completely filled vector with 2UL buffer size") {
    constexpr size_t buffer_size = 2UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\x08\x0a");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN("completelty filled vector with 2UL size is passed as input for erase function") {
      auto erase = vector1.erase(0);
      THEN("Returns ok for index 0") { REQUIRE(erase == PF::IndexStatus::ok); }
      THEN("Returns ok for index 1") {
        auto erase = vector1.erase(0);
        REQUIRE(erase == PF::IndexStatus::ok);
      }
      THEN("Returns out of bounds for index 3") {
        auto erase2 = vector1.erase(3);
        REQUIRE(erase2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("Returns correct size for size function") { REQUIRE(vector1.size() == 1); }
      THEN("Returns 2  for avaliable function") { REQUIRE(vector1.available() == 1); }
      THEN("Returns true for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns false for empty function") { REQUIRE(vector1.empty() == false); }
    }
  }

  GIVEN("Completely filled vector with 1UL buffer size") {
    constexpr size_t buffer_size = 1UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\x08");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN("completelty filled vector with 1UL size is passed as input for erase function") {
      auto erase = vector1.erase(0);
      auto erase1 = vector1.erase(1);
      THEN("Returns ok for index 0") { REQUIRE(erase == PF::IndexStatus::ok); }
      THEN("Returns out of bounds for index 1") {
        REQUIRE(erase1 == PF::IndexStatus::out_of_bounds);
      }
//      THEN("Returns size 1 as expected for size function") { REQUIRE(vector1.size() == 0); }
      THEN("Returns correcte avaliable vector for function avaliable") {
        REQUIRE(vector1.available() == 1);
      }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns true for empty function") { REQUIRE(vector1.empty() == true); }
    }
  }

  GIVEN("Empty vector of buffer size 5UL") {
    constexpr size_t buffer_size = 5UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    WHEN("Empty vector of size 5UL is passed as input for erase function") {
      auto erase = vector1.erase(0);
      THEN("Returns out of bounds for index 0") {
        REQUIRE(erase == PF::IndexStatus::out_of_bounds);
      }
//      THEN("Returns size 1 as expected for size function") { REQUIRE(vector1.size() == 0); }
      THEN("Returns correcte avaliable vector for function avaliable") {
        REQUIRE(vector1.available() == 5);
      }
      THEN("Returns false for full function") { REQUIRE(vector1.full() == false); }
      THEN("Returns ok true for empty function") { REQUIRE(vector1.empty() == true); }
    }
  }
}

SCENARIO("A vector function operator works correctly") {
  GIVEN("A vector partially filled with 20UL size") {
    constexpr size_t buffer_size = 20UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\x08\x0a\x05\x0f\x0c\x0d\x03\xff\x09\x01");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    WHEN("Function operator is used on 20UL size vector") {
      auto op = vector1.operator[](0);
      THEN(
          "Returns correct values for operator function for 0-10th index[0x0a, 0x05, 0x0f, 0x0c, "
          "0x0d, 0x03, 0xff, 0x09, 0x01]") {
        REQUIRE(op == 0x08);
        REQUIRE(vector1.operator[](1) == 0x0a);
        REQUIRE(vector1.operator[](2) == 0x05);
        REQUIRE(vector1.operator[](3) == 0x0f);
        REQUIRE(vector1.operator[](4) == 0x0c);
        REQUIRE(vector1.operator[](5) == 0x0d);
        REQUIRE(vector1.operator[](6) == 0x03);
        REQUIRE(vector1.operator[](7) == 0xff);
        REQUIRE(vector1.operator[](8) == 0x09);
        REQUIRE(vector1.operator[](9) == 0x01);
      }
      THEN("Returns 0 for 15th and 20th index for operator function") {
        REQUIRE(vector1.operator[](15) == 0);
        REQUIRE(vector1.operator[](20) == 0);
      }
    }
  }
}
