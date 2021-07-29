/// TestFrameReceiver.cpp
/// Unit tests to confirm behavior of FrameReceiver checks.

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Pufferfish/Driver/Serial/Nonin/FrameReceiver.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;
using PF::Driver::Serial::Nonin::Frame;
using PF::Util::Containers::make_array;

const PF::Driver::Serial::Nonin::FrameInputStatus input_invalid_header =
    PF::Driver::Serial::Nonin::FrameInputStatus::invalid_header;
const PF::Driver::Serial::Nonin::FrameInputStatus input_invalid_checksum =
    PF::Driver::Serial::Nonin::FrameInputStatus::invalid_checksum;

const PF::Driver::Serial::Nonin::FrameInputStatus input_output_ready =
    PF::Driver::Serial::Nonin::FrameInputStatus::output_ready;

const PF::Driver::Serial::Nonin::FrameInputStatus input_ok =
    PF::Driver::Serial::Nonin::FrameInputStatus::ok;

const PF::Driver::Serial::Nonin::FrameOutputStatus output_waiting =
    PF::Driver::Serial::Nonin::FrameOutputStatus::waiting;

const PF::Driver::Serial::Nonin::FrameOutputStatus output_ok =
    PF::Driver::Serial::Nonin::FrameOutputStatus::ok;

SCENARIO("FrameReceiver Input for 20 bytes of data.", "[NoninOEM3]") {
  GIVEN("20 bytes of valid data") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameOutputStatus frame_output_status;
    Frame frame_output;
    auto input_data = make_array<uint8_t>(
        0x01,
        0x81,
        0x01,
        0x00,
        0x83,
        0x01,
        0x80,
        0x01,
        0x48,
        0xCA,
        0x01,
        0x80,
        0x01,
        0x61,
        0xE3,
        0x01,
        0x80,
        0x01,
        0x30,
        0xB2);

    WHEN("On input of 4 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 0; index < 4; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == input_ok);
        }
      }
    }
    WHEN("On input of 5th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 4; index++) {
        frame_receiver.input(input_data[index]);
      }
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[4]);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }

    WHEN("On input of 6 to 9 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 5; index++) {
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_ok);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 5; index < 9; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == input_ok);
        }
      }
    }
    WHEN("On input of 10th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 9; index++) {
        if (index == 5) {
          frame_receiver.output(frame_output);
        }
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[9]);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }

    WHEN("On input of 11 to 14 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 10; index++) {
        if (index == 5) {
          frame_receiver.output(frame_output);
        }
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_ok);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 10; index < 14; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == input_ok);
        }
      }
    }
    WHEN("On input of 15th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 14; index++) {
        if (index % 5 == 0) {
          frame_receiver.output(frame_output);
        }
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[14]);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }

    WHEN("On input of 16 to 19 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 15; index++) {
        if (index % 5 == 0) {
          frame_receiver.output(frame_output);
        }
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_ok);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 15; index < 19; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == input_ok);
        }
      }
    }
    WHEN("On input of 10th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 19; index++) {
        if (index % 5 == 0) {
          frame_receiver.output(frame_output);
        }
        frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frame_output);
      REQUIRE(frame_output_status == output_waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[19]);
        REQUIRE(frame_input_status == input_output_ready);
        frame_output_status = frame_receiver.output(frame_output);
        REQUIRE(frame_output_status == output_ok);
      }
    }
  }
}

SCENARIO("FrameReceiver for first frames of data received ", "[NoninOEM3]") {
  GIVEN("A 4 bytes of valid data INPUT_DATA : 0x01 0x81 0x01 0x00 ") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameInputStatus frame_input_status;
    // PF::Driver::Serial::Nonin::FrameOutputStatus frame_output_status;
    auto input_data = make_array<uint8_t>(0x01, 0x81, 0x01, 0x00);

    WHEN("On input of 4 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("On FrameReceiver.input() for 4 Byte data shall return not available") {
        for (unsigned char index : input_data) {
          frame_input_status = frame_receiver.input(index);
          REQUIRE(frame_input_status == input_ok);
        }
      }
    }
  }

  GIVEN("A 7 bytes data INPUT_DATA : 0x01 0x81 0x01 0x81 0x01 0x00 0x83 ") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameOutputStatus frame_output_status;
    Frame frame_output;
    auto input_data = make_array<uint8_t>(0x01, 0x81, 0x01, 0x81, 0x01, 0x00, 0x83);
    WHEN("On input of 6 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("OFrameReceiver.input()' for 6 Byte data from INPUT_DATA shall return waiting") {
        for (uint8_t index = 0; index < 6; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          if (index < 6) {
            REQUIRE(frame_input_status == input_ok);
          }
        }
      }
    }

    WHEN("On input of 7th byte to FrameReceiver ") {
      for (uint8_t index = 0; index < 6; index++) {
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      THEN("On Invoking 'FrameReceiver.input()' for 7th byte data from INPUT_DATA.") {
        frame_input_status = frame_receiver.input(input_data[6]);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }
    AND_WHEN("'FrameReceiver.input()' status is available on receiving 7 bytes of data") {
      for (unsigned char index : input_data) {
        frame_input_status = frame_receiver.input(index);
      }
      REQUIRE(frame_input_status == input_output_ready);
      THEN("Invoke FrameReceiver::output shall return available") {
        frame_output_status = frame_receiver.output(frame_output);
        REQUIRE(frame_output_status == output_ok);
        REQUIRE(frame_output[0] == input_data[2]);
        REQUIRE(frame_output[1] == input_data[3]);
        REQUIRE(frame_output[2] == input_data[4]);
        REQUIRE(frame_output[3] == input_data[5]);
        REQUIRE(frame_output[4] == input_data[6]);
      }
    }
  }

  GIVEN("A 6 bytes INPUT_DATA 0x01 0x81 0x01 0x81 0x01 0x00") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameOutputStatus frame_output_status;
    Frame frame_output;
    auto input_data = make_array<uint8_t>(0x01, 0x81, 0x01, 0x81, 0x01, 0x00, 0x83);
    WHEN("On input of 6 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("FrameReceiver.input() for 6 Byte data from INPUT_DATA shall return waiting") {
        for (uint8_t index = 0; index < 6; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          if (index < 6) {
            REQUIRE(frame_input_status == input_ok);
          }
        }
      }
    }
    AND_WHEN("'FrameReceiver.input()' status is waiting on receiving 6 bytes of data") {
      for (uint8_t index = 0; index < 6; index++) {
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      REQUIRE(frame_input_status == input_ok);
      THEN("Invoke FrameReceiver::output shall return waiting") {
        frame_output_status = frame_receiver.output(frame_output);
        REQUIRE(frame_output_status == output_waiting);
      }
    }
  }
}

SCENARIO("Validate function 'validate_frame_header' ", "[NoninOEM3]") {
  Frame frame_buffer{};
  uint8_t checksum = 0;
  GIVEN("A valid first frame") {
    frame_buffer = {0x01, 0x81, 0x02, 0x00, 0x84};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x84") {
      THEN("validate_frame_header() shall return available") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x81U) == 0x81)));
        checksum = (frame_buffer[0] + frame_buffer[1] + frame_buffer[2] + frame_buffer[3]) % 256;
        REQUIRE(frame_buffer[4] == checksum);
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame_header(frame_buffer, 0x81) == true);
      }
    }
  }

  GIVEN("A valid frame other than first frame") {
    frame_buffer = {0x01, 0x80, 0x02, 0x00, 0x83};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x83") {
      THEN("validate_frame_header() shall return available") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x81U) != 0x81)));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame_header(frame_buffer, 0x81) == false);
      }
    }
  }

  GIVEN("A Frame having invalid checksum") {
    frame_buffer = {0x01, 0x81, 0x02, 0x00, 0x00};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x00") {
      THEN("validate_frame_checksum() shall return invalid_checksum") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x81U) == 0x81)));
        checksum = (frame_buffer[0] + frame_buffer[1] + frame_buffer[2] + frame_buffer[3]) % 256;
        REQUIRE(frame_buffer[4] != checksum);
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame_checksum(frame_buffer) == false);
      }
    }
  }

  GIVEN("A Frame having invalid status Byte") {
    frame_buffer = {0x01, 0x7F, 0x02, 0x00, 0x82};
    WHEN("FRAME DATA : 0x01  0x7F 0x02  0x00  0x82") {
      THEN("validate_frame_header()' shall return invalid_checksum") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x81U) != 0x81)));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame_header(frame_buffer, 0x81) == false);
      }
    }
  }
}

SCENARIO("Validate function 'validate_frame' ", "[NoninOEM3]") {
  Frame frame_buffer{};
  PF::Driver::Serial::Nonin::FrameInputStatus frame_input_status;
  uint8_t checksum = 0;
  GIVEN("A valid first frame") {
    frame_buffer = {0x01, 0x81, 0x02, 0x00, 0x84};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x84") {
      THEN("validate_frame()' shall return available") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x80U) == 0x80)));
        checksum = (frame_buffer[0] + frame_buffer[1] + frame_buffer[2] + frame_buffer[3]) % 256;
        REQUIRE(frame_buffer[4] == checksum);
        frame_input_status = PF::Driver::Serial::Nonin::validate_frame(frame_buffer);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }
  }

  GIVEN("A valid frame other than first frame") {
    frame_buffer = {0x01, 0x80, 0x02, 0x00, 0x83};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x83") {
      THEN("validate_frame() shall return available") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x80U) == 0x80)));
        checksum = (frame_buffer[0] + frame_buffer[1] + frame_buffer[2] + frame_buffer[3]) % 256;
        REQUIRE(frame_buffer[4] == checksum);
        frame_input_status = PF::Driver::Serial::Nonin::validate_frame(frame_buffer);
        REQUIRE(frame_input_status == input_output_ready);
      }
    }
  }

  GIVEN("A Frame having invalid checksum") {
    frame_buffer = {0x01, 0x80, 0x02, 0x00, 0x00};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x00") {
      THEN("On 'validate_frame()' shall return invalid_checksum") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x80U) == 0x80)));
        checksum = (frame_buffer[0] + frame_buffer[1] + frame_buffer[2] + frame_buffer[3]) % 256;
        REQUIRE(frame_buffer[4] != checksum);
        frame_input_status = PF::Driver::Serial::Nonin::validate_frame(frame_buffer);
        REQUIRE(frame_input_status == input_invalid_checksum);
      }
    }
  }

  GIVEN("A Frame having invalid status Byte") {
    frame_buffer = {0x01, 0x7F, 0x02, 0x00, 0x82};
    WHEN("FRAME DATA : 0x01  0x7F 0x02  0x00  0x82") {
      THEN("On validate_frame() shall return invalid_header") {
        REQUIRE(((frame_buffer[0] == 0x01) && ((frame_buffer[1] & 0x80U) != 0x80)));
        frame_input_status = PF::Driver::Serial::Nonin::validate_frame(frame_buffer);
        REQUIRE(frame_input_status == input_invalid_header);
      }
    }
  }
}
