/*
 * Vector.tpp
 * Test1
 *  Created on: April 07, 2021
 *      Author: Raavi Lagoo
 *
 * */

//#pragma once

#include "Pufferfish/Util/Containers/Vector.h"

#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
using PF::Util::Containers::ByteVector;
SCENARIO("The function in Vector: push_back works correctly") {
  GIVEN("Partially filled Vector with 252 bytes data and capacity 254") {
    constexpr size_t buffer_size = 254UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i < 252; i++) {
      vector1.push_back(i);
    }

    WHEN("The push_back method is called twice on vector of 252 bytes of data") {
      auto status = vector1.push_back(252);
      auto status1 = vector1.push_back(253);
      THEN("The push_back method returns ok status 1st time") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The push_back method returns ok status for 2nd time") {
        REQUIRE(status1 == PF::IndexStatus::ok);
      }
      THEN("After the push_back, The size method reports size as 254") {
        REQUIRE(vector1.size() == 254);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN(
          "The vector has an expected sequence of 252 bytes initially pushed and two more bytes at "
          "the end") {
        for (int i = 0; i <= 253; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }

    WHEN("The push_back method is called thrice on vector having 252 bytes ") {
      auto status = vector1.push_back(252);
      vector1.push_back(253);
      auto status2 = vector1.push_back(254);
      THEN("The push_back method returns ok status for 1st time") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The push_back method returns out of bounds status for 2nd time") {
        REQUIRE(status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the push_back method is called, The size method reports size as 254") {
        REQUIRE(vector1.size() == 254);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The erase method reports ok status on input of 0th index ") {
        REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status on input of 252nd index") {
        REQUIRE(vector1.erase(252) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status for input of 254th index") {
        REQUIRE(vector1.erase(254) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN(
          "The vector has an expected sequence of 252 bytes initially pushed and 253 pushed  at "
          "the end") {
        for (int i = 0; i <= 253; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }
  }

  GIVEN("Completely filled vector with capacity 254") {
    constexpr size_t buffer_size = 254UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i <= 253; i++) {
      vector1.push_back(i);
    }

    WHEN("The push_back method is called once on vector") {
      auto status = vector1.push_back(254);
      THEN("The push_back method reports out of bounds status ") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }

      THEN("After the push_back method is called, The size method reports size as 254") {
        REQUIRE(vector1.size() == 254);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The erase method reports ok status on input of 2nd index ") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status on input of 254 index ") {
        REQUIRE(vector1.erase(254) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN(
          "vector has expected sequence of "
          "254 bytes ") {
        for (int i = 0; i <= 253; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }
  }
  GIVEN("Empty vector with capacity 254") {
    constexpr size_t buffer_size = 254UL;
    ByteVector<buffer_size> vector1;
    WHEN("The push back method is used on empty vector for 202 times") {
      for (size_t i = 0; i <= 201; i++) {
        auto status = vector1.push_back(i);

        THEN("The push_back method returns ok status for all 202 bytes") {
          REQUIRE(status == PF::IndexStatus::ok);
        }
      }
      THEN("After the push_back, The size method reports size as 202") {
        REQUIRE(vector1.size() == 202);
      }
      THEN("The avaliable method reports that 52 bytes are avaliable") {
        REQUIRE(vector1.available() == 52);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The erase method reports ok status on input of 2nd index") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bound status on input of 258 index ") {
        REQUIRE(vector1.erase(258) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("the vector has expected sequence of 202 bytes") {
        for (int i = 0; i <= 201; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }
  }
}

SCENARIO(" The function resize in  Vectors work correctly") {
  GIVEN(
      "A uint8_t vector constructed with array size of 256 bytes is partially filled with 10 bytes "
      "of data ") {
    constexpr size_t buffer_size = 256UL;
    PF::Util::Containers::Vector<uint8_t, buffer_size> input_buffer;
    for (size_t i = 0; i < 10; i++) {
      input_buffer.push_back(i);
    }
    WHEN("Resize method is called on vector of new size 260") {
      auto status = input_buffer.resize(260);
      THEN("The resize method reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the resize method is called, the size method reports size as 10") {
        REQUIRE(input_buffer.size() == 10);
      }
      THEN("The avaliable method reports that 246 bytes are avaliable") {
        REQUIRE(input_buffer.available() == 246);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(input_buffer.empty() == false);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(input_buffer.full() == false);
      }
      THEN("The erase method reports ok status on input of 2nd index") {
        REQUIRE(input_buffer.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports ok status on input of 256th index") {
        REQUIRE(input_buffer.erase(256) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 10 bytes") {
        for (size_t i = 0; i < 10; i++) {
          REQUIRE(input_buffer.operator[](i) == i);
        }
      }
    }
    WHEN("resize method is called on input vector with a new size of 50 bytes") {
      auto status = input_buffer.resize(50);
      THEN("The resize method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("After the resize method is called, the size method reports size as 250") {
        REQUIRE(input_buffer.size() == 50);
      }
      THEN("The avaliable method reports that 6 bytes are avaliable") {
        REQUIRE(input_buffer.available() == 206);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(input_buffer.empty() == false);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(input_buffer.full() == false);
      }
      THEN("The erase method reports ok status on input of 2nd index") {
        REQUIRE(input_buffer.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports ok status on input of 256th index") {
        REQUIRE(input_buffer.erase(256) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The vector has an expected sequence of 10 bytes initially pushed") {
        for (size_t i = 0; i < 10; i++) {
          REQUIRE(input_buffer.operator[](i) == i);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t vector constructed with an array size of 100 bytes is partially filled with 20 "
      "bytes of data") {
    constexpr size_t buffer_size = 100UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i < 20; i++) {
      vector1.push_back(i);
    }

    WHEN("Vector is resized to new buffer size 50") {
      auto resize_status = vector1.resize(50);
      THEN("The resize method reports ok status ") {
        REQUIRE(resize_status == PF::IndexStatus::ok);
      }

      THEN("The avaliable method reports that 50 bytes are avaliable") {
        REQUIRE(vector1.available() == 50);
      }
      THEN("After the resize method is called, The size method reports size as 50") {
        REQUIRE(vector1.size() == 50);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("The erase method reports ok status on input of 2nd index") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports ok status on input of 258th index") {
        REQUIRE(vector1.erase(258) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The vector has an expected sequence of 20 bytes initially pushed") {
        for (int i = 0; i < 20; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }
    WHEN("Vector is resized to new buffer size of 10") {
      auto resize_status = vector1.resize(10);
      THEN("The resize method reports ok status ") {
        REQUIRE(resize_status == PF::IndexStatus::ok);
      }

      THEN("The avaliable method reports that 90 bytes are avaliable") {
        REQUIRE(vector1.available() == 90);
      }
      THEN("After the resize method is called, The size method reports size as 10") {
        REQUIRE(vector1.size() == 10);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("The erase method reports ok status on input of 2nd index") {
        REQUIRE(vector1.erase(2) == PF::IndexStatus::ok);
      }
      THEN("The erase method reports ok status on input of 258th index") {
        REQUIRE(vector1.erase(258) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The vector has an expected sequence of 10 bytes initially pushed") {
        for (int i = 0; i < 10; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }
  }
}

SCENARIO("Copy_from function for Vector works correctly ") {
  GIVEN("Empty vector with capacity 252") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector1;
    ByteVector<buffer_size> input_buffer;
    ByteVector<254> expected;
    for (size_t i = 0; i < 252; i++) {
      input_buffer.push_back(i);
    }
    expected.push_back(0);
    expected.push_back(0);
    for (size_t i = 0; i < 252; i++) {
      expected.push_back(i);
    }

    WHEN("The copy_from method is called on empty vector of capacity 252, for dest index 0") {
      auto copy_status = vector1.copy_from(input_buffer.buffer(), input_buffer.size(), 0);

      THEN("The copy_from method reports ok status") {
        REQUIRE(copy_status == PF::IndexStatus::ok);
      }
      THEN("After the copy_from method is called, The size method reports size as 252") {
        REQUIRE(vector1.size() == 252);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("Erase method returns out of bounds status with the input of index 0") {
        REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status  with the input of index 251") {
        REQUIRE(vector1.erase(251) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds  status for  the input of index 252") {
        REQUIRE(vector1.erase(252) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 200 bytes ") {
        for (int i = 0; i < 252; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }

    WHEN("The copy_from method is called on  vector of capacity 256, for dest index 2  ") {
      constexpr size_t buffer_size = 256UL;
      ByteVector<buffer_size> vector;
      auto copy = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 2);
      THEN("The copy_from method reports ok status ") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 254") {
        REQUIRE(vector.size() == 254);
      }
      THEN("The avaliable method reports that 2 bytes are avaliable") {
        REQUIRE(vector.available() == 2);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status for the input of index 254") {
        REQUIRE(vector.erase(254) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 254 bytes ") {
        for (int i = 0; i < 254; i++) {
          REQUIRE(vector.operator[](i) == expected.operator[](i));
        }
      }
    }

    WHEN("The copy_from method is called on empty vector of capacity 100, for dest index 3") {
      constexpr size_t buffer_size = 100UL;
      ByteVector<buffer_size> vector;
      auto status = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 3);
      THEN("The copy_from method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 100 bytes are avaliable") {
        REQUIRE(vector.available() == 100);
      }
      THEN("Erase method returns out of bounds status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(90) == PF::IndexStatus::out_of_bounds);
      }

      THEN("The empty method reports that the vector is empty") { REQUIRE(vector.empty() == true); }
    }
  }

  GIVEN("Partially filled vector of 100 bytes with capacity 252") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector;
    for (size_t i = 0; i < 100; i++) {
      auto status = vector.push_back(i);
      REQUIRE(status == PF::IndexStatus::ok);
    }
    ByteVector<buffer_size> input_buffer;
    ByteVector<buffer_size> expected;
    for (size_t i = 0; i < 252; i++) {
      input_buffer.push_back(i);
    }

    WHEN("The copy_from method is used on vector for dest index 0") {
      auto status = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 0);
      THEN("The copy_from method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector.full() == true);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector.available() == 0);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for  the input of index 90") {
        REQUIRE(vector.erase(90) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN(
          "The vector has an expected sequence of 252 bytes, data gets copied over initial bytes") {
        for (size_t i = 0; i < 252; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
    WHEN("The copy_from method is called  vector for dest index 101") {
      auto status = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 101);
      THEN("The copy_from method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 152 bytes are avaliable") {
        REQUIRE(vector.available() == 152);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(99) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initial pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
    WHEN("The copy_from method is called on vector  for dest index 101") {
      ByteVector<4> vector0;
      for (size_t i = 0; i < 4; i++) {
        vector0.push_back(i);
      }
      auto status = vector.copy_from(vector0.buffer(), vector0.size(), 101);
      THEN("The copy from method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 147 bytes are avaliable") {
        REQUIRE(vector.available() == 147);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status for with the input of index 90") {
        REQUIRE(vector.erase(150) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initially pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
      THEN("After copy_from method is called the vector has expected sequence of 4 bytes ") {
        REQUIRE(vector.operator[](101) == 0);
        REQUIRE(vector.operator[](102) == 1);
        REQUIRE(vector.operator[](103) == 2);
        REQUIRE(vector.operator[](104) == 3);
      }
    }
    WHEN(
        "The copy_from method is called on vector for dest index 101 where src vector is of "
        "capacity 4") {
      ByteVector<4> vector0;
      for (size_t i = 0; i < 4; i++) {
        vector0.push_back(i);
      }
      auto status = vector.copy_from(vector0.buffer(), vector0.size(), 240);
      THEN("The copy from method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 147 bytes are avaliable") {
        REQUIRE(vector.available() == 8);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(150) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initial pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
      THEN("After copy_from method is called the vector has expected sequence of 4 bytes ") {
        REQUIRE(vector.operator[](240) == 0);
        REQUIRE(vector.operator[](241) == 1);
        REQUIRE(vector.operator[](242) == 2);
        REQUIRE(vector.operator[](243) == 3);
      }
    }
  }

  GIVEN("uint16_t empty vector of capacity 2UL ") {
    constexpr size_t buffer_size = 2UL;
    PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
    PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
    input_buffer.push_back(0xe4aa);
    input_buffer.push_back(0xcbda);
    WHEN(
        "The copy_from method is used on uint16_t vector of buffer size 2, where input vector is "
        "of 2 bytes and dest index is 0") {
      auto status = vector1.copy_from(input_buffer.buffer(), input_buffer.size());
      THEN("copy_from method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The vector has an expected sequence of 2 bytes") {
        REQUIRE(vector1[0] == 0xe4aa);
        REQUIRE(vector1[1] == 0xcbda);
      }
      THEN("After the copy_from method is called, The size method reports size as 2") {
        REQUIRE(vector1.size() == 2);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is  completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector1.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for  the input of index 4") {
        REQUIRE(vector1.erase(4) == PF::IndexStatus::out_of_bounds);
      }
    }

    WHEN("The copy_from method is called on empty vector of capacity 2, for dest index 2 ") {
      constexpr size_t buffer_size = 2UL;
      PF::Util::Containers::Vector<uint16_t, buffer_size> vector;
      auto status = vector.copy_from(input_buffer.buffer(), input_buffer.size(), 2);
      THEN("The copy_from method returns out of bounds") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") { REQUIRE(vector.empty() == true); }
      THEN("The avaliable method reports that 2 bytes are avaliable") {
        REQUIRE(vector.available() == 2);
      }
      THEN("The full method reports that the vector is  not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("Erase method returns out of bounds status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Erase method returns ok status for  the input of index 4") {
        REQUIRE(vector.erase(4) == PF::IndexStatus::out_of_bounds);
      }
    }
  }
}

SCENARIO("Copy_from function for Array works correctly") {
  GIVEN("Empty vector with  capacity 4") {
    auto data = PF::Util::Containers::make_array<uint8_t>(0x0f, 0x36);
    constexpr size_t data_size = 4UL;
    ByteVector<data_size> vector2;

    WHEN("The copy_from method is called vector  and dest index is 0") {
      auto copy = vector2.copy_from(data.data(), data.size(), 0);
      THEN("The copy_from method returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 2") {
        REQUIRE(vector2.size() == 2);
      }
      THEN("The avaliable method reports that 2 bytes are avaliable") {
        REQUIRE(vector2.available() == 2);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector2.full() == false);
      }
      THEN("Erase method returns ok status with the input of index 1") {
        REQUIRE(vector2.erase(1) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status with the input of index 4") {
        REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 2 bytes") {
        REQUIRE(vector2[0] == 0x0f);
        REQUIRE(vector2[1] == 0x36);
      }
    }

    WHEN("The copy_from method is called on vector , and dest index is 1 ") {
      auto copy = vector2.copy_from(data.data(), data.size(), 1);
      THEN("The copy_from method  returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 3") {
        REQUIRE(vector2.size() == 3);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector2.full() == false);
      }
      THEN("The avaliable method reports that 1 bytes are avaliable") {
        REQUIRE(vector2.available() == 1);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector2.empty() == false);
      }
      THEN("Erase method returns ok status with the input of index 1") {
        REQUIRE(vector2.erase(1) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status with the input of index 4") {
        REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 2 bytes") {
        REQUIRE(vector2.operator[](1) == 0x0f);
        REQUIRE(vector2.operator[](2) == 0x36);
      }
    }

    WHEN("The copy_from method is called on vector , and dest index is 6 ") {
      auto copy = vector2.copy_from(data.data(), data.size(), 6);
      THEN("copy_from function returns  out of bounds status") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector2.empty() == true);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector2.full() == false);
      }
      THEN("The avaliable method reports that 4 bytes are avaliable") {
        REQUIRE(vector2.available() == 4);
      }
    }
  }
  GIVEN("Empty uint16_t vector of capacity 4") {
    auto data = PF::Util::Containers::make_array<uint16_t>(0x2b94, 0xbd20);
    constexpr size_t data_size = 4UL;
    PF::Util::Containers::Vector<uint16_t, data_size> vector2;
    WHEN("The copy_from method is called on vector ,for dest index 0 ") {
      auto copy = vector2.copy_from(data.data(), data.size(), 0);
      THEN("copy_from method returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 2") {
        REQUIRE(vector2.size() == 2);
      }
      THEN("The avaliable method reports that 2 bytes are avaliable") {
        REQUIRE(vector2.available() == 2);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector2.full() == false);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector2.empty() == false);
      }
      THEN("Erase method returns ok status with the input of index 1") {
        REQUIRE(vector2.erase(1) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status with the input of index 4") {
        REQUIRE(vector2.erase(4) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 4 bytes") {
        REQUIRE(vector2[0] == 0x2b94);
        REQUIRE(vector2[1] == 0xbd20);
      }
    }

    WHEN("The copy_from method is called on vector,for dest index 4") {
      auto copy = vector2.copy_from(data.data(), data.size(), 4);
      THEN("copy_from method returns out of bounds status") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector2.empty() == true);
      }
      THEN("The avaliable method reports that 4 bytes are avaliable") {
        REQUIRE(vector2.available() == 4);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector2.full() == false);
      }
    }
  }

  GIVEN("Partially filled vector of 100 bytes with capacity 252") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector;
    for (size_t i = 0; i < 100; i++) {
      auto status = vector.push_back(i);
      REQUIRE(status == PF::IndexStatus::ok);
    }
    std::array<uint8_t, 8> input_array = {0x0f, 0x36, 0x08, 0xff, 0x89, 0x01, 0x80, 0x02};

    WHEN("The copy_from method is used on  vector for dest index 0") {
      auto status = vector.copy_from(input_array.data(), input_array.size(), 0);
      THEN("The copy_from method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector.available() == 244);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status for  the input of index 90") {
        REQUIRE(vector.erase(90) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 8 bytes  data gets copied over initial bytes") {
        for (size_t i = 0; i < 8; i++) {
          REQUIRE(vector.operator[](i) == input_array[i]);
        }
      }
    }
    WHEN("The copy_from method is called on  vector for dest index 245") {
      auto status = vector.copy_from(input_array.data(), input_array.size(), 245);
      THEN("The copy_from method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 152 bytes are avaliable") {
        REQUIRE(vector.available() == 152);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(99) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initial pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
  }
}

SCENARIO("The copy_from method for vector works correctly for source bytes") {
  GIVEN("Empty vector of capacity 252") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector1;
    ByteVector<buffer_size> input_buffer;
    ByteVector<buffer_size> expected;
    uint8_t* re = input_buffer.buffer();
    for (size_t i = 0; i < 252; i++) {
      input_buffer.push_back(i);
    }
    expected.push_back(0);
    expected.push_back(0);
    for (size_t i = 0; i < 100; i++) {
      expected.push_back(i);
    }

    WHEN("The copy_from method is called vector , for dest index 0 ") {
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("copy_from method returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 252") {
        REQUIRE(vector1.size() == 252);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("Erase method returns out of bounds status for the input of index 252") {
        REQUIRE(vector1.erase(252) == PF::IndexStatus::out_of_bounds);
      }
      THEN("Erase method returns ok status for the input of index 251") {
        REQUIRE(vector1.erase(251) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("The vector has an expected sequence of 252 bytes") {
        for (int i = 0; i < 252; i++) {
          REQUIRE(vector1[i] == i);
        }
      }
    }

    WHEN("The copy_from method is called on vector, for dest index 3 ") {
      auto status = vector1.copy_from(re, buffer_size, 3);
      THEN("copy_from method  returns out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector1.empty() == true);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The avaliable method reports that 252 bytes are avaliable") {
        REQUIRE(vector1.available() == 252);
      }
    }

    WHEN("The copy_from method is called on empty vector of capacity 100, for dest index 2 ") {
      constexpr size_t buffer_size = 100UL;
      ByteVector<buffer_size> input_buffer;
      ByteVector<102> value;

      for (size_t i = 0; i < 100; i++) {
        input_buffer.push_back(i);
      }
      value.push_back(0);
      value.push_back(0);
      for (size_t i = 0; i < 100; i++) {
        value.push_back(i);
      }
      uint8_t* re = input_buffer.buffer();
      ByteVector<252> vector1;

      auto copy = vector1.copy_from(re, buffer_size, 2);
      THEN("copy_from method returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 102 ") {
        REQUIRE(vector1.size() == 102);
      }
      THEN("The avaliable method reports that 150 bytes are avaliable") {
        REQUIRE(vector1.available() == 150);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("Erase method returns ok status with the input of index 100") {
        REQUIRE(vector1.erase(100) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status with the input of index 251") {
        REQUIRE(vector1.erase(251) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 102 bytes ") {
        for (int i = 0; i < 102; i++) {
          REQUIRE(vector1.operator[](i) == value.operator[](i));
        }
      }
    }

    WHEN(
        "The copy_from method is called on empty uint16_t vector of capacity 2, for dest index "
        "0 ") {
      constexpr size_t buffer_size = 2UL;
      PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
      PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
      uint16_t* re = input_buffer.buffer();
      input_buffer.push_back(0x98e7);
      input_buffer.push_back(0x1bdc);
      auto copy = vector1.copy_from(re, buffer_size);
      THEN("copy_from function returns ok status") { REQUIRE(copy == PF::IndexStatus::ok); }
      THEN("After the copy_from method is called, The size method reports size as 2") {
        REQUIRE(vector1.size() == 2);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector1.available() == 0);
      }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector1.full() == true);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
      THEN("Erase method returns ok status with the input of index 1") {
        REQUIRE(vector1.erase(1) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status with the input of index 25") {
        REQUIRE(vector1.erase(25) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The vector has an expected sequence of 2 bytes") {
        REQUIRE(vector1[0] == 0x98e7);
        REQUIRE(vector1[1] == 0x1bdc);
      }
    }
    WHEN(
        "The copy_from method is called on empty uint16_t vector of capacity 2, for dest index 3") {
      constexpr size_t buffer_size = 2UL;
      PF::Util::Containers::Vector<uint16_t, buffer_size> vector1;
      PF::Util::Containers::Vector<uint16_t, buffer_size> input_buffer;
      uint16_t* re = input_buffer.buffer();
      input_buffer.push_back(0x98e7);
      input_buffer.push_back(0x1bdc);
      auto copy = vector1.copy_from(re, buffer_size, 3);
      THEN("copy_from function returns out of bounds status") {
        REQUIRE(copy == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector1.empty() == true);
      }
      THEN("The avaliable method reports that 2 bytes are avaliable") {
        REQUIRE(vector1.available() == 2);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector1.full() == false);
      }
    }
  }

  GIVEN("Partially filled vector of 100 bytes with capacity 252") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector;
    for (size_t i = 0; i < 100; i++) {
      auto status = vector.push_back(i);
      REQUIRE(status == PF::IndexStatus::ok);
    }

    ByteVector<buffer_size> input_buffer;
    ByteVector<buffer_size> expected;
    for (size_t i = 0; i < 252; i++) {
      input_buffer.push_back(i);
    }
    uint8_t* re = input_buffer.buffer();

    WHEN("The copy_from method is used on  vector  for dest index 0") {
      auto status = vector.copy_from(re, input_buffer.size(), 0);
      THEN("The copy_from method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is completely filled") {
        REQUIRE(vector.full() == true);
      }
      THEN("The avaliable method reports that 0 bytes are avaliable") {
        REQUIRE(vector.available() == 0);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for  the input of index 90") {
        REQUIRE(vector.erase(90) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN(
          "The vector has an expected sequence of 252 bytes, data gets copied over initial bytes") {
        for (size_t i = 0; i < 252; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
    WHEN("The copy_from method is called on vector of for dest index 101") {
      auto status = vector.copy_from(re, input_buffer.size(), 101);
      THEN("The copy_from method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 152 bytes are avaliable") {
        REQUIRE(vector.available() == 152);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(99) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initial pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
    }
    WHEN(
        "The copy_from method is called on  vector for dest index 101 and src vector has 4 bytes") {
      ByteVector<4> vector0;
      for (size_t i = 0; i < 4; i++) {
        vector0.push_back(i);
      }
      uint8_t* re = vector0.buffer();
      auto status = vector.copy_from(re, vector0.size(), 101);
      THEN("The copy from method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 147 bytes are avaliable") {
        REQUIRE(vector.available() == 147);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns out of bounds status for with the input of index 90") {
        REQUIRE(vector.erase(150) == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initially pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
      THEN("The vector has expected sequence of 4 bytes after copy_from method is called") {
        REQUIRE(vector.operator[](101) == 0);
        REQUIRE(vector.operator[](102) == 1);
        REQUIRE(vector.operator[](103) == 2);
        REQUIRE(vector.operator[](104) == 3);
      }
    }
    WHEN(
        "The copy_from method is called on  vector  for dest index 240 and src vector has 4 "
        "bytes") {
      ByteVector<4> vector0;
      for (size_t i = 0; i < 4; i++) {
        vector0.push_back(i);
      }
      uint8_t* re = vector0.buffer();
      auto status = vector.copy_from(re, vector0.size(), 240);
      THEN("The copy from method returns ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The full method reports that the vector is not completely filled") {
        REQUIRE(vector.full() == false);
      }
      THEN("The avaliable method reports that 147 bytes are avaliable") {
        REQUIRE(vector.available() == 8);
      }
      THEN("Erase method returns ok status for the input of index 0") {
        REQUIRE(vector.erase(0) == PF::IndexStatus::ok);
      }
      THEN("Erase method returns ok status for with the input of index 90") {
        REQUIRE(vector.erase(150) == PF::IndexStatus::ok);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector.empty() == false);
      }
      THEN("The vector has an expected sequence of 100 bytes initial pushed") {
        for (size_t i = 0; i < 100; i++) {
          REQUIRE(vector.operator[](i) == i);
        }
      }
      THEN("The vector has expected sequence of 4 bytes after copy_from method is called") {
        REQUIRE(vector.operator[](240) == 0);
        REQUIRE(vector.operator[](241) == 1);
        REQUIRE(vector.operator[](242) == 2);
        REQUIRE(vector.operator[](243) == 3);
      }
    }
  }
}

SCENARIO("The function in Vector:Element *buffer() works correctly") {
  GIVEN("Completely  filled vector with buffer size 100UL") {
    constexpr size_t buffer_size = 100UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i < 100; i++) {
      vector1.push_back(i);
    }

    WHEN("Element *buffer() function is used on completely filled vector of buffer size 100") {
      vector1.resize(4);
      auto* buffer = vector1.buffer();
      THEN("The vector has an expected sequence of 5 bytes") {
        auto* result = vector1.begin();
        for (int i = 0; i < 5; i++) {
          REQUIRE(buffer[i] == result[i]);
        }
      }
    }

    WHEN("Vector is resized to 2, Element *buffer() gives correct values") {
      vector1.resize(2);
      auto* buffer = vector1.buffer();
      THEN("Return vector of size 2 as expected data") {
        REQUIRE(buffer[0] == 0);
        REQUIRE(buffer[1] == 1);
      }
    }
  }
}

SCENARIO("The method in vector: erase works correctly") {
  GIVEN("Completely filled vector of 252UL buffer size") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i < 252; i++) {
      vector1.push_back(i);
    }
    WHEN("The erase method is called six times") {
      auto status = vector1.erase(2);
      auto status1 = vector1.erase(50);
      auto status2 = vector1.erase(248);
      auto status3 = vector1.erase(249);
      auto status4 = vector1.erase(251);
      auto status5 = vector1.erase(252);
      THEN("The erase method returns ok status for index 2") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns ok status for index 52") {
        REQUIRE(status1 == PF::IndexStatus::ok);
      }
      THEN("The erase method returns ok status for index 248") {
        REQUIRE(status2 == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds status for index 249") {
        REQUIRE(status3 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method returns out of bounds status for index 251") {
        REQUIRE(status4 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method returns out of bounds status for index 252") {
        REQUIRE(status5 == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 251") {
        REQUIRE(vector1.size() == 249);
      }
      THEN("The avaliable method reports that 1 bytes are avaliable") {
        REQUIRE(vector1.available() == 3);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
    }
    WHEN("the erase method is called with thrice on bytes 6, 11,250 , 252 and 256 ") {
      auto status = vector1.erase(5);
      THEN("The erase method reports ok status.") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The erase method reports ok status with the input of index 10") {
        auto status = vector1.erase(10);
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method reports ok status with the input of index 252") {
        auto status = vector1.erase(249);
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method reports out of bounds status for the input of index 252") {
        auto status = vector1.erase(251);
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method reports out of bounds status with the input of index 256") {
        auto status = vector1.erase(256);
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 251") {
        REQUIRE(vector1.size() == 251);
      }
      THEN("The avaliable method reports that 1 bytes are avaliable") {
        REQUIRE(vector1.available() == 1);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is non-empty") {
        REQUIRE(vector1.empty() == false);
      }
    }
  }

  GIVEN("Partially filled vector of 252 bytes with capacity") {
    constexpr size_t buffer_size = 256UL;
    ByteVector<buffer_size> vector1;
    for (size_t i = 0; i < 252; i++) {
      vector1.push_back(i);
    }
    WHEN("the erase method is called thrice") {
      auto status = vector1.erase(251);
      auto status2 = vector1.erase(251);
      auto status1 = vector1.erase(256);
      THEN("The erase method returns ok status with the input of index 251") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("The erase method returns out of bounds status with the input of index 251") {
        REQUIRE(status2 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The erase method returns out of bounds status with the input of index 256") {
        REQUIRE(status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("After the erase method is called, The size method reports size as 251") {
        REQUIRE(vector1.size() == 251);
      }
      THEN("The avaliable method reports that 5 bytes are avaliable") {
        REQUIRE(vector1.available() == 5);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
      THEN("The empty method reports that the vector is not empty") {
        REQUIRE(vector1.empty() == false);
      }
    }
  }

  GIVEN("Empty vector of buffer size 252UL") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector1;
    WHEN("Empty vector of size 252UL is passed as input for erase method") {
      auto erase = vector1.erase(0);
      THEN("After erase method, it returns out of bounds with the input of index 0") {
        REQUIRE(erase == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector1.empty() == true);
      }
      THEN("The avaliable method reports that 5 bytes are avaliable") {
        REQUIRE(vector1.available() == 252);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
    }
  }
  GIVEN("Vector containing 1 byte") {
    constexpr size_t buffer_size = 252UL;
    ByteVector<buffer_size> vector1;
    vector1.push_back(1);
    WHEN("Vector containing only 1 byte is passed as an input for erase method") {
      auto status = vector1.erase(0);
      auto status1 = vector1.erase(1);
      THEN("After erase method, it returns ok with the input of index 0") {
        REQUIRE(status == PF::IndexStatus::ok);
      }
      THEN("After erase method, it returns out of bounds with the input of index 1") {
        REQUIRE(status1 == PF::IndexStatus::out_of_bounds);
      }
      THEN("The empty method reports that the vector is empty") {
        REQUIRE(vector1.empty() == true);
      }
      THEN("The avaliable method reports that 5 bytes are avaliable") {
        REQUIRE(vector1.available() == 252);
      }
      THEN("The full method reports that the vector is not-completely filled") {
        REQUIRE(vector1.full() == false);
      }
    }
  }
}
