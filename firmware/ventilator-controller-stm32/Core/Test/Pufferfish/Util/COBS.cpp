
/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * COBS.cpp
 *
 *  Created on: Nov 24, 2020
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior of COBS Util
 *
 */
#include "Pufferfish/Util/COBS.h"

#include <iostream>

#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
using namespace std::string_literals;

SCENARIO("The Util decode_cobs method correctly decodes encoded buffers", "[COBS]") {
  GIVEN("The Util COBS::decode function") {
    constexpr size_t buffer_size = 255UL;
    constexpr size_t decoded_buffer_size = 254UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;

    WHEN("The COBS::decode method is called on a buffer containing '0x01' as the only byte") {
      input_buffer.push_back(0x01);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      // THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN("The COBS::decode method is called on a buffer containing '0x03' as the only byte") {
      input_buffer.push_back(0x03);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode method is called on a buffer that contains these bytes '0x05 0x02 "
        "0x03' ") {
      auto data = PF::Util::make_array<uint8_t>(0x05, 0x02, 0x03);
      for (auto& bytes : data) {
        input_buffer.push_back(bytes);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode method is called on a buffer that contains these bytes '0x03, 0x9c, "
        "0xb8, 0x03, 0xbe, 0xce'") {
      auto data = PF::Util::make_array<uint8_t>(0x03, 0x9c, 0xb8, 0x03, 0xbe, 0xce);
      for (auto& bytes : data) {
        input_buffer.push_back(bytes);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x9c 0xb8 0x00 0xbe 0xce' ") {
        auto expected = std::string("\x9c\xb8\x00\xbe\xce"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode method is called on a encoded buffer of capacity 254 bytes filled with "
        "data whose original buffer was of capacity 253 with no null bytes") {
      input_buffer.push_back(0xfe);
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer has expected sequence of 254 bytes") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
      }
    }

    WHEN(
        "The COBS::decode method is called on a encoded buffer of capacity 256 bytes filled with "
        "data whose original buffer was of capacity 254 with null byte at the end") {
      input_buffer.push_back(0xff);
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }
      input_buffer.push_back(0x01);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer has expected sequence of 254 bytes with last byte as null byte") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
        REQUIRE(decoded_buffer.operator[](254) == 0);
      }
    }

    WHEN(
        "The COBS::decode method is called on a encoded buffer of capacity 257 bytes whose "
        "original buffer was of capacity 255 with no null bytes") {
      uint8_t val = 10;
      input_buffer.push_back(0xff);
      for (size_t i = 0; i < 253; i++) {
        input_buffer.push_back(val);
      }
      input_buffer.push_back(0x02);
      input_buffer.push_back(val);
      input_buffer.push_back(val);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN(
          "The decoded buffer has expected sequence of 255 bytes as '0xff 0x71 0xcf ......0x02 "
          "0xa2 0xd2' ") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
      }
    }
  }
}

SCENARIO("The Util encode_cobs method correctly encodes buffers", "[COBS]") {
  GIVEN("The Util COBS::encode function") {
    constexpr size_t buffer_size = 256UL;
    constexpr size_t encoded_buffer_size = 257UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<encoded_buffer_size> encoded_buffer;

    WHEN("The cobs::encode is called on a null byte") {
      input_buffer.push_back(0x00);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x01\x01' ") {
        auto expected = std::string("\x01\x01"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode is called on a buffer that contains these bytes '0x00 0x00'") {
      input_buffer.push_back(0x00);
      input_buffer.push_back(0x00);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x01\x01\x01' ") {
        auto expected = std::string("\x01\x01\x01"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode is called on a buffer that contains these bytes '0x6e 0xd7 0xf1 0x00 "
        "0xf7 0xab' ") {
      auto data = PF::Util::make_array<uint8_t>(0x6e, 0xd7, 0xf1, 0x00, 0xf7, 0xab);
      for (auto& bytes : data) {
        input_buffer.push_back(bytes);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x04\x6e\xd7\xf1\x03\xf7\xab' ") {
        auto expected = std::string("\x04\x6e\xd7\xf1\x03\xf7\xab"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode is called on a 253 byte buffer") {
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xfe") {
        REQUIRE(encoded_buffer.operator[](0) == 0xfe);
        for (size_t i = 1; i < 253; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
      }
    }

    WHEN("The cobs::encode is called on a 254 bytes buffer with last byte as a null byte") {
      for (size_t i = 0; i < 252; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }
      input_buffer.push_back(0);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xfd") {
        REQUIRE(encoded_buffer.operator[](0) == 0xfd);
        for (size_t i = 1; i < 253; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        REQUIRE(encoded_buffer.operator[](253) == 1);
      }
    }

    WHEN("The cobs::encode is called on a 254 byte buffer with no null bytes") {
      for (size_t i = 0; i < 254; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xff") {
        REQUIRE(encoded_buffer.operator[](0) == 0xff);
        for (size_t i = 1; i < 254; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        // REQUIRE(encoded_buffer.operator[](256) == 1);
      }
    }

    WHEN("The cobs::encode is called on a 255 byte buffer with no null bytes") {
      for (size_t i = 0; i < 255; i++) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 257 bytes with first byte as 0xff") {
        REQUIRE(encoded_buffer.operator[](0) == 0xff);
        for (size_t i = 1; i < 254; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        // REQUIRE(encoded_buffer.operator[](256) == 2);
        // REQUIRE(encoded_buffer.operator[](257) == 10);
      }
    }
  }
}
