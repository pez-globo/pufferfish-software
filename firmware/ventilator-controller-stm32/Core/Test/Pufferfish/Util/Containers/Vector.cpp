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
SCENARIO(" The function resize in  Vectors work correctly") {
 GIVEN("A uint8_t vector(ByteVector) constructed with array size of 256 bytes") {
    constexpr size_t buffer_size = 256UL;
    PF::Util::Containers::ByteVector<buffer_size> vector;

    WHEN("10 bytes are pushed after which resize method is called with a new size of 260") {
      for (size_t i = 0; i < 10; i++) {
        vector.push_back(i);
      }
      THEN("Before the resize method is called, The size method reports size as 10") {
        REQUIRE(vector.size() == 10);
      }
      THEN("The avaliable method reports that 246 bytes are avaliable") {
        REQUIRE(vector.available() == 246);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      auto status = vector.resize(260);
      THEN("The resize method reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the resize method is called, the size method reports size as 10") {
        REQUIRE(vector.size() == 10);
      }
      THEN("The avaliable method reports that 246 bytes are avaliable") {
        REQUIRE(vector.available() == 246);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The vector has an expected sequence of 10 bytes") {
        for (size_t i = 0; i < 10; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
    WHEN("20 bytes are pushed after which resize method is called with a new size of 256 bytes") {
      for (size_t i = 0; i < 20; i++) {
        vector.push_back(i);
      }
      THEN("Before the resize method is called, The size method reports size as 10") {
        REQUIRE(vector.size() == 20);
      }
      THEN("The avaliable method reports that 236 bytes are avaliable") {
        REQUIRE(vector.available() == 236);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      auto status = vector.resize(256);
      THEN("The resize method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("After the resize method is called, the size method reports size as 256") {
        REQUIRE(vector.size() == 256);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector.available() == 0);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector.full() == true);
      }
      THEN("The vector has an expected sequence of 20 bytes initially pushed, and 236 null bytes") {
        for (size_t i = 0; i < 20; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
        for (size_t i = 20; i < 256; i++) {
          REQUIRE(vector.operator[](i) == 0);
        }
      }
    }
  }

}
SCENARIO("The function in Vector push_back works correctly") {
  GIVEN("Vector with an partially filled buffer with capacity 8") {
    constexpr size_t buffer_size = 8UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f\x0a\xff");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    std::vector<uint8_t> expected = {0xe3, 0x03, 0x08, 0x0f, 0x0a, 0xff, 0x08, 0x01 };

    WHEN(
        "partially filled vector with buffer size 8 is passed as input for adding data at index "
        "6 ") {
      auto push_back = vector1.push_back(0x08);
      auto push_back1 = vector1.push_back(0x01);
      THEN("The push_back method reports ok status") {
        REQUIRE(push_back == PF::IndexStatus::ok);
        REQUIRE(push_back1 == PF::IndexStatus::ok);
      }
      THEN("After push_back returns expected data of size 8 at every index") {
        for(int i =0;i<=7;i++){
           REQUIRE(vector1[i] == expected[i]);
        }
      }
      THEN("After the push_back method is called, The size method reports size as 8") { REQUIRE(vector1.size() == 8); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }
  }

  GIVEN("Partially filled vector with capacity 5") {
    constexpr size_t buffer_size = 5UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    std::vector<uint8_t> expected = {0xe3, 0x03, 0x08, 0x0f, 0x0a, 0x00};    
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
      THEN("Returned data of size 5 is as expected at every index") {
        for(int i = 0;i<=5;i++){
          REQUIRE(vector1[i]== expected[i]);
        }
      }
      THEN("After the push_back method is called, The size method reports size as 5") { REQUIRE(vector1.size() == 5); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("After erase method is called at index 5, it returns ok ") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("After erase method is called at index 10, it returns out of bounds") {
        REQUIRE(vector1.erase(10) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }
  }

  GIVEN("Completely filled vector with  size of 4") {
    constexpr size_t buffer_size = 4UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    std::vector<uint8_t> expected = {0xe3, 0x03, 0x08, 0x0f, 0x00};    

    WHEN("One byte is pushed to 5th index of vector size 4") {
      auto push_back = vector1.push_back(0x00);
      THEN("push_back returns out of bounds, vector is completly filled") {
        REQUIRE(push_back == PF::IndexStatus::out_of_bounds);
      }

      THEN("Returned data of size 5 is as expected at every index ") {
        for(int i =0;i<=3;i++){
          REQUIRE(vector1[i] == expected[i]);
        }
      }
      THEN("After the push_back method is called, The size method reports size as 4") { REQUIRE(vector1.size() == 4); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("After erase method is called at index 2, it returns ok") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("After erase method is called at index , it returns out of bounds") {
        REQUIRE(vector1.erase(7) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }
  }
}

SCENARIO(" The function resize in  Vectors work correctly") {
  GIVEN("Partially filled Vector  with 256 buffer size") {
    constexpr size_t buffer_size = 100UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    auto data = std::string("\xe3\x03\x08\x0f\x0a\xff\x83");
    for (auto& ch : data) {
      vector1.push_back(ch);
    }
    std::vector<uint8_t> expected = {0xe3, 0x03, 0x08, 0x0f, 0x0a, 0xff, 0x83};    


    WHEN("Partially filled vector is resized to 5 buffer size") {
      auto re_vector = vector1.resize(5);
      THEN("Vector is resized correctly to 5 ") { REQUIRE(re_vector == PF::IndexStatus::ok); }
      THEN("Returns correct data of 5 for every indices ") {
        for(int i =0; i<=4; i++){
          REQUIRE(vector1[i] == expected[i] );
        }
      }

      THEN("The avaliable method reports that 95 bytes are avaliable") { REQUIRE(vector1.available() == 95); }
      THEN("After the resize method is called, The size method reports size as 5") { REQUIRE(vector1.size() == 5); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }

      THEN("After erase method is called at index 0, it returns ok") { REQUIRE(vector1.erase(0) == PF::IndexStatus::ok); }
      THEN("After erase method is called at index 5, it returns out of bounds") {
        REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("A partially filled vector is passed to resize for 400 new buffer size ") {
      auto re_vector = vector1.resize(400);
      THEN("Returns  out of bounds") { REQUIRE(re_vector == PF::IndexStatus::out_of_bounds); }
      THEN("After the resize , returned vector of size 7 is as expected at every index") {
        REQUIRE(re_vector == PF::IndexStatus::out_of_bounds);
        for(int i = 0;i<=6;i++){
          REQUIRE(vector1[i] == expected[i]);
        }
      }
      THEN("After the resize method is called, The size method reports size as 7") { REQUIRE(vector1.size() == 7); }
      THEN("The avaliable method reports that 93 bytes are avaliable") { REQUIRE(vector1.available() == 93); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector1.full() == false); }
      THEN("After erase method is called at index 4, it returns ok") { REQUIRE(vector1.erase(4) == PF::IndexStatus::ok); }
      THEN("After erase method is called at index 22, it returns out of bounds") {
        REQUIRE(vector1.erase(22) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN(
        "Partially filled vector is passed as input for resize function longer than original "
        "size") {
      auto re_vector = vector1.resize(25);
      THEN("For resize function it returns ok ") { REQUIRE(re_vector == PF::IndexStatus::ok); }
      THEN("The avaliable method reports that 75 bytes are avaliable") {
        REQUIRE(vector1.available() == 75);
        ;
      }
      THEN("After the resize method is called, The size method reports size as 25") { REQUIRE(vector1.size() == 25); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector1.full() == false); }
      THEN("Returned vector of size 7 has correct data in every index") {
        for(int i = 0;i<=6;i++){
          REQUIRE(vector1[i] == expected[i]);
        }
      }

      THEN("After erase method is called at index 1, it returns ok") {
        REQUIRE(vector1.erase(1) == PF::IndexStatus::ok);
      }
      THEN("After erase method is called at index 20, it returns out of bounds") {
        REQUIRE(vector1.erase(30) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
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
    std::vector<uint8_t> expected = {0x08, 0x01, 0x04, 0x02};
    WHEN("Vector of 4UL is passed as input and for copy_from function") {
      auto vector_copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size(), 0);

      THEN("After copy_from method is called ,copy_from method returns ok") { REQUIRE(vector_copy == PF::IndexStatus::ok); }
      THEN("Returns correct values of vector of size 4 at every index ") {
        for(int i = 0;i<=3;i++){
          REQUIRE(vector1[i] == expected[i]);
        }
      }
      THEN("After the copy_from method is called, The size method reports size as 4") { REQUIRE(vector1.size() == 4); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector of 6UL is passed as input and output is called for dest index 3") {
      constexpr size_t buffer_size = 6UL;
      PF::Util::Containers::Vector<uint8_t, buffer_size> vector;
      std::vector<uint8_t> result = {0,0,0x08,0x01,0x04,0x02};
      auto copy = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 2);
      THEN("After copy_from method is called ,copy_from method returns ok ") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct values of vector 6 at every index") {
        for(int i =0;i<=5;i++){
          REQUIRE(vector.operator[](i) == result.operator[](i));
        }
      }
      THEN("After the copy_from method is called, The size method reports size as 6") { REQUIRE(vector.size() == 6); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector.available() == 0); }
      THEN("After erase method is called at index 9, it returns out of bounds") {
        REQUIRE(vector.erase(9) == PF::IndexStatus::out_of_bounds);
      }
      THEN("After erase method is called at index 2, it returns ok") {
        REQUIRE(vector.erase(2) == PF::IndexStatus::ok);
      }

      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector.empty() == false); }
    }

    WHEN("Vector of 4UL is passed as input for  copy_from function at dest_start_index 5") {
      auto vector_copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size(), 3);
      THEN("After copy_from method is used, it returns out of bounds") {
        REQUIRE(vector_copy == PF::IndexStatus::out_of_bounds);
      }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The avaliable method reports that 4 bytes are avaliable") { REQUIRE(vector1.available() == 4); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector1.empty() == true); }
    }

    WHEN("Vector of 4UL is passed as input for copy_from function, output is 4UL vector") {
      constexpr size_t data_size = 4UL;
      PF::Util::Containers::Vector<uint8_t, data_size> vector_copy;
      auto data_copy = vector_copy.copy_from(input_buffer.buffer(), data.size(), 0);
      THEN("After copy_from method is used, it returns out of bounds") { REQUIRE(data_copy == PF::IndexStatus::ok); }
      THEN("Returns correct data for vector of size 4 at every index") {
        for (int i = 0; i <= 3; i++) {
          REQUIRE(vector_copy.operator[](i) == input_buffer.operator[](i));
        }
      }
      THEN("After the copy_from method is called, The size method reports size as 4") { REQUIRE(vector_copy.size() == 4); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector_copy.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector_copy.full() == true); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector_copy.empty() == false); }
    }
  }
  GIVEN(" uint16 Vector with completely filled buffer with 1UL capacity") {
    constexpr size_t buffer_size = 2UL;
    PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
    PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
    input_buffer.push_back(0x0201);
    WHEN("uint16 vector is passed as an input for copy_from method") {
      auto copy = vector1.copy_from(input_buffer.buffer(), input_buffer.size());
      THEN("After copy_from method is called, it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data") { REQUIRE(vector1[0] == 0x0201); }
      THEN("After the copy_from method is called, The size method reports size as 1") { REQUIRE(vector1.size() == 1); }
      THEN("The avaliable method reports that 1 bytes are avaliable") { REQUIRE(vector1.available() == 1); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
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
      THEN("After copy_from method is called, it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value of vector 2 at every index") {
        REQUIRE(vector2[0] == 0x0f);
        REQUIRE(vector2[1] == 0x36);
      }
      THEN("After the copy_from method is called, The size method reports size as 2") { REQUIRE(vector2.size() == 2); }
      THEN("The avaliable method reports that 2 bytes are avaliable") { REQUIRE(vector2.available() == 2); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector2.full() == false); }
      THEN("Retuns ok for erase at index 2") { REQUIRE(vector2.erase(1) == PF::IndexStatus::ok); }
      THEN("retuns out of bounds for erase used on out of bound index") {
        REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
      }
    }
    WHEN("An array of 2UL size is passed as input to copy_from function for dest index 1") {
      auto copy = vector2.copy_from(data.data(), data.size(), 1);
      THEN("After copy_from method is called, it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value for every index of size 2") {
        REQUIRE(vector2.operator[](0) == 0);
        REQUIRE(vector2.operator[](1) == 0x0f);
      }
      THEN("After the copy_from method is called, The size method reports size as 3") { REQUIRE(vector2.size() == 3); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector2.full() == false); }
      THEN("The avaliable method reports that 1 bytes are avaliable") { REQUIRE(vector2.available() == 1); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector2.empty() == false); }
    }

    WHEN("An array of 2UL size is passed as input to copy_from function for dest index 6") {
      auto copy = vector2.copy_from(data.data(), data.size(), 6);
      THEN("After copy_from function is used for dest index 6, it returns  out of bounds") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
      //      THEN("Returns 2 for size function") { REQUIRE(vector2.size() == 0); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector2.full() == false); }
      THEN("The avaliable method reports that 4 bytes are avaliable") { REQUIRE(vector2.available() == 4); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector2.empty() == true); }
    }

    WHEN("Vector of uint16 is passed as input for copy_from function") {
      auto data = PF::Util::Containers::make_array<uint16_t>(0x0f02);
      constexpr size_t data_size = 4UL;
      PF::Util::Containers::Vector<uint16_t, data_size> vector2;
      auto copy = vector2.copy_from(data.data(), data.size(), 0);
      THEN("After copy_from method is called, it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct value for size 2") { REQUIRE(vector2[0] == 0x0f02); }
      THEN("After the copy_from method is called, The size method reports size as 1") { REQUIRE(vector2.size() == 1); }
      THEN("The avaliable method reports that 3 bytes are avaliable") { REQUIRE(vector2.available() == 3); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector2.full() == false); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector2.empty() == false); }
    }
  }
}

SCENARIO("The function in Vector: copy_from works correctly") {
  GIVEN("Vector with completely filled buffer with 4UL capacity") {
    constexpr size_t buffer_size = 2UL;
    constexpr size_t size = 2UL;
    PF::Util::Containers::Vector<uint8_t, size> vector1;
    PF::Util::Containers::Vector<uint8_t, buffer_size> input_buffer;
    uint8_t* re = input_buffer.buffer();
    auto data = std::string("\x08\x0a");
    for (auto& ch : data) {
      input_buffer.push_back(ch);
    }
    std::vector<uint8_t> expected = {0x08, 0x0a};
    WHEN("Vector of 2UL is passed as input for copy_from function") {
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("After copy_from method is called, it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data for every index for size 2") {
        REQUIRE(vector1.operator[](0) == 0x08);
        REQUIRE(vector1.operator[](1) == 0x0a);
      }
      THEN("After the copy_from method is called, The size method reports size as 2") { REQUIRE(vector1.size() == 2); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("After erase method is called at index 0, it returns ok") {
        REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
      }
      THEN("After erase method is called at index 5, it returns out of bounds") {
        REQUIRE(vector1.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector of 10UL is passed as input for copy_from function for dest index 3") {
      constexpr size_t size = 5UL;
      PF::Util::Containers::Vector<uint8_t, size> vector;
      uint8_t* re = input_buffer.buffer();
      std::vector<uint8_t> result = {0, 0, 0, 0x08, 0x0a};
      auto copy = vector.copy_from(re, buffer_size, 3);
      THEN("After copy_from method is called, it returns ok") {
        REQUIRE(copy == PF::IndexStatus::ok);
      }
      THEN("Returns correct data for every index of size 5") {
        for(int i=0;i<=4;i++){
          REQUIRE(vector.operator[](i) == result.operator[](i));
        }
      }
      THEN("After the copy_from method is called, The size method reports size as 5") { REQUIRE(vector.size() == 5); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector.full() == true); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector.available() == 0); }
      THEN("After erase method is called at index 3, it returns ok") {
        REQUIRE(vector.erase(3) == PF::IndexStatus::ok);
      }
      THEN("After erase method is called at index 5, it returns out of bounds") {
        REQUIRE(vector.erase(5) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector.empty() == false); }
    }

    WHEN("Vector of 2UL is passed as input and output is called for dest index 3") {
      auto copy = vector1.copy_from(re, buffer_size, 3);
      THEN("After copy_from method is used for dest_index 3,it returns out of bounds ") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the copy_from method is called, The size method reports size as 0") { REQUIRE(vector1.size() == 0); }
      THEN("The avaliable method reports that 2 bytes are avaliable") { REQUIRE(vector1.available() == 2); }
      THEN("The full method reports that the vector is not completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector1.empty() == true); }
    }

    WHEN("uint16 vector is passed as input for copy_from function") {
      constexpr size_t buffer_size = 1UL;
      PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
      PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
      uint16_t* re = input_buffer.buffer();
      input_buffer.push_back(0x0201);
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("After copy_from function , it returns ok") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("Returns correct data of uint16") { REQUIRE(vector1[0] == 0x0201); }
      THEN("After the copy_from method is called, The size method reports size as 1") { REQUIRE(vector1.size() == 1); }
      THEN("The avaliable method reports that 0 bytes are avaliable") { REQUIRE(vector1.available() == 0); }
      THEN("The full method reports that the vector is completely filled") { REQUIRE(vector1.full() == true); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
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
    std::vector<uint8_t> expected = {0x08, 0x0a, 0x83, 0x02};
    WHEN("Element *buffer() function is used on vector") {
      vector1.resize(4);
      auto* buffer = vector1.buffer();
      THEN("Returned vector is as expected at every index of size 4") {
        for(int i=0;i<=3;i++){
          REQUIRE(buffer[i] == expected[i]);
        }
      }
      THEN("After the Element *buffer() method is called, The size method reports size as 4") { REQUIRE(vector1.size() == 4); }
      THEN("The avaliable method reports that 96 bytes are avaliable") { REQUIRE(vector1.available() == 96); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
    }

    WHEN("Vector is resized to 2, Element *buffer() gives correct values") {
      vector1.resize(2);
      auto size = vector1.size();
      auto* buffer = vector1.buffer();
      THEN("Return vector of size 2 as expected data") {
        REQUIRE(buffer[0] == 0x08);
        REQUIRE(buffer[1] == 0x0a);
      }
      THEN("After the Element *buffer() method is called, The size method reports size as 2") { REQUIRE(vector1.size() == 2); }
      THEN("The avaliable method reports that 98 bytes are avaliable") { REQUIRE(vector1.available() == 98); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
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
      THEN("After erase method, it returns ok for index 0") { REQUIRE(erase == PF::IndexStatus::ok); }
      THEN("After erase method, it returns ok index 1") {
        auto erase = vector1.erase(0);
        REQUIRE(erase == PF::IndexStatus::ok);
      }
      THEN("After erase method, it returns out of bounds for index 3") {
        auto erase2 = vector1.erase(3);
        REQUIRE(erase2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 1") { REQUIRE(vector1.size() == 1); }
      THEN("The avaliable method reports that 1 bytes are avaliable") { REQUIRE(vector1.available() == 1); }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is non-empty") { REQUIRE(vector1.empty() == false); }
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
      THEN("After erase method, it returns ok for index 0") { REQUIRE(erase == PF::IndexStatus::ok); }
      THEN("After erase method, it returns out of bounds for index 1") {
        REQUIRE(erase1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 0") { REQUIRE(vector1.size() == 0); }
      THEN("The avaliable method reports that 1 bytes are avaliable") {
        REQUIRE(vector1.available() == 1);
      }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector1.empty() == true); }
    }
  }

  GIVEN("Empty vector of buffer size 5UL") {
    constexpr size_t buffer_size = 5UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> vector1;
    WHEN("Empty vector of size 5UL is passed as input for erase function") {
      auto erase = vector1.erase(0);
      THEN("After erase method, it returns out of bounds for index 0") {
        REQUIRE(erase == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 0") { REQUIRE(vector1.size() == 0); }
      THEN("The avaliable method reports that 5 bytes are avaliable") {
        REQUIRE(vector1.available() == 5);
      }
      THEN("The full method reports that the vector is not-completely filled") { REQUIRE(vector1.full() == false); }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector1.empty() == true); }
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
    std::vector<uint8_t> expected = {0x08, 0x0a, 0x05, 0x0f, 0x0c, 0x0d, 0x03, 0xff, 0x09, 0x01};
    WHEN("Function operator is used on 20UL size vector") {
      THEN(
          "Returns correct values for operator function for size of 10 ") {
            for(int i =0;i<=9;i++){
              REQUIRE(vector1.operator[](i) == expected.operator[](i));
            }
      }
      THEN("After operator method, it returns 0 for 15th and 20th index") {
        REQUIRE(vector1.operator[](15) == 0);
        REQUIRE(vector1.operator[](20) == 0);
      }
    }
  }
}
