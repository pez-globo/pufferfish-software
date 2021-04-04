/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Frames.cpp
 *
 *  Created on: Nov 17, 2020
 *      Author: Renji Panicker
 *
 * Unit tests to confirm behavior of the Frames
 *
 */
#include "Pufferfish/Driver/Serial/Backend/Frames.h"

#include <iostream>

#include "Pufferfish/HAL/CRCChecker.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
using namespace std::string_literals;

SCENARIO(
    "Serial::The CobsDecoder class correctly decodes payloads with COBS to payloads", "[Backend]") {
  GIVEN("A CobsDecoder object constructed with default parameters") {
    constexpr size_t buffer_size = 254UL;
    PF::Driver::Serial::Backend::COBSDecoder cobs_decoder{};
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<buffer_size> output_buffer;
    PF::IndexStatus push_status;

    REQUIRE(output_buffer.empty() == true);

    WHEN("transform is called on a buffer containing '0x03' as the only byte") {
      push_status = input_buffer.push_back(0x03);
      REQUIRE(push_status == PF::IndexStatus::ok);
      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(output_buffer.empty() == true); }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x03"s);
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN(
        "transform is called on a buffer that contains these bytes '0x05 0x02 "
        "0xff' ") {
      auto body = std::string("\x05\x02\xff"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The decoded buffer is empty") { REQUIRE(output_buffer.empty() == true); }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on a buffer that contains these bytes '0x02 0x02 "
        "0xff' ") {
      auto body = std::string("\x02\x02\xff"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports out of bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '0x00 0x00'") {
      auto body = std::string("\x01\x01"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '0x00'") {
        auto expected = std::string("\x00"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '\x01\x01\x01'") {
      auto body = std::string("\x01\x01\x01"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '0x00 0x00'") {
        auto expected = std::string("\x00\x00"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on an input buffer that contains these bytes '\x03\x11\x22\x02\x33'") {
      auto body = std::string("\x03\x11\x22\x02\x33"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '0x11 0x22 0x00 0x33'") {
        auto expected = std::string("\x11\x22\x00\x33"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on an input buffer that contains these bytes '\x05\x11\x22\x33\x44'") {
      auto body = std::string("\x05\x11\x22\x33\x44"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x11\x22\x33\x44'") {
        auto expected = std::string("\x11\x22\x33\x44"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '\x02x'") {
      auto body = std::string("\x02\x78"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected 'x'") {
        auto expected = std::string("x"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '\x03xy'") {
      auto body = std::string("\x03\x78\x79"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected 'xy'") {
        auto expected = std::string("xy"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer containing the bytestring '\x0cHello World'") {
      auto body = std::string("\x0cHello World"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected 'Hello World'") {
        auto expected = std::string("Hello World"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on an input buffer that contains a encoded sensor measurements "
        "message payload") {
      auto body =
          std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42"s);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer has an expected sequence of 16 bytes") {
        auto expected =
            std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on a 255-byte buffer filled with the manual encoding "
        "of a 254 non-null-byte payload") {
      constexpr size_t buffer_size = 255UL;
      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0xff);
      REQUIRE(push_status == PF::IndexStatus::ok);
      for (size_t i = 0; i < 254; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer has expected sequence of 254 bytes") {
        for (size_t i = 0; i < 254; i++) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }
}

SCENARIO("Serial::The CobsEncoder class correctly encodes payloads with COBS", "[Backend]") {
  GIVEN("A CobsEncoder object constructed with default parameters") {
    constexpr size_t buffer_size = 254UL;
    constexpr size_t encoded_buffer_size = 255UL;
    PF::Driver::Serial::Backend::COBSEncoder cobs_encoder{};
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<encoded_buffer_size> output_buffer;
    PF::IndexStatus push_status;
    const PF::IndexStatus push_ok = PF::IndexStatus::ok;

    REQUIRE(output_buffer.empty() == true);

    WHEN("transform is called on an input buffer that contains a null byte") {
      push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == push_ok);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x01\x01'") {
        auto expected = std::string("\x01\x01"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x00"s);
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN("transform is called on an input buffer that contains these bytes '0x00 0x00'") {
      auto body = std::string("\x00\x00"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x01\x01\x01'") {
        auto expected = std::string("\x01\x01\x01"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '0x11 0x22 0x00 0x33'") {
      auto body = std::string("\x11\x22\x00\x33"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x03\x11\x22\x02\x33'") {
        auto expected = std::string("\x03\x11\x22\x02\x33"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains these bytes '\x11\x22\x33\x44'") {
      auto body = std::string("\x11\x22\x33\x44"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x05\x11\x22\x33\x44'") {
        auto expected = std::string("\x05\x11\x22\x33\x44"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains the char 'x'") {
      auto body = std::string("x"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x02\x78'") {
        auto expected = std::string("\x02\x78"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer that contains the char 'xy'") {
      auto body = std::string("xy"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x03\x78\x79'") {
        auto expected = std::string("\x03\x78\x79"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on an input buffer containing the string 'Hello world'") {
      auto body = std::string("Hello World"s);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer is as expected '\x0cHello World'") {
        auto expected = std::string("\x0cHello World"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN(
        "transform is called on an input buffer that contains a 16-byte sensor measurements "
        "message payload") {
      auto body = std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer has an expected sequence of 17 bytes") {
        auto expected =
            std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42"s);
        REQUIRE(output_buffer == expected);
      }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }

    WHEN("transform is called on a 254 byte buffer with last byte as the delimiter") {
      for (size_t i = 0; i < 253; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);
      REQUIRE(input_buffer.size() == 254);
      PF::Util::ByteVector<buffer_size> expected;
      expected.copy_from(input_buffer.buffer(), input_buffer.size());

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") { REQUIRE(status == PF::IndexStatus::ok); }
      THEN("The output buffer has an expected sequence of 255 bytes with first byte as 0xfe") {
        REQUIRE(output_buffer.operator[](0) == 0xfe);
        for (size_t i = 1; i < 254; i++) {
          REQUIRE(output_buffer.operator[](i) == 10);
        }
        REQUIRE(output_buffer.operator[](254) == 1);
      }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == expected); }
    }

    WHEN("transform is called on a buffer filled with 254 non-delimiter bytes") {
      for (size_t i = 0; i < 254; i++) {
        uint8_t val = 10;
        push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }
      REQUIRE(input_buffer.size() == 254);
      PF::Util::ByteVector<buffer_size> expected;
      expected.copy_from(input_buffer.buffer(), input_buffer.size());

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports out_of_bounds status") {
        REQUIRE(status == PF::IndexStatus::out_of_bounds);
      }
      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == expected); }
    }
  }
}

SCENARIO(
    "Serial::The FrameReceiver class correctly generates COBS decoded bodies from encoded buffers",
    "[framereceiver]") {
  GIVEN("A FrameReceiver object constructed with default parameters") {
    constexpr size_t buffer_size = 255UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    using TestFrameProps = PF::Driver::Serial::Backend::FrameProps;
    TestFrameProps::PayloadBuffer output_buffer;

    PF::Driver::Serial::Backend::FrameReceiver frame_receiver{};

    WHEN("255 non-delimiter frame body is passed as input and output is called after that") {
      for (size_t i = 0; i < buffer_size; ++i) {
        uint8_t val = 10;
        auto input_status = frame_receiver.input(val);
        THEN("The input method reports ok status for all 255 non delimiter") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }

      auto output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::waiting);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("256 non-delimiter frame body is passed as input and output is called after that") {
      for (size_t i = 0; i < buffer_size; ++i) {
        uint8_t val = 10;
        auto input_status = frame_receiver.input(val);
        THEN(
            "The input method reports ok status for all 255 non delimiter bytes and invalid_length "
            "for 256th byte") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }
      uint8_t val = 10;
      auto input_status = frame_receiver.input(val);
      THEN(
          "The input method reports ok status for all 255 non delimiter bytes and invalid_length "
          "for 256th byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::invalid_length);
      }
      auto output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports invalid_length status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::invalid_length);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN(
        "128 non-delimiter frame body followed by a delimiter are passed as input, and output is "
        "called after that") {
      PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
      uint8_t val = 0x80;
      input_status = frame_receiver.input(val);
      REQUIRE(input_status == TestFrameProps::InputStatus::ok);

      for (size_t i = 1; i < 128; ++i) {
        uint8_t val = 10;
        input_status = frame_receiver.input(val);
        THEN(
            "The input method reports ok status for all 128 non delimiter bytes and output_ready "
            "for 129th byte") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }

      input_status = frame_receiver.input(0);
      THEN(
          "The input method reports ok status for all 128 non delimiter bytes and output_ready for "
          "129th byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      auto output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        for (size_t i = 0; i < 127; ++i) {
          uint8_t val = 10;
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }

    WHEN(
        "256 bytes are passed as input, where the input's delimiter bytes are at positions 129 and "
        "256, and then output is called after that") {
      PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
      uint8_t val = 10;
      for (size_t i = 0; i < 128; i++) {
        input_status = frame_receiver.input(val);
        REQUIRE(input_status == TestFrameProps::InputStatus::ok);
      }

      input_status = frame_receiver.input(0);
      THEN(
          "The input method reports ok status for all 128 non delimiter bytes and output_ready for "
          "129th byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      input_status = frame_receiver.input(0x80);
      THEN(
          "Then input method reports input_overwritten on the next input byte and ok status for "
          "subsequent bytes") {
        REQUIRE(input_status == TestFrameProps::InputStatus::input_overwritten);
      }

      for (size_t i = 0; i < 127; i++) {
        input_status = frame_receiver.input(val);
        REQUIRE(input_status == TestFrameProps::InputStatus::ok);
      }

      input_status = frame_receiver.input(0);
      THEN("The input method reports output ready status for the delimiter byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }
      auto output_status = frame_receiver.output(output_buffer);

      THEN("The output method reports ok status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::ok);
      }
      THEN(
          "The output buffer has an expected sequence of 128 bytes of the input data passed after "
          "the 129th delimiter byte") {
        for (size_t i = 0; i < 127; i++) {
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }

    WHEN(
        "A 10-byte frame body with improper encoding followed by a delimiter is passed as input "
        "and output is called after that") {
      PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
      auto data =
          PF::Util::make_array<uint8_t>(0x0c, 0x25, 0x39, 0xbb, 0x30, 0xf1, 0x6b, 0x62, 0x95, 0xfd);
      for (auto& bytes : data) {
        input_status = frame_receiver.input(bytes);
        THEN("The input method reports ok status for all the non-delimiter bytes") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }

      input_status = frame_receiver.input(0);
      THEN("The input method reports output ready status for the delimiter byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      auto output_status = frame_receiver.output(output_buffer);

      THEN("The output method reports invalid_cobs status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::invalid_cobs);
      }

      THEN("The output buffer remains empty") { REQUIRE(output_buffer.empty() == true); }
    }
  }
}

SCENARIO(
    "After output is consumed, FrameReceiver's behavior resets to that of a FramReceiver with an "
    "empty buffer",
    "[framereceiver]") {
  GIVEN(
      "A FrameReceiver object constructed with default parameters, is given a 255 non-delimiter "
      "frame body followed by a delimiter and output is called after that") {
    constexpr size_t buffer_size = 255UL;
    using TestFrameProps = PF::Driver::Serial::Backend::FrameProps;
    PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
    PF::Driver::Serial::Backend::FrameProps::OutputStatus output_status;
    PF::Driver::Serial::Backend::FrameReceiver frame_receiver{};
    TestFrameProps::PayloadBuffer output_buffer;

    uint8_t val = 10;
    input_status = frame_receiver.input(0xff);
    REQUIRE(input_status == TestFrameProps::InputStatus::ok);
    for (size_t i = 0; i < buffer_size - 1; ++i) {
      input_status = frame_receiver.input(val);
      REQUIRE(input_status == TestFrameProps::InputStatus::ok);
    }

    input_status = frame_receiver.input(0);
    REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);

    output_status = frame_receiver.output(output_buffer);
    REQUIRE(output_status == TestFrameProps::OutputStatus::ok);

    for (size_t i = 0; i < buffer_size - 1; ++i) {
      REQUIRE(output_buffer.operator[](i) == val);
    }

    WHEN(
        "254-byte frame body is given as input followed by a delimiter, and output is called after "
        "that") {
      input_status = frame_receiver.input(0xff);
      REQUIRE(input_status == TestFrameProps::InputStatus::ok);
      for (size_t i = 0; i < buffer_size - 1; ++i) {
        input_status = frame_receiver.input(val);
        THEN("The input method reports ok status for all the non-delimiter bytes") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }

      input_status = frame_receiver.input(0);
      THEN("The input method reports output ready status for the delimiter byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::ok);
      }

      THEN("The output buffer has the expected sequence of 254 bytes") {
        for (size_t i = 0; i < buffer_size - 1; ++i) {
          REQUIRE(output_buffer.operator[](i) == val);
        }
      }
    }
  }

  GIVEN(
      "A FrameReceiver object constructed with default parameters, and a 254-byte frame body is "
      "given as input") {
    constexpr size_t buffer_size = 254UL;
    using TestFrameProps = PF::Driver::Serial::Backend::FrameProps;
    PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
    PF::Driver::Serial::Backend::FrameProps::OutputStatus output_status;
    TestFrameProps::PayloadBuffer output_buffer;

    PF::Driver::Serial::Backend::FrameReceiver frame_receiver{};
    uint8_t val = 10;
    input_status = frame_receiver.input(0xfe);
    REQUIRE(input_status == TestFrameProps::InputStatus::ok);
    for (size_t i = 0; i < buffer_size - 1; ++i) {
      input_status = frame_receiver.input(val);
      REQUIRE(input_status == TestFrameProps::InputStatus::ok);
    }

    WHEN("The output method is called, after which a 254-byte frame body is given as input") {
      output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::waiting);
      }

      for (size_t i = 0; i < buffer_size; ++i) {
        input_status = frame_receiver.input(val);
        THEN("The input method reports ok status") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }
    }

    WHEN(
        "A delimiter is passed as input and output method is called, after which a 254-byte frame "
        "body is given as input") {
      input_status = frame_receiver.input(0);
      THEN("The input method reports output ready status for the delimiter byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports ok status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::ok);
      }

      for (size_t i = 0; i < buffer_size; ++i) {
        input_status = frame_receiver.input(val);
        THEN("The input method reports ok status") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }
    }

    WHEN(
        "2 non-delimiter bytes are passed as input and output method is called, after which a "
        "254-byte frame body is given as input") {
      input_status = frame_receiver.input(val);
      REQUIRE(input_status == TestFrameProps::InputStatus::ok);
      input_status = frame_receiver.input(val);
      REQUIRE(input_status == TestFrameProps::InputStatus::invalid_length);

      output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports invalid_length status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::invalid_length);
      }

      for (size_t i = 0; i < buffer_size; ++i) {
        input_status = frame_receiver.input(val);
        THEN("The input method reports ok status") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }
    }
  }
}

SCENARIO(
    "Serial::The FrameSender class correctly encodes frame bodies and appends a delimiter",
    "[frameresender]") {
  GIVEN("A FrameReceiver object constructed with default parameters") {
    constexpr size_t buffer_size = 254UL;
    using TestFrameProps = PF::Driver::Serial::Backend::FrameProps;
    TestFrameProps::PayloadBuffer input_buffer;
    TestFrameProps::ChunkBuffer output_buffer;

    PF::Driver::Serial::Backend::FrameSender frame_sender{};

    WHEN("transform is called on a 254-byte frame body") {
      uint8_t val = 10;
      for (size_t i = 0; i < buffer_size; i++) {
        auto push_status = input_buffer.push_back(val);
        REQUIRE(push_status == PF::IndexStatus::ok);
      }

      auto transform_status = frame_sender.transform(input_buffer, output_buffer);
      THEN("The transform method reports invalid_length status") {
        REQUIRE(transform_status == TestFrameProps::OutputStatus::invalid_length);
      }

      THEN("The output buffer has an expected sequence of 256 bytes with no delimiters") {
        REQUIRE(output_buffer.operator[](0) == 0xff);
        for (size_t i = 1; i < buffer_size; i++) {
          REQUIRE(output_buffer.operator[](i) == val);
        }
        REQUIRE(output_buffer.operator[](255) == 0x01);
      }
      THEN("The input buffer is unchanged after transform") {
        for (size_t i = 0; i < buffer_size; i++) {
          REQUIRE(input_buffer.operator[](i) == val);
        }
      }
    }
  }
}