/*
 * RingBuffer.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 *
 * */
#pragma once

#include "Pufferfish/Util/Containers/RingBuffer.h"

#include <cstring>
#include <iostream>

#include "catch2/catch.hpp"
namespace PF = Pufferfish;
// newest: ^
// oldest: *

SCENARIO("Volatile RingBuffer works correctly") {
  GIVEN("A RingBuffer with capacity 6 to which two bytes of data were pushed") {
    // internal state should look like [0*] [1^] [] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    test.push(1);
    WHEN("1 bytes of data is pushed in RingBuffer") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method ,the peek method returns expected data ") {
        ////internal state should look like [0*] [1] [2^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
    WHEN("1 bytes of data is poped from RingBuffer") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [1*^] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which three bytes of data were pushed and 1 byte were "
      "poped") {  //[][1*][2^][][]
    // internal state should look like [] [1*] [2^] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    WHEN("2 bytes of data are pushed in RingBuffer") {
      for (size_t i = 3; i < 5; i++) {
        auto size = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      }

      THEN("After push method, the peek method returns expected data ") {
        // internal state should look like [] [1*] [2] [3] [4^]
        uint8_t re1 = 0;
        uint8_t& val2 = re1;
        test.peek(val2);
        REQUIRE(val2 == 1);
      }
    }
    WHEN("2 bytes of data are poped from RingBuffer") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which three bytes of data were pushed and 2 byte were poped "
      "and one byte was pushed") {  //[][][2*][3^][]
                                    // internal state should look like [] [] [2*] [3^] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(3);

    WHEN("The push method is called on ringbuffer once") {
      auto push_status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [] [] [2*] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        REQUIRE(test.peek(val) == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called on ringbuffer once") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 2);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 3 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [] [] [] [3*] [4^]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("The push method is used on ringbuffer thrice ") {
      for (size_t i = 0; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [3*] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 3);
      }
    }
    WHEN("The pop method is used once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] [4*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte were poped and "
      "1 byte was pushed") {  // [0^][][][][4*]
    // internal state should look like [0^] [] [] [] [4*]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);
    WHEN("The push method is called on RingBuffer thrice") {
      for (size_t i = 1; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is called thrice") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status for 1st time") {
        REQUIRE(status == PF::BufferStatus::ok);
      }
      auto status1 = test.pop(val);
      THEN("The pop method returns ok status for 2nd time") {
        REQUIRE(status1 == PF::BufferStatus::ok);
      }
      auto status2 = test.pop(val);
      THEN("The pop method returns empty status for 3rd time") {
        REQUIRE(status2 == PF::BufferStatus::empty);
      }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 3 bytes of data were pushed ") {  // [0*][1][2^][][]
    // internal state should look like [0*] [1] [2^] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      auto status1 = test.push(6);
      THEN("The push method returns full status for 6th byte") {
        REQUIRE(status1 == PF::BufferStatus::full);
      }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [1*] [2^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 4 bytes of data were pushed and 1 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [] [1*] [2] [3^] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [] [1*] [2] [3^] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is called twice") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 2 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [] [] [2*] [3] [4^]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 0; i < 2; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1^] [2*] [3] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte were poped and "
      "2 bytes were pushed") {  // [0][1^][][][4*]
    // internal state should look like [0] [1^] [] [] [4*]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is used thrice") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status")
        REQUIRE(status == PF::BufferStatus::ok);
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 2 bytes of data were pushed and 1 byte was poped and "
      "1 bytes were pushed") {  // [0^][1*][][][]
    // internal state should look like [0^] [1*] [] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 2; i < 4; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After push method, peek method returns expected data") {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [0*^] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 3 bytes of data were pushed and 2 byte was poped and "
      "2 bytes were pushed") {  // [0][1^][2*][][]
    // internal state should look like [0] [1^] [2*] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2*] [3^] []
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [0*] [1^] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 4 bytes of data were pushed and 3 byte was poped and "
      "3 bytes were pushed") {  // [0][1][2^][3*][]
    // internal state should look like [0] [1] [2^] [3*] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("The push method is called on RingBuffer twice") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      auto status1 = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status1 == PF::BufferStatus::full); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 3);
      }
    }
    WHEN("The pop method is called thrice") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [2*^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte was poped and "
      "4 bytes were pushed") {  // [0][1][2][3^][4*]
    // internal state should look like [0] [1] [2] [3^] [4*]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2] [3^] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is used five times") {
      for (size_t i = 0; i < 5; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 5 bytes of data were pushed") {  // [0*][1][2][3][4^]
    // internal state should look like [0*] [1] [2] [3] [4^]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    for (size_t i = 1; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is used four times") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] [4*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 4 bytes of data were pushed ") {  // [0*][1][2][3^][]
    // internal state should look like [0*] [1] [2] [3^] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    for (size_t i = 1; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is used thrice") {
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {  // [][1*][2][3][4^]
    // internal state should look like [] [1*] [2] [3] [4^]
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    for (size_t i = 2; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("The peek method, returns expected data") {
        // internal state should look like [0^] [1*] [2] [3] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is used twice") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 6 to which 3 bytes of data were pushed and 2 byte was poped and "
      "1 bytes was pushed") {  // [0^][1][2*][][]
    // internal state should look like [0^] [1] [2*] [] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2*] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [0^] [1*] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
}

SCENARIO("Non-volatile RingBuffer works correctly") {
  GIVEN("A RingBuffer with capacity 6 to which two bytes of data were pushed") {
    // internal state should look like [0*] [1^] [] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    test.push(1);
    WHEN("1 bytes of data is pushed in RingBuffer") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method ,the peek method returns expected data ") {
        ////internal state should look like [0*] [1] [2^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
    WHEN("1 bytes of data is poped from RingBuffer") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [1*^] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which three bytes of data were pushed and 1 byte were "
      "poped") {  //[][1*][2^][][]
    // internal state should look like [] [1*] [2^] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    WHEN("2 bytes of data are pushed in RingBuffer") {
      for (size_t i = 3; i < 5; i++) {
        auto size = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      }

      THEN("After push method, the peek method returns expected data ") {
        // internal state should look like [] [1*] [2] [3] [4^]
        uint8_t re1 = 0;
        uint8_t& val2 = re1;
        test.peek(val2);
        REQUIRE(val2 == 1);
      }
    }
    WHEN("2 bytes of data are poped from RingBuffer") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which three bytes of data were pushed and 2 byte were poped "
      "and one byte was pushed") {  //[][][2*][3^][]
                                    // internal state should look like [] [] [2*] [3^] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(3);

    WHEN("The push method is called on ringbuffer once") {
      auto push_status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [] [] [2*] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        REQUIRE(test.peek(val) == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called on ringbuffer once") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 2);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 3 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [] [] [] [3*] [4^]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("The push method is used on ringbuffer thrice ") {
      for (size_t i = 0; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [3*] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 3);
      }
    }
    WHEN("The pop method is used once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] [4*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte were poped and "
      "1 byte was pushed") {  // [0^][][][][4*]
    // internal state should look like [0^] [] [] [] [4*]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);
    WHEN("The push method is called on RingBuffer thrice") {
      for (size_t i = 1; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is called thrice") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status for 1st time") {
        REQUIRE(status == PF::BufferStatus::ok);
      }
      auto status1 = test.pop(val);
      THEN("The pop method returns ok status for 2nd time") {
        REQUIRE(status1 == PF::BufferStatus::ok);
      }
      auto status2 = test.pop(val);
      THEN("The pop method returns empty status for 3rd time") {
        REQUIRE(status2 == PF::BufferStatus::empty);
      }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 3 bytes of data were pushed ") {  // [0*][1][2^][][]
    // internal state should look like [0*] [1] [2^] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      auto status1 = test.push(6);
      THEN("The push method returns full status for 6th byte") {
        REQUIRE(status1 == PF::BufferStatus::full);
      }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [1*] [2^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 4 bytes of data were pushed and 1 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [] [1*] [2] [3^] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [] [1*] [2] [3^] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is called twice") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 2 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [] [] [2*] [3] [4^]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 0; i < 2; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1^] [2*] [3] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte were poped and "
      "2 bytes were pushed") {  // [0][1^][][][4*]
    // internal state should look like [0] [1^] [] [] [4*]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is used thrice") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status")
        REQUIRE(status == PF::BufferStatus::ok);
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 2 bytes of data were pushed and 1 byte was poped and "
      "1 bytes were pushed") {  // [0^][1*][][][]
    // internal state should look like [0^] [1*] [] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 2; i < 4; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After push method, peek method returns expected data") {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [0*^] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 3 bytes of data were pushed and 2 byte was poped and "
      "2 bytes were pushed") {  // [0][1^][2*][][]
    // internal state should look like [0] [1^] [2*] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2*] [3^] []
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [0*] [1^] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 4 bytes of data were pushed and 3 byte was poped and "
      "3 bytes were pushed") {  // [0][1][2^][3*][]
    // internal state should look like [0] [1] [2^] [3*] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 3; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("The push method is called on RingBuffer twice") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      auto status1 = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status1 == PF::BufferStatus::full); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 3);
      }
    }
    WHEN("The pop method is called thrice") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [2*^] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 5 bytes of data were pushed and 4 byte was poped and "
      "4 bytes were pushed") {  // [0][1][2][3^][4*]
    // internal state should look like [0] [1] [2] [3^] [4*]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2] [3^] [4*]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 4);
      }
    }
    WHEN("The pop method is used five times") {
      for (size_t i = 0; i < 5; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 5 bytes of data were pushed") {  // [0*][1][2][3][4^]
    // internal state should look like [0*] [1] [2] [3] [4^]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    for (size_t i = 1; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is used four times") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [] [4*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN("A RingBuffer with capacity 6 to which 4 bytes of data were pushed ") {  // [0*][1][2][3^][]
    // internal state should look like [0*] [1] [2] [3^] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    test.push(0);
    for (size_t i = 1; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 0);
      }
    }
    WHEN("The pop method is used thrice") {
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 6 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {  // [][1*][2][3][4^]
    // internal state should look like [] [1*] [2] [3] [4^]
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    for (size_t i = 2; i < 5; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("The push method is called on RingBuffer once") {
      auto status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("The peek method, returns expected data") {
        // internal state should look like [0^] [1*] [2] [3] [4]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
      }
    }
    WHEN("The pop method is used twice") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*] [4^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 6 to which 3 bytes of data were pushed and 2 byte was poped and "
      "1 bytes was pushed") {  // [0^][1][2*][][]
    // internal state should look like [0^] [1] [2*] [] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 2);

    WHEN("The push method is called on RingBuffer twice") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2*] [3] [4^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.peek(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 2);
      }
    }
    WHEN("The pop method is called once") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [0^] [1*] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
  }
}
