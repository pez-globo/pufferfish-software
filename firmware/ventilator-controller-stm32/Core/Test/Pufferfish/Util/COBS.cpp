
/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * COBS.cpp
 *
 *  Created on: Mar 25, 2021
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior of COBS
 *
 */
#include "Pufferfish/Util/COBS.h"

#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
using namespace std::string_literals;

SCENARIO("The Util encode_cobs function correctly encodes buffers", "[COBS]") {
  GIVEN("The Util COBS::encode function") {
    constexpr size_t buffer_size = 256UL;
    constexpr size_t encoded_buffer_size = 257UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<encoded_buffer_size> encoded_buffer;
    PF::IndexStatus push_status;

    WHEN("The encoded byte vector is too small to hold the encoded data") {
      constexpr size_t encoded_buffer_size = 4UL;
      PF::Util::ByteVector<encoded_buffer_size> encoded_buffer;
      auto body = std::string("\x48\x0e\x79\x1a"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The encoded buffer is empty") { REQUIRE(encoded_buffer.empty() == true); }
    }

    WHEN("The cobs::encode function is called on a null byte") {
      push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x01\x01' ") {
        auto expected = std::string("\x01\x01"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on a buffer that contains these bytes '0x00 0x00'") {
      push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);
      push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x01\x01\x01' ") {
        auto expected = std::string("\x01\x01\x01"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode function is called on a buffer that contains these bytes '0x11 0x22 0x00 "
        "0x33'") {
      auto body = std::string("\x11\x22\x00\x33"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x03\x11\x22\x02\x33'") {
        auto expected = std::string("\x03\x11\x22\x02\x33"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode function is called on a buffer that contains these bytes '0x11 0x22 0x00 "
        "0x33' and a output_buffer paritally filled with 2-bytes is passed") {
      auto body = std::string("\x11\x22\x00\x33"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      push_status = encoded_buffer.push_back(0x01);
      REQUIRE(push_status == PF::IndexStatus::ok);
      push_status = encoded_buffer.push_back(0x02);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x03\x11\x22\x02\x33'") {
        auto expected = std::string("\x03\x11\x22\x02\x33"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode function is called on a buffer that contains these bytes "
        "'\x74\x27\xab\xfb' and a output_buffer partially filled with 10-bytes is passed") {
      auto body = std::string("\x74\x27\xab\xfb"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto data = std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA"s);
      PF::Util::convert_string_to_byte_vector(data, encoded_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x05\x74\x27\xab\xfb'") {
        auto expected = std::string("\x05\x74\x27\xab\xfb"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode function is called on a buffer that contains these bytes "
        "'\x11\x22\x33\x44'") {
      auto body = std::string("\x11\x22\x33\x44"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x05\x11\x22\x33\x44'") {
        auto expected = std::string("\x05\x11\x22\x33\x44"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on the char 'x'") {
      auto body = std::string("x"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\02x'") {
        auto expected = std::string("\x02\x78"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on the char 'xy'") {
      auto body = std::string("xy"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\03xy'") {
        auto expected = std::string("\x03\x78\x79"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on the string 'Hello world'") {
      auto body = std::string("Hello World"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x0cHello World'") {
        auto expected = std::string("\x0cHello World"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on a sensor measurements message payload") {
      auto body = std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected") {
        auto expected =
            std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN(
        "The cobs::encode function is called on a buffer that contains these bytes '0x6e 0xd7 0xf1 "
        "0x00 "
        "0xf7 0xab' ") {
      auto data = PF::Util::make_array<uint8_t>(0x6e, 0xd7, 0xf1, 0x00, 0xf7, 0xab);
      for (auto& bytes : data) {
        push_status = input_buffer.push_back(bytes);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);

      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer is as expected '\x04\x6e\xd7\xf1\x03\xf7\xab' ") {
        auto expected = std::string("\x04\x6e\xd7\xf1\x03\xf7\xab"s);
        REQUIRE(encoded_buffer == expected);
      }
    }

    WHEN("The cobs::encode function is called on a 253 byte buffer") {
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xfe") {
        REQUIRE(encoded_buffer.operator[](0) == 0xfe);
        for (size_t i = 1; i < 253; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
      }
    }

    WHEN(
        "The cobs::encode function is called on a 254 bytes buffer with null byte as the last "
        "byte") {
      for (size_t i = 0; i < 252; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      push_status = input_buffer.push_back(0);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xfd") {
        REQUIRE(encoded_buffer.operator[](0) == 0xfd);
        for (size_t i = 1; i < 253; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        REQUIRE(encoded_buffer.operator[](253) == 1);
      }
    }

    WHEN("The cobs::encode function is called on a 254 byte buffer with no null bytes") {
      for (size_t i = 0; i < 254; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 254 bytes with first byte as 0xff") {
        REQUIRE(encoded_buffer.operator[](0) == 0xff);
        for (size_t i = 1; i < 254; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        REQUIRE(encoded_buffer.operator[](255) == 1);
      }
    }

    WHEN("The cobs::encode function is called on a 255 byte buffer with no null bytes") {
      for (size_t i = 0; i < 255; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::encode_cobs(input_buffer, encoded_buffer);
      THEN("The encode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The encoded buffer has an expected sequence of 257 bytes with first byte as 0xff") {
        REQUIRE(encoded_buffer.operator[](0) == 0xff);
        for (size_t i = 1; i < 254; i++) {
          REQUIRE(encoded_buffer.operator[](i) == 10);
        }
        REQUIRE(encoded_buffer.operator[](255) == 2);
        REQUIRE(encoded_buffer.operator[](256) == 10);
      }
    }
  }
}

SCENARIO("The Util decode_cobs function correctly decodes encoded buffers", "[COBS]") {
  GIVEN("The Util COBS::decode function") {
    constexpr size_t buffer_size = 255UL;
    constexpr size_t decoded_buffer_size = 254UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;
    PF::IndexStatus push_status;

    WHEN("The COBS::decode function is called on a buffer containing '0x01' as the only byte") {
      push_status = input_buffer.push_back(0x01);
      REQUIRE(push_status == PF::IndexStatus::ok);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN("The COBS::decode function is called on a buffer containing '0x03' as the only byte") {
      push_status = input_buffer.push_back(0x03);
      REQUIRE(push_status == PF::IndexStatus::ok);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode function is called on a buffer that contains these bytes '0x05 0x02 "
        "0x03' ") {
      auto data = PF::Util::make_array<uint8_t>(0x05, 0x02, 0x03);
      for (auto& bytes : data) {
        push_status = input_buffer.push_back(bytes);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode function is called on a buffer that contains these bytes '0x05 0x02 "
        "0xff' ") {
      auto data = PF::Util::make_array<uint8_t>(0x05, 0x02, 0xff);
      for (auto& bytes : data) {
        push_status = input_buffer.push_back(bytes);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode function is called on a buffer that contains these bytes '0x02 0x02 "
        "0xff' ") {
      auto data = PF::Util::make_array<uint8_t>(0x02, 0x02, 0xff);
      for (auto& bytes : data) {
        push_status = input_buffer.push_back(bytes);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
    }

    WHEN("The decoded buffer is too small to hold the decoded data") {
      constexpr size_t buffer_size = 5UL;
      PF::Util::ByteVector<buffer_size> decoded_buffer;
      auto body = std::string("\xd2\xf4\xa0\x11\xa9\x42\x64\x5e\x8a\xe8"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(decoded_buffer.empty() == true); }
    }

    WHEN(
        "The COBS::decode function is called on a 257-byte buffer filled with the manual encoding "
        "of a 255 non-null-byte payload with a bad null header byte") {
      constexpr size_t buffer_size = 257UL;
      constexpr size_t decoded_buffer_size = 255UL;
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;
      uint8_t val = 10;
      push_status = input_buffer.push_back(0xff);
      REQUIRE(push_status == PF::IndexStatus::ok);
      for (size_t i = 0; i < 253; i++) {
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      push_status = input_buffer.push_back(0x02);
      REQUIRE(push_status == PF::IndexStatus::ok);
      push_status = input_buffer.push_back(val);
      REQUIRE(push_status == PF::IndexStatus::ok);
      push_status = input_buffer.push_back(val);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN(
          "The decoded buffer has expected sequence of 255 bytes as '0xff 0x71 0xcf ......0x02 "
          "0xa2 0xd2' ") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
        REQUIRE(decoded_buffer.operator[](253) == 0x02);
      }
    }

    WHEN("The COBS::decode function is called on a buffer containing these bytes '\x01\x01'") {
      auto body = std::string("\x01\x01"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x00'") {
        auto expected = std::string("\x00"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN("The COBS::decode function is called on a buffer containing these bytes '\x01\x01\x01'") {
      auto body = std::string("\x01\x01\x01"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x00 0x00'") {
        auto expected = std::string("\x00\x00"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer containing these bytes "
        "'\x03\x11\x22\x02\x33'") {
      auto body = std::string("\x03\x11\x22\x02\x33"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x11 0x22 0x00 0x33'") {
        auto expected = std::string("\x11\x22\x00\x33"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer containing these bytes "
        "'\x03\x9f\x8c\x01\x03\x21\xe8' and a decoded buffer partially filled with 3-bytes is "
        "passed") {
      auto body = std::string("\x03\x9f\x8c\x01\x03\x21\xe8"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto data = std::string("\x01\x02\x03"s);
      PF::Util::convert_string_to_byte_vector(data, decoded_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x9f 0x8c 0x00 0x00 0x21 0xe8'") {
        auto expected = std::string("\x9f\x8c\x00\x00\x21\xe8"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer containing these bytes "
        "'\x05\x11\x22\x33\x44'") {
      auto body = std::string("\x05\x11\x22\x33\x44"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '\x11\x22\x33\x44'") {
        auto expected = std::string("\x11\x22\x33\x44"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN("The COBS::decode function is called on a buffer containing these bytes '\x02x'") {
      auto body = std::string("\x02\x78"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected 'x' ") {
        auto expected = std::string("x"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN("The COBS::decode function is called on a buffer containing these bytes '\x03xy'") {
      auto body = std::string("\x03\x78\x79"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected 'xy' ") {
        auto expected = std::string("xy"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer containing the bytestring '\x0cHello "
        "World'") {
      auto body = std::string("\x0cHello World"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected 'Hello World'") {
        auto expected = std::string("Hello World"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer containing a encoded sensor measurements "
        "message payload") {
      auto body =
          std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42"s);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected") {
        auto expected =
            std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
        REQUIRE(decoded_buffer == expected);
      }
    }

    WHEN(
        "The COBS::decode function is called on a buffer that contains these bytes '0x03, 0x9c, "
        "0xb8, 0x03, 0xbe, 0xce'") {
      auto data = PF::Util::make_array<uint8_t>(0x03, 0x9c, 0xb8, 0x03, 0xbe, 0xce);
      for (auto& bytes : data) {
        push_status = input_buffer.push_back(bytes);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer is as expected '0x9c 0xb8 0x00 0xbe 0xce' ") {
        auto expected = std::string("\x9c\xb8\x00\xbe\xce"s);
        REQUIRE(decoded_buffer == expected);
      }
    }
  }
}

SCENARIO(
    "The Util decode_cobs function correctly decodes encoded buffers of large size", "[COBS]") {
  GIVEN("The Util COBS::decode function") {
    constexpr size_t buffer_size = 255UL;
    constexpr size_t decoded_buffer_size = 254UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;
    PF::IndexStatus push_status;
    WHEN(
        "The COBS::decode function is called on a 254-byte buffer filled with the manual encoding "
        "of a 253 non-null-byte payload") {
      push_status = input_buffer.push_back(0xfe);
      REQUIRE(push_status == PF::IndexStatus::ok);
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer has expected sequence of 254 bytes") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
      }
    }

    WHEN(
        "The COBS::decode function is called on a 256-byte buffer filled with the manual encoding "
        "of a 254 byte payload with null byte at the end") {
      constexpr size_t buffer_size = 256UL;
      constexpr size_t decoded_buffer_size = 254UL;
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;
      push_status = input_buffer.push_back(0xff);
      REQUIRE(push_status == PF::IndexStatus::ok);
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      push_status = input_buffer.push_back(0x01);
      REQUIRE(push_status == PF::IndexStatus::ok);
      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The decoded buffer has expected sequence of 254 bytes with last byte as null byte") {
        for (size_t i = 0; i < 253; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
        REQUIRE(decoded_buffer.operator[](254) == 0);
      }
    }

    WHEN(
        "The COBS::decode function is called on a 257-byte buffer filled with the manual encoding "
        "of a 255 non-null-byte payload") {
      constexpr size_t buffer_size = 257UL;
      constexpr size_t decoded_buffer_size = 255UL;
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::ByteVector<decoded_buffer_size> decoded_buffer;
      uint8_t val = 10;
      push_status = input_buffer.push_back(0xff);
      REQUIRE(push_status == PF::IndexStatus::ok);
      for (size_t i = 0; i < 254; i++) {
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      push_status = input_buffer.push_back(0x02);
      REQUIRE(push_status == PF::IndexStatus::ok);
      push_status = input_buffer.push_back(val);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = PF::Util::decode_cobs(input_buffer, decoded_buffer);

      THEN("The decode_cobs function reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN(
          "The decoded buffer has expected sequence of 255 bytes as '0xff 0x71 0xcf ......0x02 "
          "0xa2 0xd2' ") {
        for (size_t i = 0; i < 255; i++) {
          uint8_t val = 10;
          REQUIRE(decoded_buffer.operator[](i) == val);
        }
      }
    }
  }
}