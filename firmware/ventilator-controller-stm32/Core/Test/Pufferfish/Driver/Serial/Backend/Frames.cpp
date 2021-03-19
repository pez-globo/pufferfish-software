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

SCENARIO(
    "Serial::The CobsDecoder class correctly decodes payloads with COBS to payloads", "[Backend]") {
  GIVEN("A CobsDecoder object constructed with default parameters") {
    constexpr size_t buffer_size = 254UL;
    PF::Driver::Serial::Backend::COBSDecoder cobs_decoder{};
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<buffer_size> output_buffer;

    REQUIRE(output_buffer.empty() == true);

    WHEN("Input to the CobsDecoder is '0x00 0x00'") {
      auto body = std::string("\x01\x01", 2);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x00", 1);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is '\x01\x01\x01'") {
      auto body = std::string("\x01\x01\x01", 3);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x00\x00", 2);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is '\x03\x11\x22\x02\x33'") {
      auto body = std::string("\x03\x11\x22\x02\x33", 5);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x11\x22\x00\x33", 4);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is '\x05\x11\x22\x33\x44'") {
      auto body = std::string("\x05\x11\x22\x33\x44", 5);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x11\x22\x33\x44", 4);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is char '\x02\x78'") {
      auto body = std::string("\x02\x78", 2);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("x", 1);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is char '\x03\x78\x79'") {
      auto body = std::string("\x03\x78\x79", 3);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("xy", 2);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is string '\x0cHello World'") {
      auto body = std::string("\x0cHello World", 12);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("Hello World", 11);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsDecoder is a encoded sensor measurements message payload") {
      auto body =
          std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42", 17);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_decoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected =
            std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42", 16);
        REQUIRE(output_buffer == expected);
      }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
  }
}

SCENARIO("Serial::The CobsEncoder class correctly encodes payloads with COBS", "[Backend]") {
  GIVEN("A CobsEncoder object constructed with default parameters") {
    constexpr size_t buffer_size = 254UL;
    PF::Driver::Serial::Backend::COBSEncoder cobs_encoder{};
    PF::Util::ByteVector<buffer_size> input_buffer;
    PF::Util::ByteVector<buffer_size> output_buffer;

    REQUIRE(output_buffer.empty() == true);

    WHEN("Input buffer size is greater than the payload max size (254 bytes)") {
      for (size_t i = 0; i < 255; ++i) {
        uint8_t val = 10;
        input_buffer.push_back(val);
      }
      auto status = cobs_encoder.transform(input_buffer, output_buffer);
      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::invalid_length);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }
    WHEN(
        "The size of the output buffer is smaller than the size required for an encoded input "
        "buffer") {
      constexpr size_t buffer_size = 11UL;
      auto body = std::string("\x45\x00\x00\x2c\x4c\x79\x00\x00\x40\x06\x4f\x37", 12);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      PF::Util::ByteVector<buffer_size> output_buffer;
      auto status = cobs_encoder.transform(input_buffer, output_buffer);
      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::invalid_length);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }
    WHEN("The input to the CobsEncoder contains multiple delimited bytes") {
      auto body = std::string("\x01\x02\x00\x00\x03\x04\x00\x00", 8);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);
      auto status = cobs_encoder.transform(input_buffer, output_buffer);
      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer remains unchanged") {
        auto expected = std::string("\x03\x01\x02\x01\x03\x03\x04\x01\x01", 9);
        REQUIRE(output_buffer == expected);
      }
    }
    WHEN("Input to the cobsEncoder is a null byte") {
      input_buffer.push_back(0x00);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x01\x01", 2);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") {
        auto expected = std::string("\x00", 1);
        REQUIRE(input_buffer == expected);
      }
    }
    WHEN("Input to the cobsEncoder is '0x00 0x00'") {
      input_buffer.push_back(0x00);
      input_buffer.push_back(0x00);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x01\x01\x01", 3);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") {
        auto expected = std::string("\x00\x00", 2);
        REQUIRE(input_buffer == expected);
      }
    }
    WHEN("Input to the cobsEncoder is '0x11 0x22 0x00 0x33'") {
      auto body = std::string("\x11\x22\x00\x33", 4);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x03\x11\x22\x02\x33", 5);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the cobsEncoder is '\x11\x22\x33\x44'") {
      auto body = std::string("\x11\x22\x33\x44", 4);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x05\x11\x22\x33\x44", 5);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the cobsEncoder is char 'x'") {
      auto body = std::string("x", 1);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x02\x78", 2);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the cobsEncoder is char 'xy'") {
      auto body = std::string("xy", 2);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x03\x78\x79", 3);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the cobsEncoder is string 'Hello world'") {
      auto body = std::string("Hello World", 11);
      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected = std::string("\x0cHello World", 12);
        REQUIRE(output_buffer == expected);
      }
      THEN("Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
    WHEN("Input to the CobsEncoder is a sensor measurements message payload") {
      auto body =
          std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42", 16);

      PF::Util::convert_string_to_byte_vector(body, input_buffer);

      auto status = cobs_encoder.transform(input_buffer, output_buffer);

      THEN("The transform method reports ok status") {
        REQUIRE(status == PF::Driver::Serial::Backend::FrameProps::OutputStatus::ok);
      }
      THEN("The output buffer is as expected") {
        auto expected =
            std::string("\x03\x02\x25\x01\x04\xF0\x41\x35\x01\x04\xAA\x42\x3D\x01\x03\x90\x42", 17);
        REQUIRE(output_buffer == expected);
      }
      THEN("The Input buffer is unchanged after transform") { REQUIRE(input_buffer == body); }
    }
  }
}

SCENARIO(
    "Serial::The FrameReceiver class correctly generates COBS decoded bodies from encoded buffers",
    "[Backend]") {
  GIVEN("A FrameReceiver object constructed with default parameters") {
    constexpr size_t buffer_size = 255UL;
    PF::Util::ByteVector<buffer_size> input_buffer;
    using TestFrameProps = PF::Driver::Serial::Backend::FrameProps;
    TestFrameProps::PayloadBuffer output_buffer;

    PF::Driver::Serial::Backend::FrameReceiver frame_receiver{};

    WHEN("255 non-delimiter bytes are passed as input and output is called after that") {
      for (size_t i = 0; i < buffer_size; ++i) {
        uint8_t val = 10;
        auto input_status = frame_receiver.input(val);
        THEN("The input method reports ok status for all 255 non delimiter bytes") {
          REQUIRE(input_status == TestFrameProps::InputStatus::ok);
        }
      }

      auto output_status = frame_receiver.output(output_buffer);
      THEN("The output method reports waiting status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::waiting);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }

    WHEN("256 non-delimiter bytes are passed as input and output is called after that") {
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
        "128 non-delimiter bytes with COBS byte are passed as input, 129th byte is passed as a "
        "delimiter, and output is called after that") {
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

    WHEN("Encoded payload is passed") {
      PF::Driver::Serial::Backend::FrameProps::InputStatus input_status;
      auto data = PF::Util::make_array<uint8_t>(0x03, 0xc0, 0xf7, 0x03, 0xfa, 0x43);
      for (auto& bytes : data) {
        input_status = frame_receiver.input(bytes);
        REQUIRE(input_status == TestFrameProps::InputStatus::ok);
      }

      input_status = frame_receiver.input(0);
      THEN(
          "The input method reports ok status for all 128 non delimiter bytes and output_ready for "
          "129th byte") {
        REQUIRE(input_status == TestFrameProps::InputStatus::output_ready);
      }

      input_status = frame_receiver.input(data[0]);
      REQUIRE(input_status == TestFrameProps::InputStatus::input_overwritten);
      for (size_t i = 1; i < data.size(); i++)
      {
        input_status = frame_receiver.input(data[i]);
        REQUIRE(input_status == TestFrameProps::InputStatus::ok);        
      }
      
      input_status = frame_receiver.input(0);
      auto output_status = frame_receiver.output(output_buffer);

      THEN("The output method reports ok status") {
        REQUIRE(output_status == TestFrameProps::OutputStatus::ok);
      }
      REQUIRE(output_buffer.empty() == true);
    }
  }
}