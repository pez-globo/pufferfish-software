/*
 * RingBuffer.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 *
 * */
#include "Pufferfish/Util/Containers/RingBuffer.h"

#include <cstring>
#include <iostream>

#include "catch2/catch.hpp"
namespace PF = Pufferfish;
// newest: ^
// oldest: *

SCENARIO("Non-Volatile RingBuffer works correctly") {
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 0 bytes were poped") {
    // internal state should look like [0*] [1] [2] [3^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto peek_status = test.peek(val1, i);
      REQUIRE(peek_status == PF::BufferStatus::ok);
      REQUIRE(val1 == i);
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(2);
      THEN("The push method returns out of bounds status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After push method ,the peek method returns expected data ") {
        ////internal state should look like [0*] [1] [2] [3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.peek(val1, i);
          REQUIRE(peek_status == PF::BufferStatus::ok);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t result = 0;
      uint8_t& pop = result;
      auto pop_status = test.pop(pop);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [1*] [2] [3^]
        uint8_t re = 0;
        uint8_t& val = re;
        auto peek_status = test.peek(val, 0);
        REQUIRE(val == 1);
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 1);
        REQUIRE(val1 == 2);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 1 byte was "
      "poped") {
    // internal state should look like [] [1*] [2] [3^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    REQUIRE(val == 0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    REQUIRE(val1 == 1);
    REQUIRE(val2 == 2);
    REQUIRE(val3 == 3);

    WHEN("1 byte of data is pushed") {
      auto size = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      THEN("After push method, the peek method returns expected data ") {
        // internal state should look like [0^] [1*] [2] [3]
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
        REQUIRE(val3 == 3);
        REQUIRE(val4 == 0);
      }
    }
    WHEN("2 bytes of data are poped") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [][][][3*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 2 byte were poped ") {
    // internal state should look like [][][2*][3^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    test.peek(val1, 0);
    test.peek(val2, 1);
    REQUIRE(val1 == 2);
    REQUIRE(val2 == 3);

    WHEN("1 byte of data is pushed ") {
      auto push_status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0^][][2*][3]
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
        REQUIRE(val3 == 0);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 2);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
}
SCENARIO("The pop method in Non-Volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 3 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [][][][3*^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
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

    WHEN("3 bytes of data are pushed ") {
      for (size_t i = 0; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0] [1] [2^] [3*]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 3);
        REQUIRE(val2 == 0);
        REQUIRE(val3 == 1);
        REQUIRE(val4 == 2);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 0 byte were poped") {  // [0^][][][][4*]
    // internal state should look like [0*] [1] [2^] []
    PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    REQUIRE(val1 == 0);
    REQUIRE(val2 == 1);
    REQUIRE(val3 == 2);
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.peek(val, i);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("4 bytes of data are poped ") {
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
      THEN("The pop method returns ok status for 3rd time") {
        REQUIRE(status2 == PF::BufferStatus::ok);
      }
      auto status3 = test.pop(val);
      THEN("The pop method returns empty status for 4th time") {
        REQUIRE(status3 == PF::BufferStatus::empty);
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 1 bytes was poped ") {  // [0*][1][2^][][]
    // internal state should look like [] [1*] [2^] []
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t result = 0;
    uint8_t& pop = result;
    test.pop(pop);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    test.peek(val1, 0);
    test.peek(val2, 1);
    REQUIRE(val1 == 1);
    REQUIRE(val2 == 2);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [4^] [1*] [2] [3]
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
        REQUIRE(val3 == 3);
        REQUIRE(val4 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
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
}
SCENARIO("The push method in Non-Volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [][][2*^][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
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

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][][2*][3^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.peek(val1, 0);
        test.peek(val2, 1);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
      }
    }
    WHEN("2 bytes of data are poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 0 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [0*] [1^] [] []
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re1 = 0;
      uint8_t& val1 = re1;
      REQUIRE(test.peek(val1, i) == PF::BufferStatus::ok);
      REQUIRE(val1 == i);
    }

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 2; i < 4; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*][1][2][3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.peek(val, i);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][1*^][][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped") {  // [0][1^][][][4*]
    // internal state should look like [][1*^][][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
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

    WHEN("3 bytes of data are pushed ") {
      for (size_t i = 2; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [4^][1*][2][3]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
        REQUIRE(val3 == 3);
        REQUIRE(val4 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly for differfent combinations") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 1 bytes of data were pushed and 0 byte was poped") {  // [0^][1*][][][]
    // internal state should look like [0*^][] [] []
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    auto status = test.push(0);
    REQUIRE(status == PF::BufferStatus::ok);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 1; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0*][1] [2^] []
        for (size_t i = 0; i < 3; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.peek(val, i);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 3 byte was poped and "
      "1 byte was pushed") {
    // internal state should look like [0^][][][3*]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    test.peek(val1, 0);
    test.peek(val2, 1);
    REQUIRE(val1 == 3);
    REQUIRE(val2 == 0);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0][1^][][3*]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        REQUIRE(val1 == 3);
        REQUIRE(val2 == 0);
        REQUIRE(val3 == 1);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
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
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {
    // internal state should look like [4^][1*][2][3]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    for (size_t i = 2; i < 5; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    test.peek(val4, 3);
    REQUIRE(val1 == 1);
    REQUIRE(val2 == 2);
    REQUIRE(val3 == 3);
    REQUIRE(val4 == 4);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [4^][1*][2][3]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
        REQUIRE(val3 == 3);
        REQUIRE(val4 == 4);
      }
    }
    WHEN("3 bytes of data are poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [4*^][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
}
SCENARIO("The pop and push method in Non-Volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped and "
      "2 bytes were pushed") {  //
    // internal state should look like [4^][][2*][3]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 3; i < 5; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    REQUIRE(val1 == 2);
    REQUIRE(val2 == 3);
    REQUIRE(val3 == 4);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [4][1^][2*][3]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
        REQUIRE(val3 == 4);
        REQUIRE(val4 == 1);
      }
    }
    WHEN("2 bytes of data are poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [4*^][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed 2 bytes were poped and 3 "
      "were pushed") {  // [0*][1][2][3][4^]
    // internal state should look like [4] [5^] [2*] [3]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 3; i < 6; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    test.peek(val4, 3);
    REQUIRE(val1 == 2);
    REQUIRE(val2 == 3);
    REQUIRE(val3 == 4);
    REQUIRE(val4 == 5);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [4] [5^] [2*] [3]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
        REQUIRE(val3 == 4);
        REQUIRE(val4 == 5);
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][5*^][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 5);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly for peek method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "bytes were pushed ") {  // [0*][1][2][3^][]
    // internal state should look like [0][1][2^][3*]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    test.push(0);
    for (size_t i = 1; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    test.peek(val1, 0);
    test.peek(val2, 1);
    test.peek(val3, 2);
    test.peek(val4, 3);
    REQUIRE(val1 == 3);
    REQUIRE(val2 == 0);
    REQUIRE(val3 == 1);
    REQUIRE(val4 == 2);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0][1][2^][3*]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        test.peek(val4, 3);
        REQUIRE(val1 == 3);
        REQUIRE(val2 == 0);
        REQUIRE(val3 == 1);
        REQUIRE(val4 == 2);
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [][][2*^][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 2);
      }
    }
  }
}

SCENARIO("Volatile RingBuffer works correctly") {
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 0 bytes were poped") {
    // internal state should look like [0*] [1] [2] [3^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("Aftre pop method, it returns expected data") {
          // internal state should look like [0*] [1] [2] [3^]
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 bytes of data is pushed") {
      auto status = test.push(2);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After push method ,the pop method returns expected data ") {
        ////internal state should look like [0*] [1] [2] [3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.pop(val1);
          REQUIRE(peek_status == PF::BufferStatus::ok);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [] [] [] [3*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 1 byte was "
      "poped") {
    // internal state should look like [] [1*] [2] [3^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    REQUIRE(val == 0);
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 1; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, ite returns expected data") {
          // internal state should look like [] [1*] [2] [3^]
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is pushed ") {
      auto size = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      THEN("After push method, the pop method returns expected data ") {
        // internal state should look like [4^] [1*] [2] [3]
        for (size_t i = 1; i < 5; i++) {
          uint8_t re1 = 0;
          uint8_t& val2 = re1;
          test.pop(val2);
          REQUIRE(val2 == i);
        }
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto pop_status = test.pop(val1);
        THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns expected data") {
        // internal state should look like [][][][3*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 2 byte were poped ") {
    // internal state should look like [][][2*][3^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      test.push(i);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 2; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, it returns expected data") {
          // internal state should look like [][][2*][3^]
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto push_status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, pop method returns expected data") {
        // internal state should look like [0^][][2*][3]
        for (size_t i = 2; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          test.pop(val);
          REQUIRE(val == i);
        }
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.pop(val1);
        REQUIRE(val1 == 0);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 2);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] [3*^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }
  }
}
SCENARIO("The pop method in volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 3 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [][][][3*^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    WHEN("The peek method is called") {
      uint8_t re1 = 0;
      uint8_t& val1 = re1;
      THEN("The peek method returns expected sequence of data") {
        REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
      }
    }

    WHEN("3 bytes of data is pushed  ") {
      for (size_t i = 0; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [0] [1] [2^] [3*]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.pop(val);
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        REQUIRE(val == 3);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
        REQUIRE(val3 == 2);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 0 byte were poped") {  // [0^][][][][4*]
    // internal state should look like [0*] [1] [2^] []
    volatile PF::Util::Containers::RingBuffer<6, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, it returns expected data initially given") {
          // internal state should look like [0*] [1] [2^] []
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [0*] [1] [2] [3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("4 bytes of data is poped ") {
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
      THEN("The pop method returns ok status for 3rd time") {
        REQUIRE(status2 == PF::BufferStatus::ok);
      }
      auto status3 = test.pop(val);
      THEN("The pop method returns empty status for 4th time") {
        REQUIRE(status3 == PF::BufferStatus::empty);
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [] [] []
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 1 bytes was poped ") {  // [0*][1][2^][][]
    // internal state should look like [] [1*] [2^] []
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    test.pop(val2);
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 1; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, it returns expected data initially given") {
          // internal state should look like [] [1*] [2^] []
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("2 bytes of data is pushed ") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [4^] [1*] [2] [3]
        for (size_t i = 1; i < 5; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
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
}
SCENARIO("The push method in volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [][][2*^][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    WHEN("1 byte of data is peeked") {
      uint8_t re1 = 0;
      uint8_t& val1 = re1;
      REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
      THEN("The peek method returns expected sequence of data") { REQUIRE(val1 == 2); }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [][][2*][3^]
        for (size_t i = 2; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 0 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [0*] [1^] [] []
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    WHEN("2 bytes of data is poped") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, ite returns expected data initially given") {
          // internal state should look like [0*] [1^] [] []
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("2 bytes of data is pushed ") {
      for (size_t i = 2; i < 4; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [0*][1][2][3^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][1*^][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped") {  // [0][1^][][][4*]
    // internal state should look like [][1*^][][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    WHEN("1 byte of data is peeked ") {
      uint8_t re1 = 0;
      uint8_t& val1 = re1;
      REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
      THEN("The peek method returns expected sequence of data") {
        // internal state should look like [][1*^][][]
        REQUIRE(val1 == 1);
      }
    }

    WHEN("3 bytes of data is pushed ") {
      for (size_t i = 2; i < 5; i++) {
        auto status = test.push(i);

        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [4^][1*][2][3]
        for (size_t i = 1; i < 5; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.pop(val);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for differfent combinations") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 1 bytes of data were pushed and 0 byte was poped") {  // [0^][1*][][][]
    // internal state should look like [0*^][] [] []
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    auto status = test.push(0);
    REQUIRE(status == PF::BufferStatus::ok);
    WHEN("1 byte of data is peeked ") {
      uint8_t re1 = 0;
      uint8_t& val1 = re1;
      REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
      THEN("The peek method returns expected sequence of data") {
        // internal state should look like [0*^][][][]
        REQUIRE(val1 == 0);
      }
    }

    WHEN("2 bytes of data is pushed ") {
      for (size_t i = 1; i < 3; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After push method, pop method returns expected data") {
        // internal state should look like [0*][1][2^] []
        for (size_t i = 0; i < 3; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.pop(val);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the peek method returns sequence of data in RingBuffer") {
        // internal state should look like [][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 3 byte was poped and "
      "1 byte was pushed") {
    // internal state should look like [0^][][][3*]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    uint8_t re = 0;
    uint8_t& val = re;
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    test.push(0);
    WHEN("2 bytes of data is poped ") {
      test.pop(val);
      test.pop(val1);
      THEN("Aftre pop method, it returns expected data initially given") {
        // internal state should look like [0^][][][3*]
        REQUIRE(val == 3);
        REQUIRE(val1 == 0);
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, pop method returns expected data") {
        // internal state should look like [0][1^][][3*]
        test.pop(val);
        test.pop(val1);
        test.pop(val2);
        REQUIRE(val == 3);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
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
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {
    // internal state should look like [4^][1*][2][3]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    for (size_t i = 2; i < 5; i++) {
      test.push(i);
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 1; i < 5; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, ite returns expected data initially given") {
          // internal state should look like [4^][1*][2][3]
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After push method, pop method returns expected data") {
        // internal state should look like [4^][1*][2][3]
        for (size_t i = 1; i < 5; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [4*^][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
}
SCENARIO("The pop and push method in volatile ringbuffer works correctly") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped and "
      "2 bytes were pushed") {  //
    // internal state should look like [4^][][2*][3]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 3; i < 5; i++) {
      test.push(i);
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 2; i < 5; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("After pop method, it returns expected data initially given") {
          // internal state should look like [4^][][2*][3]
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [4][5^][2*][3]
        for (size_t i = 2; i < 6; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [4*^][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed 2 bytes were poped and 3 "
      "were pushed") {  // [0*][1][2][3][4^]
    // internal state should look like [4] [5^] [2*] [3]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 3; i < 6; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }

    WHEN("3 bytes of data is poped ") {
      for (size_t i = 2; i < 5; i++) {
        uint8_t re = 0;
        uint8_t& val1 = re;
        test.pop(val1);
        THEN("Aftre pop method, ite returns expected data initially given") {
          // internal state should look like [4][5^][2*][3]
          REQUIRE(val1 == i);
        }
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [4] [5^] [2*] [3]
        for (size_t i = 2; i < 5; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][5*^][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 5);
      }
    }
  }
}
SCENARIO("volatile RingBuffer works correctly for peek method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "bytes were pushed ") {  // [0*][1][2][3^][]
    // internal state should look like [0][1][2^][3*]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    uint8_t re = 0;
    uint8_t& val = re;
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    test.push(0);
    for (size_t i = 1; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }

    WHEN("4 bytes of data is poped ") {
      test.pop(val);
      test.pop(val1);
      test.pop(val2);
      test.pop(val3);
      THEN("Aftre pop method, ite returns expected data initially given") {
        // internal state should look like [0] [1] [2^] [3*]
        REQUIRE(val == 3);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
        REQUIRE(val3 == 2);
      }
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns full status") { REQUIRE(status == PF::BufferStatus::full); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [0][1][2^][3*]
        test.pop(val);
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        REQUIRE(val == 3);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
        REQUIRE(val3 == 2);
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [][][2*^][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
      }
    }
  }
}
