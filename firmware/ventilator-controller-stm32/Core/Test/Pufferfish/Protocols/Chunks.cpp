/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Chunks.cpp
 *
 *  Created on: Nov 3, 2020
 *      Author: Renji Panicker
 *
 * Unit tests to confirm behavior of chunk splitter
 *
 */

#include "Pufferfish/Protocols/Chunks.h"

#include <iostream>

#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/Vector.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

using namespace std::string_literals;

SCENARIO(
    "Protocols::ChunkSplitter correctly handles input chunks of acceptable lengths and "
    "include_delimiter is false",
    "[chunks]") {
  constexpr size_t buffer_size = 256;
  PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks;
  PF::Protocols::ChunkInputStatus status;
  PF::Protocols::ChunkOutputStatus output_status;
  PF::Util::ByteVector<buffer_size> output_buffer;
  bool input_overwritten = false;

  GIVEN("A char chunk splitter with internal buffer equal to 102 bytes") {
    constexpr size_t buffer_size = 102;
    PF::Protocols::ChunkSplitter<buffer_size, char> chunks;

    WHEN(
        "102 non-delimiter bytes are passed as input, and 103rd byte is passed as a delimiter, and "
        "output is called between each input") {
      uint8_t val = 128;
      bool input_overwritten = false;
      PF::Protocols::ChunkInputStatus input_status;

      for (size_t i = 0; i < buffer_size; ++i) {
        input_status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status for all the 102 non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      PF::Util::Vector<char, buffer_size> buffer;
      auto output_status = chunks.output(buffer);
      THEN("After the input of 102 non-delimiter bytes, The output method reports waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }
      THEN("the output buffer is empty") { REQUIRE(output_buffer.empty() == true); }
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the 103rd byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      auto final_output = chunks.output(buffer);
      THEN("After the delimiter is passed to input, The output method reports ok status") {
        REQUIRE(final_output == PF::Protocols::ChunkOutputStatus::ok);
      }
    }
  }

  GIVEN("A uint8_t chunksplitter with an empty internal buffer of capacity 256 bytes") {
    PF::Protocols::ChunkInputStatus status;
    bool input_overwritten = false;
    PF::Util::ByteVector<buffer_size> output_buffer;
    REQUIRE(output_buffer.empty() == true);

    WHEN("A delimiter is passed as input and output method is called after that") {
      uint8_t delimiter = 0x00;
      status = chunks.input(delimiter, input_overwritten);
      THEN("The input method reports output_ready status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      PF::Util::ByteVector<buffer_size> output_buffer;

      auto output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN(
        "255 non-delimiter bytes and 256th byte as a delimeter are passed as input, after which "
        "output is called") {
      for (size_t i = 0; i < buffer_size; ++i) {
        uint8_t val = 10;
        status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status for all the 255 non-delimeter bytes") {
          REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      uint8_t delimiter = 0x00;
      status = chunks.input(delimiter, input_overwritten);

      THEN("The input method reports output_ready status on the 256th delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);

      THEN("After the delimiter is passed as input, The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN(
          "The output buffer has the expected sequence of 256 bytes where the last byte is a "
          "delimeter") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        REQUIRE(output_buffer.operator[](256) == 0);
      }
    }

    WHEN(
        "255 non-delimiter bytes are passed as input, and 256th byte is passed as a delimiter, and "
        "output is called between each input") {
      uint8_t val = 10;
      for (size_t i = 0; i < buffer_size; ++i) {
        status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status for all the 256 non-delimeter bytes") {
          REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      output_status = chunks.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }
      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }

      uint8_t delimiter = 0x00;
      status = chunks.input(delimiter, input_overwritten);
      THEN("The input method reports output_ready status on the 257th delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);
      THEN("After the delimiter is passed as input, The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN(
          "The output buffer has the expected sequence of 256 bytes where the last byte is a "
          "delimeter") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        REQUIRE(output_buffer.operator[](256) == 0);
      }
    }
  }
}

SCENARIO(
    "A ChunkSplitter constructed with default value for delimeter and include_delimeter boolean as "
    "true correctly handles input chunks of acceptable lengths",
    "[chunks]") {
  GIVEN("A uint8_t chunksplitter with an empty internal buffer of capacity 256 bytes") {
    constexpr size_t buffer_size = 256;
    bool include_delimiter = true;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{0x00, include_delimiter};
    PF::Protocols::ChunkInputStatus status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    PF::Util::ByteVector<buffer_size> expected_buffer;
    bool input_overwritten = false;

    WHEN("A delimiter is passed as input and output method is called after that") {
      uint8_t delimiter = 0x00;
      status = chunks.input(delimiter, input_overwritten);
      THEN("The input method reports output_ready status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      auto output_status = chunks.output(output_buffer);

      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer is a single null byte buffer") {
        expected_buffer.push_back(0x00);
        REQUIRE(output_buffer == expected_buffer);
      }
    }

    WHEN(
        "255 non-delimiter bytes and 256th byte as a delimeter are passed as input, after which "
        "output is called") {
      for (size_t i = 0; i < (buffer_size - 1); ++i) {
        uint8_t val = 10;
        status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status") {
          REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the 256th delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        for (size_t i = 0; i < (buffer_size - 1); ++i) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        // last byte is the delimiter
        REQUIRE(output_buffer.operator[](256) == 0x00);
      }
    }

    WHEN(
        "255 non-delimiter bytes are passed as input, and 256th byte is passed as a delimiter, and "
        "output is called between each input") {
      uint8_t val = 10;
      for (size_t i = 0; i < (buffer_size - 1); ++i) {
        status = chunks.input(val, input_overwritten);
      }

      THEN("The input method reports ok status for all the 255 non-delimeter bytes") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
      }

      output_status = chunks.output(output_buffer);

      THEN("The output method reports waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }

      uint8_t delimiter = 0x00;
      status = chunks.input(delimiter, input_overwritten);

      THEN("The input method reports output_ready status on the 256th delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);

      THEN("After the delimiter is passed as input, The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN(
          "The output buffer has the expected sequence of 256 bytes where the last byte is a "
          "delimeter") {
        for (size_t i = 0; i < (buffer_size - 1); ++i) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        // last byte is the delimiter
        REQUIRE(output_buffer.operator[](256) == 0x00);
      }
    }
  }
}

SCENARIO(
    "A ChunkSplitter constructed with default parameters correctly handles unacceptably long input "
    "chunks",
    "[chunks]") {
  GIVEN(
      "A uint8_t chunk splitter with a completely full internal buffer of 256 bytes with no "
      "delimeters") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{};
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;
    uint8_t val = 128;
    for (size_t i = 0; i < buffer_size; ++i) {
      input_status = chunks.input(val, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    }
    PF::Util::Vector<uint8_t, buffer_size> output_buffer;

    WHEN("257th non-delimiter byte is passed as input") {
      input_status = chunks.input(val, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::invalid_length);
      }
    }

    WHEN("A non-delimiter byte is passed as input and output is called") {
      input_status = chunks.input(val, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::invalid_length);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports invalid_length status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }

      THEN("The output buffer is as expected") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }

    WHEN("A delimeter byte is passed as input and output is called") {
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN(
          "The output buffer has the expected sequence of 256 bytes consisting of the 256 bytes "
          "which had initially filled the internal buffer") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }
}

SCENARIO(
    "ChunkSplitter constructed with with default value for delimeter and include_delimeter boolean "
    "as true correctly handles unacceptably long input chunks",
    "[chunks]") {
  GIVEN("A uint8_t chunk splitter with a completely full internal buffer of 256 bytes") {
    constexpr size_t buffer_size = 256;
    bool include_delimiter = true;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{0x00, include_delimiter};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      input_status = chunks.input(val, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    }

    PF::Util::ByteVector<buffer_size> output_buffer;
    REQUIRE(output_buffer.empty() == true);

    WHEN("257th non-delimiter byte is passed as input") {
      input_status = chunks.input(val, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::invalid_length);
      }
    }

    WHEN("A non-delimiter byte is passed as input and output is called") {
      input_status = chunks.input(val, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::invalid_length);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports invalid_length status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }

      THEN(
          "The output buffer has the expected sequence of 256 bytes that had initially filled the "
          "internal buffer without the delimeter as the last byte") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        REQUIRE(output_buffer.operator[](256) == 0x00);
      }
    }

    WHEN("A delimeter byte is passed as input and output is called") {
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::invalid_length);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports invalid_length status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }

      THEN(
          "The output buffer has the expected sequence of 256 bytes that had initially filled the "
          "internal buffer without the delimeter as the last byte") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
        REQUIRE(output_buffer.operator[](256) == 0x00);
      }
    }
  }
}

SCENARIO(
    "Protocols:: After output is consumed, ChunkSplitter's behavior resets to that of a "
    "ChunkSplitter with an empty buffer",
    "[chunks]") {
  GIVEN(
      "A uint8_t ChunkSplitter completely filled with input data, and no delimiters are consumed "
      "with an output call") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks;
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      input_status = chunks.input(val, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    }

    input_status = chunks.input(0, input_overwritten);
    REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
    output_status = chunks.output(output_buffer);
    REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);

    WHEN(
        "chunks of capacity 254 bytes with varying data are passed as input, after which a "
        "delimiter is passed and output is called") {
      uint8_t val = GENERATE(take(255, random(0, 255)));
      for (size_t i = 0; i < buffer_size; i++) {
        val++;
        chunks.input(val, input_overwritten);
      }
      input_status = chunks.input(0, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      output_status = chunks.output(output_buffer);
      REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
    }
  }
}

SCENARIO(
    "Protocols::A chunksplitter constructed with a non-zero delimeter parameter correctly handles "
    "input chunks",
    "[chunks]") {
  GIVEN(
      "A uint8_t chunksplitter constructed with value of delimeter parameter as 0x01 and "
      "include_delimeter boolean as false is completely filled with non-delimeter input data") {
    constexpr size_t buffer_size = 256;
    uint8_t delimeter = 0x01;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{delimeter};

    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus status;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      status = chunks.input(val, input_overwritten);
      REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
    }
    PF::Util::ByteVector<buffer_size> output_buffer;
    REQUIRE(output_buffer.empty() == true);

    WHEN("A single null (0x00) byte is passed as input") {
      status = chunks.input(0, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::invalid_length);
      }
    }

    WHEN("The output method is called") {
      PF::Protocols::ChunkOutputStatus output_status = chunks.output(output_buffer);
      THEN("The output method returns waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("A delimeter byte (0x01) is passed as input and output is called") {
      uint8_t new_val = 1;
      status = chunks.input(new_val, input_overwritten);
      THEN("the input method reports output ready status on the delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      PF::Protocols::ChunkOutputStatus output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN(
          "The output buffer has expected sequence of 256 bytes consisting of the 256 bytes which "
          "had initially filled the internal buffer") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t chunksplitter constructed with value of delimeter parameter as 0xff and "
      "include_delimeter boolean as false is completely filled with non-delimeter input data") {
    constexpr size_t buffer_size = 256;
    uint8_t delimeter = 0xff;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{delimeter};

    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus status;
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      status = chunks.input(val, input_overwritten);
      REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
    }
    PF::Util::ByteVector<buffer_size> output_buffer;

    WHEN("A single null (0x00) byte is passed as input") {
      status = chunks.input(0, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::invalid_length);
      }
    }

    WHEN("The output method is called") {
      output_status = chunks.output(output_buffer);
      THEN("The output method returns waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("A delimeter byte (0xff) is passed as input and output is called") {
      uint8_t new_val = 0xff;
      status = chunks.input(new_val, input_overwritten);
      THEN("the input method reports output ready status on the delimeter byte") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN(
          "The output buffer has expected sequence of 256 bytes consisting of the 256 bytes which "
          "had initially filled the internal buffer") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t chunksplitter constructed with value of delimeter parameter as 0x01 and "
      "include_delimeter boolean as true is completely filled with non-delimeter input data") {
    constexpr size_t buffer_size = 256;
    uint8_t delimeter = 1;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{delimeter, true};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus status;
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      status = chunks.input(val, input_overwritten);
      REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
    }
    PF::Util::ByteVector<buffer_size> output_buffer;

    WHEN("A single null (0x00) byte is passed as input") {
      status = chunks.input(0, input_overwritten);
      THEN("The input method reports invalid_length status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::invalid_length);
      }
    }

    WHEN("The output method is called") {
      output_status = chunks.output(output_buffer);
      THEN("The output method returns waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("Output is called after passing delimiter equal to 0x01 as input") {
      uint8_t new_val = 1;
      status = chunks.input(new_val, input_overwritten);
      THEN("the input method reports invalid_length status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::invalid_length);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports invalid length status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }

      THEN(
          "The output buffer has expected sequence of 256 bytes consisting of the 256 bytes which "
          "had initially filled the internal buffer") {
        for (size_t i = 0; i < buffer_size; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t chunksplitter constructed with value of delimeter parameter as 0x01 and "
      "include_delimeter boolean as true is partially filled with 128 non-delimeter bytes input "
      "data") {
    constexpr size_t buffer_size = 256;
    uint8_t delimeter = 1;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{delimeter, true};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus status;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size / 2; ++i) {
      status = chunks.input(val, input_overwritten);
      REQUIRE(status == PF::Protocols::ChunkInputStatus::ok);
    }
    PF::Util::ByteVector<buffer_size> output_buffer;

    WHEN("The output method is called") {
      PF::Protocols::ChunkOutputStatus output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("A delimeter byte (0x01) is passed as input and output is called") {
      uint8_t new_val = 1;
      status = chunks.input(new_val, input_overwritten);
      THEN("the input method reports output_ready status") {
        REQUIRE(status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      PF::Util::ByteVector<buffer_size> output_buffer;
      PF::Protocols::ChunkOutputStatus output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN(
          "The output buffer has expected sequence of 129 bytes consisting of the 128 bytes which "
          "had initially filled the internal buffer with delimeter as the last byte") {
        // first 128 bytes of the buffer are 0x80
        for (size_t i = 0; i < (buffer_size / 2) - 1; ++i) {
          uint8_t val = 128;
          REQUIRE(output_buffer.operator[](i) == val);
        }

        // last byte of the buffer is the delimiter
        REQUIRE(output_buffer.operator[](128) == 1);
      }
    }
  }
}

SCENARIO(
    "A ChunkSplitter correctly reports if input is overwritten before or after the output is "
    "consumed",
    "[chunks]") {
  GIVEN(
      "A uint8_t chunksplitter constructed with default parameters with an empty internal buffer "
      "of capacity 256 bytes") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    bool input_overwritten = false;

    WHEN(
        "input to the chunksplitter is '0x01 0x02 0x03 0x00 0x04 0x05 0x00' and output is called "
        "after each delimeter input") {
      auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x00, 0x04, 0x05, 0x00);
      for (size_t i = 0; i < 3; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status for the first 3 non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      input_status = chunks.input(input_data[3], input_overwritten);
      THEN("The input method reports output_ready status for the first delimeter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      THEN("The input_overwritten flag is unchanged") { REQUIRE(input_overwritten == false); }

      output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }

      THEN("The output buffer is as expected '\x01\x02\x03' ") {
        auto expected = std::string("\x01\x02\x03"s);
        REQUIRE(output_buffer == expected);
      }

      for (size_t i = 4; i < 7; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status for the subsequent non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }
      THEN("The input_overwritten flag is unchanged") { REQUIRE(input_overwritten == false); }

      THEN("The input method reports output_ready status for the delimeter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);

      THEN("the output method reports ok status after the last delimeter byte") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer is as expected '\x04\x05' ") {
        auto expected = std::string("\x04\x05"s);
        REQUIRE(output_buffer == expected);
      }
    }

    WHEN(
        "input to the chunksplitter is '0x01 0x02 0x03 0x00 0x04 0x05 0x00' input_overwritten is "
        "initalised to false, and output is called after all the bytes are passed") {
      auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x04, 0x05);

      // first 3 bytes are given as input
      for (size_t i = 0; i < 3; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status on the first 3 bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      // input is passed with delimiter
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the 4th byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      // input is called again
      for (size_t i = 3; i < input_data.size(); ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
      }

      THEN(
          "The input method reports ok status and sets the input_overwritten flag to true on the "
          "5th byte") {
        REQUIRE(input_overwritten == true);
      }

      // input is passed with delimiter
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the delimiter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      auto output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer only contains the last chunk '0x04 0x05' ") {
        auto expected = std::string("\x04\x05"s);
        REQUIRE(output_buffer == expected);
      }
    }
  }

  GIVEN(
      "A uint8_t ChunkSplitter constructed with default parameters, and capacity of 256 bytes "
      "which has received 256 non-delimiter bytes as input, and then a delimeter is passed as "
      "input") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks;
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size; ++i) {
      input_status = chunks.input(val, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    }

    input_status = chunks.input(0, input_overwritten);
    REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);

    input_status = chunks.input(val, input_overwritten);
    REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    REQUIRE(input_overwritten == true);

    WHEN(
        "10 bytes of non-delimiter bytes followed by a delimiter byte are passed as input, with "
        "the input method's input_overwritten flag initalised as true, and output is called "
        "between each input") {
      constexpr size_t size = 10;

      for (size_t i = 0; i < size; ++i) {
        input_status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status for all the non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }
      THEN("The input_overwritten flag remains unchanged after input of all 10 bytes") {
        REQUIRE(input_overwritten == true);
      }
      output_status = chunks.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }
      input_status = chunks.input(0, input_overwritten);

      THEN("The input method reports output_ready status on passing the delimeter") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      THEN("The input_overwritten flag is unchanged") { REQUIRE(input_overwritten == true); }
      output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer has an expected sequence of 10 bytes without delimeters") {
        for (size_t i = 0; i < size; ++i) {
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t ChunkSplitter constructed with default parameters, and capacity of 256 bytes "
      "which is partially filled with 128 non-delimeter bytes input data") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    bool input_overwritten = false;
    for (size_t i = 0; i < buffer_size / 2; ++i) {
      input_status = chunks.input(val, input_overwritten);
      REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
    }

    WHEN(
        "10 bytes of non-delimiter bytes followed by a delimiter byte are passed as input, with "
        "the input method's input_overwritten flag initalised as false, and output is called "
        "between each input") {
      constexpr size_t size = 10;

      for (size_t i = 0; i < size; ++i) {
        input_status = chunks.input(val, input_overwritten);
        THEN("The input method reports ok status for all the non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }
      THEN("The input_overwritten flag remains unchanged after input of all 10 bytes") {
        REQUIRE(input_overwritten == false);
      }
      output_status = chunks.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::waiting);
      }
      input_status = chunks.input(0, input_overwritten);

      THEN("The input method reports output_ready status on passing the delimeter") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      THEN("The input_overwritten flag is unchanged") { REQUIRE(input_overwritten == false); }
      output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN(
          "The output buffer has an expected sequence of 138 bytes consisting of the 128 bytes "
          "which had initially filled the internal buffer without delimeters") {
        for (size_t i = 0; i < 137; ++i) {
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A uint8_t chunksplitter constructed with default delimeter parameter and include_delimeter "
      "boolean as true with an empty internal buffer of capacity 256 bytes") {
    constexpr size_t buffer_size = 256;
    bool include_delimiter = true;
    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunks{0x00, include_delimiter};
    uint8_t val = 128;
    PF::Protocols::ChunkInputStatus input_status;
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> output_buffer;
    bool input_overwritten = false;

    WHEN(
        "input to the chunksplitter is '0x01 0x02 0x03 0x00 0x04 0x05 0x00' with the input "
        "method's input_overwritten flag initalised as false, and output is called after each "
        "delimeter") {
      auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x00, 0x04, 0x05, 0x00);
      for (size_t i = 0; i < 3; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status for all the non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      input_status = chunks.input(input_data[3], input_overwritten);
      THEN("The input method reports output_ready status for the delimeter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }
      THEN("The input_overwritten flag is unchanged") { REQUIRE(input_overwritten == false); }

      output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer is as expected '0x01 0x02 0x03 0x00' ") {
        auto expected = std::string("\x01\x02\x03\x00"s);
        REQUIRE(output_buffer == expected);
      }

      for (size_t i = 4; i < 6; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status for the subsequent non-delimeter bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      input_status = chunks.input(input_data[6], input_overwritten);
      THEN("The input method reports output_ready status for the delimeter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      output_status = chunks.output(output_buffer);
      THEN("the output method reports ok status after the last delimeter byte") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer is as expected '0x04 0x05 0x00' ") {
        auto expected = std::string("\x04\x05\x00"s);
        REQUIRE(output_buffer == expected);
      }
    }

    WHEN(
        "input to the chunksplitter is '0x01 0x02 0x03 0x00 0x04 0x05' and output is not called "
        "after passing delimeter") {
      auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x04, 0x05);

      // first 3 bytes are given as input
      for (size_t i = 0; i < 3; ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        THEN("The input method reports ok status on the first 3 bytes") {
          REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
        }
      }

      // input is passed with delimiter
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the 4th byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      // input is called again
      for (size_t i = 3; i < input_data.size(); ++i) {
        input_status = chunks.input(input_data[i], input_overwritten);
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::ok);
      }

      THEN(
          "The input method reports ok status and sets the input_overwritten flag to true on the "
          "4th "
          "byte") {
        REQUIRE(input_overwritten == true);
      }

      // input is passed with delimiter
      input_status = chunks.input(0, input_overwritten);
      THEN("The input method reports output_ready status on the delimiter byte") {
        REQUIRE(input_status == PF::Protocols::ChunkInputStatus::output_ready);
      }

      auto output_status = chunks.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The output buffer only contains the last chunk '0x04 0x05 0x00' ") {
        auto expected = std::string("\x04\x05\x00"s);
        REQUIRE(output_buffer == expected);
      }
    }
  }
}

SCENARIO("Protocols::ChunkMerger behaves correctly", "[chunks]") {
  GIVEN("A ChunkMerger with delimiter equal to 0x00") {
    constexpr size_t buffer_size = 30;
    PF::Protocols::ChunkMerger chunk_merger{};
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;

    WHEN("A partially full buffer of size 10 bytes is passed as input to transform method") {
      constexpr size_t size = 10;
      PF::Util::ByteVector<size> buffer;
      PF::IndexStatus index_status;

      for (size_t i = 0; i < size - 1; ++i) {
        uint8_t val = 10;
        buffer.push_back(val);
      }

      output_status = chunk_merger.transform<size, uint8_t>(buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends a delimeter") {
        auto expected = std::string("\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x00"s);
        REQUIRE(buffer == expected);
      }
    }

    WHEN("Completely full buffer of size 30 bytes is passed as input to transform method") {
      uint8_t val = 128;
      PF::Util::ByteVector<buffer_size> buffer;
      PF::IndexStatus index_status;

      for (size_t i = 0; i < buffer_size; ++i) {
        index_status = buffer.push_back(val);
        REQUIRE(index_status == PF::IndexStatus::ok);
      }

      output_status = chunk_merger.transform<buffer_size, uint8_t>(buffer);

      THEN("The transform method reports invalid length") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }
    }

    WHEN("An input data with it's last byte equal to the delimiter is passed to transform") {
      constexpr size_t size = 10;
      PF::Util::ByteVector<size> buffer;
      PF::IndexStatus index_status;

      for (size_t i = 0; i < size - 2; ++i) {
        uint8_t val = 10;
        buffer.push_back(val);
      }

      buffer.push_back(0x00);

      output_status = chunk_merger.transform<size, uint8_t>(buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends a delimeter") {
        auto expected = std::string("\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x00\x00"s);
        REQUIRE(buffer == expected);
      }
    }

    WHEN("An input data with it's first byte equal to the delimiter is passed to transform") {
      constexpr size_t size = 10;
      PF::Util::ByteVector<size> buffer;
      PF::IndexStatus index_status;

      buffer.push_back(0x00);

      for (size_t i = 1; i < size - 1; ++i) {
        uint8_t val = 10;
        buffer.push_back(val);
      }

      output_status = chunk_merger.transform<size, uint8_t>(buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends a delimeter") {
        auto expected = std::string("\x00\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x00"s);
        REQUIRE(buffer == expected);
      }
    }

    WHEN("An input data with multiple delimited chunks is passed as input to transform") {
      constexpr size_t size = 10;
      PF::Util::ByteVector<size> buffer;
      PF::IndexStatus index_status;

      auto data =
          PF::Util::make_array<uint8_t>(0x01, 0x02, 0x00, 0x03, 0x00, 0x04, 0x05, 0x00, 0x07);

      for (size_t i = 0; i < 9; ++i) {
        buffer.push_back(data[i]);
      }

      output_status = chunk_merger.transform<size, uint8_t>(buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends a delimeter") {
        auto expected = std::string("\x01\x02\x00\x03\x00\x04\x05\x00\x07\x00"s);
        REQUIRE(buffer == expected);
      }
    }
  }

  GIVEN("A ChunkMerger with delimiter equal to 0x01") {
    constexpr size_t buffer_size = 30;
    PF::Protocols::ChunkMerger chunk_merger{0x01};
    PF::Protocols::ChunkOutputStatus output_status;
    bool input_overwritten = false;

    WHEN("A partially full buffer of size 10 bytes is passed as input to transform method") {
      constexpr size_t size = 10;
      PF::Util::ByteVector<size> buffer;
      PF::IndexStatus index_status;

      for (size_t i = 0; i < size - 1; ++i) {
        uint8_t val = 10;
        buffer.push_back(val);
      }

      output_status = chunk_merger.transform<size, uint8_t>(buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends a delimeter") {
        auto expected = std::string("\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x01"s);
        REQUIRE(buffer == expected);
      }
    }

    WHEN("Completely full buffer of size 30 bytes is passed as input to transform method") {
      uint8_t val = 128;
      PF::Util::ByteVector<buffer_size> buffer;
      PF::IndexStatus index_status;

      for (size_t i = 0; i < buffer_size; ++i) {
        index_status = buffer.push_back(val);
        REQUIRE(index_status == PF::IndexStatus::ok);
      }

      output_status = chunk_merger.transform<buffer_size, uint8_t>(buffer);

      THEN("The transform method reports invalid length") {
        REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::invalid_length);
      }
    }
  }
}

SCENARIO("ChunkMerger and Chunksplitter roundtrip behaves properly", "[chunks]") {
  GIVEN(
      "A chunkmerger object with delimiter as 0x00,A chunksplitter object with empty internal "
      "buffer and multiple bytevectors") {
    constexpr size_t buffer_size = 256;
    PF::Protocols::ChunkMerger chunk_merger{};
    PF::Protocols::ChunkOutputStatus output_status;
    PF::Util::ByteVector<buffer_size> expected1;
    PF::Util::ByteVector<buffer_size> expected2;
    PF::Util::ByteVector<buffer_size> expected3;

    PF::Protocols::ChunkSplitter<buffer_size, uint8_t> chunk_splitter{};

    WHEN(
        "A merged chunk from 3 different byte vectors after transform is generated, then passed as "
        "input to a chunksplitter, after which the merged output is passed to a new Chunkmerger") {
      PF::Util::ByteVector<buffer_size> vector1;
      auto body1 = std::string("\xda\xb3\x1b\x94\xa5"s);
      PF::Util::convert_string_to_byte_vector(body1, vector1);
      PF::Util::ByteVector<buffer_size> vector2;
      auto body2 = std::string("\x1b\x1a\xc5\xd9\xf3"s);
      PF::Util::convert_string_to_byte_vector(body2, vector2);
      PF::Util::ByteVector<buffer_size> vector3;
      auto body3 = std::string("\xbb\x7d\xa8\x01\xf1"s);
      PF::Util::convert_string_to_byte_vector(body3, vector3);

      expected1.copy_from(vector1.buffer(), vector1.size());
      expected2.copy_from(vector2.buffer(), vector2.size());
      expected3.copy_from(vector3.buffer(), vector3.size());

      // ChunkMerger
      auto status_1 = chunk_merger.transform<buffer_size, uint8_t>(vector1);
      THEN("The transform method of the first ChunkMerger reports ok status for first ByteVector") {
        REQUIRE(status_1 == PF::Protocols::ChunkOutputStatus::ok);
      }
      expected1.push_back(0);
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(vector1 == expected1); }
      auto status_2 = chunk_merger.transform<buffer_size, uint8_t>(vector2);
      THEN(
          "The transform method of the first ChunkMerger reports ok status for second ByteVector") {
        REQUIRE(status_2 == PF::Protocols::ChunkOutputStatus::ok);
      }
      expected2.push_back(0);
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(vector2 == expected2); }
      auto status_3 = chunk_merger.transform<buffer_size, uint8_t>(vector3);
      THEN("The transform method of the first ChunkMerger reports ok status for third ByteVector") {
        REQUIRE(status_3 == PF::Protocols::ChunkOutputStatus::ok);
      }
      expected3.push_back(0);
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(vector3 == expected3); }

      PF::Util::ByteVector<buffer_size> merged1;
      merged1.copy_from(vector1.buffer(), vector1.size());
      merged1.copy_from(vector2.buffer(), vector2.size(), 6);
      merged1.copy_from(vector3.buffer(), vector3.size(), 12);

      // ChunkSplitter
      PF::Util::ByteVector<buffer_size> split1;
      PF::Util::ByteVector<buffer_size> split2;
      PF::Util::ByteVector<buffer_size> split3;
      bool input_overwritten = false;
      PF::Util::ByteVector<buffer_size> output_buffer;

      for (size_t i = 0; i < 6; i++) {
        auto input_status = chunk_splitter.input(merged1[i], input_overwritten);
        if (input_status == PF::Protocols::ChunkInputStatus::output_ready) {
          output_status = chunk_splitter.output(output_buffer);
          split1.copy_from(output_buffer.buffer(), output_buffer.size());
          THEN("The output method of the chunksplitter reports ok status after each delimeter") {
            REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
          }
        }
      }

      for (size_t i = 6; i < 12; i++) {
        auto input_status = chunk_splitter.input(merged1[i], input_overwritten);
        if (input_status == PF::Protocols::ChunkInputStatus::output_ready) {
          output_status = chunk_splitter.output(output_buffer);
          split2.copy_from(output_buffer.buffer(), output_buffer.size());
          THEN("The output method of the chunksplitter reports ok status after each delimeter") {
            REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
          }
        }
      }

      for (size_t i = 12; i < merged1.size(); i++) {
        auto input_status = chunk_splitter.input(merged1[i], input_overwritten);
        if (input_status == PF::Protocols::ChunkInputStatus::output_ready) {
          output_status = chunk_splitter.output(output_buffer);
          split3.copy_from(output_buffer.buffer(), output_buffer.size());
          THEN("The output method of the chunksplitter reports ok status after each delimeter") {
            REQUIRE(output_status == PF::Protocols::ChunkOutputStatus::ok);
          }
        }
      }
      THEN("The ChunkSplitter outputs 3 ByteVectors as expected") {
        REQUIRE(split1 == body1);
        REQUIRE(split2 == body2);
        REQUIRE(split3 == body3);
      }

      // ChunkMerger
      auto merge1 = chunk_merger.transform<buffer_size, uint8_t>(split1);
      THEN("The transform method reports ok status for first bytevector") {
        REQUIRE(merge1 == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(split1 == expected1); }
      auto merge2 = chunk_merger.transform<buffer_size, uint8_t>(split2);
      THEN("The transform method reports ok status for second bytevector") {
        REQUIRE(merge2 == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(split2 == expected2); }
      auto merge3 = chunk_merger.transform<buffer_size, uint8_t>(split3);
      THEN("The transform method reports ok status for third bytevector") {
        REQUIRE(merge3 == PF::Protocols::ChunkOutputStatus::ok);
      }
      THEN("The ChunkMerger appends the delimeter") { REQUIRE(split3 == expected3); }

      PF::Util::ByteVector<buffer_size> merged2;
      merged2.copy_from(split1.buffer(), split1.size());
      merged2.copy_from(split2.buffer(), split2.size(), 6);
      merged2.copy_from(split3.buffer(), split3.size(), 12);

      REQUIRE(merged1 == merged2);
    }
  }
}