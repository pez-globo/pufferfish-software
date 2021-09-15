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

SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 0") {
  GIVEN("A RingBuffer with capacity 4 to which 0 bytes of data were pushed, 0 bytes were poped") {
    // internal state should look like [*^] [] [] [] []
    PF::Util::Containers::RingBuffer<5, uint8_t> test;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method ,the peek method returns expected data ") {
        ////internal state should look like [0*] [^] [] [][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t result = 0;
      uint8_t& pop = result;
      auto pop_status = test.pop(pop);
      THEN("The pop method returns empty status") {
        REQUIRE(pop_status == PF::BufferStatus::empty);
      }
      THEN("The peek method returns empty on all indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.peek(val1, i);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
      // internal state should look like [*^] [] [] [][]
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 1 bytes of data were pushed and 0 byte was "
      "poped") {
    // internal state should look like [0*] [^] [] [][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    test.push(0);
    uint8_t re = 0;
    uint8_t& val = re;
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    WHEN("1 byte of data is pushed") {
      auto size = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      THEN("After push method, the peek method returns expected data ") {
        // internal state should look like [0*] [1] [^] [][]
        test.peek(val, 0);
        test.peek(val1, 1);
        REQUIRE(val == 0);
        REQUIRE(val1 == 1);
      }
    }
    WHEN("1 byte of data is poped") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns empty on every indices") {
        // internal state should look like [] [*^] [] [][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 0 byte were poped ") {
    // internal state should look like [0*][1][^][][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    test.peek(val1, 0);
    test.peek(val2, 1);
    REQUIRE(val1 == 0);
    REQUIRE(val2 == 1);

    WHEN("1 byte of data is pushed ") {
      auto push_status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0*][1][2][^][]
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.peek(val1, 0);
        test.peek(val2, 1);
        test.peek(val3, 2);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
        REQUIRE(val3 == 2);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 0);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [1*] [^] [][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 0 for pop method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 0 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [0*][1][2][^][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("1 bytes of data are pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [1*] [2] [3] [4][^]
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
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
        REQUIRE(val3 == 2);
        REQUIRE(val4 == 3);
      }
    }
    WHEN("2 byte of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][2*][^][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 0 byte were poped") {  // [0^][][][][4*]
    // internal state should look like [0*] [1] [2] [3][^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
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
    REQUIRE(val1 == 0);
    REQUIRE(val2 == 1);
    REQUIRE(val3 == 2);
    REQUIRE(val4 == 3);
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns the full error code ") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3][^]
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
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }

      THEN("After the pop method,the peek method returns empty on all indices") {
        // internal state should look like [] [] [] [][^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
}

SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 1") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 2 bytes was poped ") {
    // internal state should look like [][*^][][][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t result = 0;
      uint8_t& pop = result;
      test.pop(pop);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::empty);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 0; i < 2; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][0*][1][][]
        test.peek(val1, 0);
        test.peek(val2, 1);
        REQUIRE(val1 == 0);
        REQUIRE(val2 == 1);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      // internal state should look like [][*^][][][]
      THEN("The pop method returns empty status for 1 byte") {
        REQUIRE(status == PF::BufferStatus::empty);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [][1*][^][][]
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

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][1*][2][^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.peek(val1, 0);
        test.peek(val2, 1);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
      }
    }
    WHEN("1 bytes of data are poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns empty on all indices") {
        // internal state should look like [][*^][][][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 1 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [][1*][2][^][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    REQUIRE(val == 0);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1, 0) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    REQUIRE(test.peek(val2, 1) == PF::BufferStatus::ok);
    REQUIRE(val2 == 2);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.peek(val, 0);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
        auto status1 = test.peek(val1, 1);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto status2 = test.peek(val2, 2);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
        auto status3 = test.peek(val3, 3);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val3 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][2*][^][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 2);
      }
    }
  }
}
SCENARIO(
    "Non-Volatile RingBuffer works correctly when oldest index is 1 and for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 1 byte was poped") {  // [0][1^][][][4*]
    // internal state should look like [][1*][2][3][^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
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

    WHEN("1 bytes of data are pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
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
        // internal state should look like [][][2*][3][^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        uint8_t re1 = 0;
        uint8_t& val2 = re1;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {  // [0^][1*][][][]
    // internal state should look like [^][1*][2][3][4]
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
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("2 bytes of data are pushed ") {
      auto status = test.push(5);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.peek(val, 0);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
        auto status1 = test.peek(val1, 1);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto status2 = test.peek(val2, 2);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
        auto status3 = test.peek(val3, 3);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val3 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [^][][2*][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        auto status1 = test.peek(val, 0);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val == 2);
        auto status2 = test.peek(val1, 1);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
        auto status3 = test.peek(val2, 2);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 2") {
  GIVEN("A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 3 byte was poped ") {
    // internal state should look like [][][*^][][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [][][1*][^][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 1);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      THEN("The peek method returns empty on every indices") {
        // internal state should look like [][][*^][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re1 = 0;
          uint8_t& val1 = re1;
          auto status = test.peek(val1, i);
          REQUIRE(status == PF::BufferStatus::empty);
        }
      }
    }
  }

  GIVEN("A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped") {
    // internal state should look like [][][2*][^][]
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
    test.peek(val1, 0);
    REQUIRE(val1 == 2);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [][][2*][3][^]
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
    WHEN("1 bytes of data are poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns empty on every indices") {
        // internal state should look like [][][*^][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.peek(val1);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 2 byte was poped") {  //
    // internal state should look like [][][2*][3][^]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
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
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    test.peek(val1, 0);
    test.peek(val2, 1);
    REQUIRE(val1 == 2);
    REQUIRE(val2 == 3);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][2*][3][4]
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
      }
    }
    WHEN("2 bytes of data are poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the pop method,the peek method returns empty on every indices") {
        // internal state should look like [][][*^][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.peek(val1);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 2 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed 2 bytes were poped and 1 "
      "were pushed") {
    // internal state should look like [^][][2*][3][4]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    auto status = test.push(4);
    REQUIRE(status == PF::BufferStatus::ok);

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
      auto status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0][^][2*][3][4]
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
        REQUIRE(val4 == 0);
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (size_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 3 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the pop method,the peek method returns empty on every indices") {
        // internal state should look like [][][][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val1 = re;
          auto peek_status = test.peek(val1);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 2 bytes were poped and 2 "
      "bytes were pushed ") {
    // internal state should look like [5][^][2*][3][4]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 4; i < 6; i++) {
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
      auto status = test.push(4);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][^][2*][3][4]
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
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 3 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [5*][^][][][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 5);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 3") {
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 4 bytes were poped") {
    // internal state should look like [][][][*^][]
    PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::empty);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][][][3*][^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1, 0);
        REQUIRE(val1 == 3);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      // internal state should look like [][][][*^][]
      THEN("The peek method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t re1 = 0;
          uint8_t& val1 = re1;
          auto status = test.peek(val1, i);
          REQUIRE(status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped") {
    // internal state should look like [][][][3*][^]
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
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.peek(val1, 0);
        REQUIRE(val1 == 3);
        test.peek(val2, 1);
        REQUIRE(val2 == 4);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [][][][*^][]
      THEN("The peek method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          auto status = test.peek(val1, i);
          REQUIRE(status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped 1 "
      "byte was pushed") {
    // internal state should look like [^][][][3*][4]
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
    test.push(4);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 4);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][^][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.peek(val1, 0);
        REQUIRE(val1 == 3);
        test.peek(val2, 1);
        REQUIRE(val2 == 4);
        test.peek(val3, 2);
        REQUIRE(val3 == 5);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(val == 3);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [^][][][][4*]
      THEN("The peek method returns expected data") {
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        auto status = test.peek(val2, 0);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
      }
    }
  }
}
SCENARIO(
    "Non-Volatile RingBuffer works correctly when oldest index is 3 and for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 2 "
      "bytes pushed") {
    // internal state should look like [5][^][][3*][4]
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
    for (size_t i = 4; i < 6; i++) {
      test.push(i);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 4);
    auto peek_status2 = test.peek(val3, 2);
    REQUIRE(peek_status2 == PF::BufferStatus::ok);
    REQUIRE(val3 == 5);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(6);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][6][^][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.peek(val1, 0);
        REQUIRE(val1 == 3);
        test.peek(val2, 1);
        REQUIRE(val2 == 4);
        test.peek(val3, 2);
        REQUIRE(val3 == 5);
        test.peek(val4, 3);
        REQUIRE(val4 == 6);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(val == 3);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [5][^][][][4*]
      THEN("The peek method returns expected data") {
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.peek(val2, 0);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
        auto status1 = test.peek(val3, 1);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val3 == 5);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed") {
    // internal state should look like [5][6][^][3*][4]
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
    for (size_t i = 4; i < 7; i++) {
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
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 4);
    auto peek_status2 = test.peek(val3, 2);
    REQUIRE(peek_status2 == PF::BufferStatus::ok);
    REQUIRE(val3 == 5);
    auto peek_status3 = test.peek(val4, 3);
    REQUIRE(peek_status3 == PF::BufferStatus::ok);
    REQUIRE(val4 == 6);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
        auto peek_status2 = test.peek(val3, 2);
        REQUIRE(peek_status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 5);
        auto peek_status3 = test.peek(val4, 3);
        REQUIRE(peek_status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 6);
      }
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }

      // internal state should look like [][*^][][][]
      THEN("The peek method returns empty for every indices") {
        for (size_t i = 0; i < 4; i++) {
          auto status = test.peek(val1, 0);
          REQUIRE(status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 4") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 1 "
      "pushed and 2 poped") {
    // internal state should look like [][][][][*^]
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
    test.push(4);
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::empty);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][][4*]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      // internal state should look like [][][][][*^]
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      THEN("The peek method returns empty for every indices") {
        for (size_t i = 0; i < 4; i++) {
          auto peek_status = test.peek(val1, i);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 1 "
      "pushed and 1 poped") {
    // internal state should look like [^][][][][4*]
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
    test.push(4);
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][^][][][4*]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns empty at every indices") {
        // internal state should look like [^][][][][*]
        for (size_t i = 0; i < 4; i++) {
          auto peek_status = test.peek(val1, 0);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 2 "
      "pushed and 1 poped") {
    // internal state should look like [5][^][][][4*]
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
    for (size_t i = 4; i < 6; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 5);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(6);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][6][^][][4*]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto peek_status2 = test.peek(val3, 2);
        REQUIRE(peek_status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5*][^][][][]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 5);
      }
    }
  }
}
SCENARIO("Non-Volatile RingBuffer works correctly when oldest index is 4 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed and 1 poped") {
    // internal state should look like [5][6][^][][4*]
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
    for (size_t i = 4; i < 7; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 5);
    auto peek_status2 = test.peek(val3, 2);
    REQUIRE(peek_status2 == PF::BufferStatus::ok);
    REQUIRE(val3 == 6);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(7);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][6][7][^][4*]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto peek_status2 = test.peek(val3, 2);
        REQUIRE(peek_status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
        auto peek_status3 = test.peek(val4, 3);
        REQUIRE(peek_status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 7);
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][6*][^][][]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 6);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed and 1 poped, 1 pushed") {
    // internal state should look like [5][6][7][^][4*]
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
    for (size_t i = 4; i < 7; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    test.push(7);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;
    auto peek_status = test.peek(val1, 0);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 4);
    auto peek_status1 = test.peek(val2, 1);
    REQUIRE(peek_status1 == PF::BufferStatus::ok);
    REQUIRE(val2 == 5);
    auto peek_status2 = test.peek(val3, 2);
    REQUIRE(peek_status2 == PF::BufferStatus::ok);
    REQUIRE(val3 == 6);
    auto peek_status3 = test.peek(val4, 3);
    REQUIRE(peek_status3 == PF::BufferStatus::ok);
    REQUIRE(val4 == 7);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(8);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5][6][7][^][4*]
        auto peek_status = test.peek(val1, 0);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto peek_status1 = test.peek(val2, 1);
        REQUIRE(peek_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto peek_status2 = test.peek(val3, 2);
        REQUIRE(peek_status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
        auto peek_status3 = test.peek(val4, 3);
        REQUIRE(peek_status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 7);
      }
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the push method, peek method returns empty on every indices") {
        // internal state should look like [][][*^][][]
        for (size_t i = 0; i < 4; i++) {
          auto peek_status = test.peek(val1, 0);
          REQUIRE(peek_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////

SCENARIO("Volatile RingBuffer works correctly for index 0") {
  GIVEN("A RingBuffer with capacity 4 to which 0 bytes of data were pushed, 0 bytes were poped") {
    // internal state should look like [*^] [] [] [] []
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(0);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method ,the peek method returns expected data ") {
        ////internal state should look like [0*] [^] [] [][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 0);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t result = 0;
      uint8_t& pop = result;
      auto pop_status = test.pop(pop);
      THEN("The pop method returns empty status") {
        REQUIRE(pop_status == PF::BufferStatus::empty);
      }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [*^] [] [] [] []
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 1 bytes of data were pushed and 0 byte was "
      "poped") {
    // internal state should look like [0*] [^] [] [][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    test.push(0);
    WHEN("1 byte of data is pushed") {
      auto size = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(size == PF::BufferStatus::ok); }
      THEN("After push method, the peek method returns expected data ") {
        // internal state should look like [0*] [1] [^] [][]
        for (size_t i = 0; i < 2; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          test.pop(val);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [*] [^] [] [][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 0 byte were poped ") {
    // internal state should look like [0*][1][^][][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      test.push(i);
    }

    WHEN("1 byte of data is pushed ") {
      auto push_status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(push_status == PF::BufferStatus::ok); }

      THEN("After push method, peek method returns expected data") {
        // internal state should look like [0*][1][2][^][]
        for (size_t i = 0; i < 3; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          test.pop(val);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val1 = re;
      auto pop_status = test.pop(val1);
      REQUIRE(val1 == 0);
      THEN("The pop method returns ok status") { REQUIRE(pop_status == PF::BufferStatus::ok); }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [] [1*] [^] [][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 1);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 0 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 0 byte were poped") {  //[][][][3*][4^]
    // internal state should look like [0*][1][2][^][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
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

    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 0);

    WHEN("1 bytes of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [0*] [1] [2] [3][^]
        uint8_t re = 0;
        uint8_t& val = re;
        test.pop(val);
        REQUIRE(val == 0);
        test.pop(val2);
        REQUIRE(val2 == 1);
        test.pop(val3);
        REQUIRE(val3 == 2);
        test.pop(val4);
        REQUIRE(val4 == 3);
      }
    }

    WHEN("2 byte of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][2*][^][]
        uint8_t re = 0;
        uint8_t& val1 = re;
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 0 byte were poped") {  // [0^][][][][4*]
    // internal state should look like [0*] [1] [2] [3][^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [0*] [1] [2] [3][^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t re = 0;
          uint8_t& val = re;
          auto status = test.pop(val);
          REQUIRE(status == PF::BufferStatus::ok);
          REQUIRE(val == i);
        }
      }
    }
    WHEN("4 bytes of data are poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [*][][][][^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 1") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 2 bytes was poped ") {  // [0*][1][2^][][]
    // internal state should look like [][*^][][][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 2; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t result = 0;
      uint8_t& pop = result;
      test.pop(pop);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    auto peek_status = test.peek(val1);
    REQUIRE(peek_status == PF::BufferStatus::empty);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 0; i < 2; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][0*][1][][]
        for (size_t i = 0; i < 2; i++) {
          test.pop(val1);
          REQUIRE(val1 == i);
        }
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [][*^][][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped") {  // [][1*][2][3^][]
    // internal state should look like [][1*][^][][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
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

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(2);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][1*][2][^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.pop(val1);
        test.pop(val2);
        REQUIRE(val1 == 1);
        REQUIRE(val2 == 2);
      }
    }
    WHEN("1 bytes of data are poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [][*][^][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 1 byte was poped") {  // [][][2*][3][4^]
    // internal state should look like [][1*][2][^][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    REQUIRE(val == 0);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    REQUIRE(test.peek(val1) == PF::BufferStatus::ok);
    REQUIRE(val1 == 1);

    WHEN("2 bytes of data are pushed ") {
      for (size_t i = 3; i < 5; i++) {
        auto status = test.push(i);
        THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.pop(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
        auto status1 = test.pop(val1);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto status2 = test.pop(val2);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
        auto status3 = test.pop(val3);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val3 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the pop method,the peek method returns expected data in RingBuffer") {
        // internal state should look like [][][2*][^][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1);
        REQUIRE(val1 == 2);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 1 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 1 byte was poped") {  // [0][1^][][][4*]
    // internal state should look like [][1*][2][3][^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
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

    WHEN("1 bytes of data are pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        test.pop(val4);
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
        // internal state should look like [][][2*][3][^]
        uint8_t re = 0;
        uint8_t& val1 = re;
        uint8_t re1 = 0;
        uint8_t& val2 = re1;
        auto pop_status = test.pop(val1);
        REQUIRE(pop_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto pop_status1 = test.pop(val2);
        REQUIRE(pop_status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 2 bytes of data were pushed and 1 byte was poped and "
      "3 bytes were pushed") {
    // internal state should look like [^][1*][2][3][4]
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

    WHEN("2 bytes of data are pushed ") {
      auto status = test.push(5);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [^][1*][2][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.pop(val);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val == 1);
        auto status1 = test.pop(val1);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val1 == 2);
        auto status2 = test.pop(val2);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val2 == 3);
        auto status3 = test.pop(val3);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val3 == 4);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(status == PF::BufferStatus::ok);

      THEN("After the pop method,the pop method returns the data in RingBuffer") {
        // internal state should look like [^][][2*][3][4]
        uint8_t re = 0;
        uint8_t& val = re;
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        auto status1 = test.pop(val);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val == 2);
        auto status2 = test.pop(val1);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
        auto status3 = test.pop(val2);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 2") {
  GIVEN("A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 3 byte was poped") {
    // internal state should look like [][][*^][][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 3; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 3; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    WHEN("1 byte of data is pushed ") {
      auto status = test.push(1);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [][][1*][^][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1);
        REQUIRE(val1 == 1);
      }
    }
    WHEN("1 byte of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [][][*^][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }

  GIVEN("A RingBuffer with capacity 4 to which 3 bytes of data were pushed and 2 byte was poped ") {
    // internal state should look like [][][2*][^][]
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
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    test.peek(val1);
    REQUIRE(val1 == 2);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After push method, peek method returns expected data") {
        // internal state should look like [][][2*][3][^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.pop(val1);
        test.pop(val2);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
      }
    }
    WHEN("1 bytes of data are poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [][][*][^][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed and 2 byte was poped and "
      "2 bytes were pushed") {  //
    // internal state should look like [][][2*][3][^]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }

      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [^][][2*][3][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
        REQUIRE(val3 == 4);
      }
    }
    WHEN("2 bytes of data are poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [][][*][][^]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 2 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed 2 bytes were poped and 1 "
      "was pushed") {
    // internal state should look like [^][][2*][3][4]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    auto status = test.push(4);
    REQUIRE(status == PF::BufferStatus::ok);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][^][2*][3][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        test.pop(val4);
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
        THEN("The pop method returns ok status for 3 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("The pop method returns empty on every indices") {
        // internal state should look like [^][][*][][]
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 2 bytes were poped and 2 "
      "bytes were pushed ") {
    // internal state should look like [5][^][2*][3][4]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    for (size_t i = 4; i < 6; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][^][2*][3][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.pop(val1);
        test.pop(val2);
        test.pop(val3);
        test.pop(val4);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
        REQUIRE(val3 == 4);
        REQUIRE(val4 == 5);
      }
    }
    WHEN("3 bytes of data is poped ") {
      for (uint8_t i = 0; i < 3; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 3 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }

      THEN("After the pop method,the peek method returns the data in RingBuffer") {
        // internal state should look like [5*][^][][][]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1);
        REQUIRE(val1 == 5);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 3") {
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 4 bytes were poped") {
    // internal state should look like [][][][*^][]
    volatile PF::Util::Containers::RingBuffer<5, uint8_t> test;
    for (size_t i = 0; i < 4; i++) {
      auto status = test.push(i);
      REQUIRE(status == PF::BufferStatus::ok);
    }
    for (size_t i = 0; i < 4; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    auto peek_status = test.peek(val1);
    REQUIRE(peek_status == PF::BufferStatus::empty);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(3);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][][][3*][^]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        test.peek(val1);
        REQUIRE(val1 == 3);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      // internal state should look like [][][][*^][]
      THEN("The pop method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN("A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped") {
    // internal state should look like [][][][3*][^]
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
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    auto peek_status = test.peek(val1);
    REQUIRE(peek_status == PF::BufferStatus::ok);
    REQUIRE(val1 == 3);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [^][][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        test.pop(val1);
        REQUIRE(val1 == 3);
        test.pop(val2);
        REQUIRE(val2 == 4);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [][][][*][^]
      THEN("The pop method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped 1 "
      "byte was pushed") {
    // internal state should look like [^][][][3*][4]
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
    test.push(4);

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][^][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        test.pop(val1);
        REQUIRE(val1 == 3);
        test.pop(val2);
        REQUIRE(val2 == 4);
        test.pop(val3);
        REQUIRE(val3 == 5);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(val == 3);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [^][][][][4*]
      THEN("The peek method returns expected data") {
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        auto status = test.peek(val2);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 3 for pop and push method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 2 "
      "bytes pushed") {
    // internal state should look like [5][^][][3*][4]
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
    for (size_t i = 4; i < 6; i++) {
      test.push(i);
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(6);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][6][^][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        test.pop(val1);
        REQUIRE(val1 == 3);
        test.pop(val2);
        REQUIRE(val2 == 4);
        test.pop(val3);
        REQUIRE(val3 == 5);
        test.pop(val4);
        REQUIRE(val4 == 6);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      REQUIRE(val == 3);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      // internal state should look like [5][^][][][4*]
      THEN("The peek method returns expected data") {
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        auto status = test.pop(val2);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
        auto status1 = test.pop(val3);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val3 == 5);
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed") {
    // internal state should look like [5][6][^][3*][4]
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
    for (size_t i = 4; i < 7; i++) {
      test.push(i);
    }

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][3*][4]
        uint8_t re1 = 0;
        uint8_t& val1 = re1;
        uint8_t re2 = 0;
        uint8_t& val2 = re2;
        uint8_t re3 = 0;
        uint8_t& val3 = re3;
        uint8_t re4 = 0;
        uint8_t& val4 = re4;
        auto status = test.pop(val1);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val1 == 3);
        auto status1 = test.pop(val2);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 4);
        auto status2 = test.pop(val3);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 5);
        auto status3 = test.pop(val4);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 6);
      }
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }

      // internal state should look like [][][^][*][]
      THEN("The pop method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
SCENARIO("Volatile RingBuffer works correctly for index 4") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 1 "
      "pushed and 2 poped") {
    // internal state should look like [][][][][*^]
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
    test.push(4);
    for (size_t i = 0; i < 2; i++) {
      uint8_t re = 0;
      uint8_t& val = re;
      test.pop(val);
    }

    uint8_t re1 = 0;
    uint8_t& val1 = re1;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(4);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][][4*]
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      // internal state should look like [][][][][*^]
      THEN("The pop method returns empty status") { REQUIRE(status == PF::BufferStatus::empty); }
      THEN("The pop method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 1 "
      "pushed and 1 poped") {
    // internal state should look like [^][][][][4*]
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
    test.push(4);
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(5);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][^][][][4*]
        auto status = test.pop(val1);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto status1 = test.pop(val2);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [^][][][][4*]
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
    }
  }
}

SCENARIO("Volatile RingBuffer works correctly for index 4 for push and pop method") {
  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 2 "
      "pushed and 1 poped") {
    // internal state should look like [5][^][][][4*]
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
    for (size_t i = 4; i < 6; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(6);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][6][^][][4*]
        auto status = test.pop(val1);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto status1 = test.pop(val2);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto status2 = test.pop(val3);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
      }
    }
    WHEN("1 bytes of data is poped ") {
      uint8_t re = 0;
      uint8_t& val = re;
      auto status = test.pop(val);
      THEN("The pop method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [5*][^][][][]
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 5);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed and 1 poped") {
    // internal state should look like [5][6][^][][4*]
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
    for (size_t i = 4; i < 7; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(7);
      THEN("The push method returns ok status") { REQUIRE(status == PF::BufferStatus::ok); }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][6][7][^][4*]
        auto status = test.pop(val1);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto status1 = test.pop(val2);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto status2 = test.pop(val3);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
        auto status3 = test.pop(val4);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 7);
      }
    }
    WHEN("2 bytes of data is poped ") {
      for (size_t i = 0; i < 2; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 2 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the push method, peek method returns expected data") {
        // internal state should look like [][6*][^][][]
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::ok);
        REQUIRE(val1 == 6);
      }
    }
  }

  GIVEN(
      "A RingBuffer with capacity 4 to which 4 bytes of data were pushed, 3 bytes were poped and 3 "
      "pushed and 1 poped, 1 pushed") {
    // internal state should look like [5][6][7][^][4*]
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
    for (size_t i = 4; i < 7; i++) {
      test.push(i);
    }
    uint8_t re = 0;
    uint8_t& val = re;
    test.pop(val);
    test.push(7);

    uint8_t re1 = 0;
    uint8_t& val1 = re1;
    uint8_t re2 = 0;
    uint8_t& val2 = re2;
    uint8_t re3 = 0;
    uint8_t& val3 = re3;
    uint8_t re4 = 0;
    uint8_t& val4 = re4;

    WHEN("1 byte of data is pushed ") {
      auto status = test.push(8);
      THEN("The push method returns the full error code status") {
        REQUIRE(status == PF::BufferStatus::full);
      }
      THEN("After the push method, pop method returns expected data") {
        // internal state should look like [5][6][7][^][4*]
        auto status = test.pop(val1);
        REQUIRE(status == PF::BufferStatus::ok);
        REQUIRE(val1 == 4);
        auto status1 = test.pop(val2);
        REQUIRE(status1 == PF::BufferStatus::ok);
        REQUIRE(val2 == 5);
        auto status2 = test.pop(val3);
        REQUIRE(status2 == PF::BufferStatus::ok);
        REQUIRE(val3 == 6);
        auto status3 = test.pop(val4);
        REQUIRE(status3 == PF::BufferStatus::ok);
        REQUIRE(val4 == 7);
      }
    }
    WHEN("4 bytes of data is poped ") {
      for (size_t i = 0; i < 4; i++) {
        uint8_t re = 0;
        uint8_t& val = re;
        auto status = test.pop(val);
        THEN("The pop method returns ok status for 4 bytes") {
          REQUIRE(status == PF::BufferStatus::ok);
        }
      }
      THEN("After the push method, peek method returns expected data") {
        auto peek_status = test.peek(val1);
        REQUIRE(peek_status == PF::BufferStatus::empty);
      }
      // internal state should look like [][][*^][][]
      THEN("The pop method returns empty on every indices") {
        for (size_t i = 0; i < 4; i++) {
          uint8_t result = 0;
          uint8_t& pop = result;
          auto pop_status = test.pop(pop);
          REQUIRE(pop_status == PF::BufferStatus::empty);
        }
      }
    }
  }
}
