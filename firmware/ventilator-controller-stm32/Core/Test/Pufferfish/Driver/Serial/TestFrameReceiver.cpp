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

#include "catch2/catch.hpp"
#include "Pufferfish/Driver/Serial/Nonin/FrameReceiver.h"

namespace PF = Pufferfish;

SCENARIO("Validate FrameReceiver::Input for 20 bytes of valid data.", "[NoninOEM3]") {

  GIVEN("A 20 bytes of valid data\n"
      "INPUT_DATA :\n"
      "0x01, 0x81, 0x01, 0x00, 0x83\n"
      "0x01, 0x80, 0x01, 0x48, 0xCA\n"
      "0x01, 0x81, 0x01, 0x00, 0x83\n"
      "0x01, 0x80, 0x01, 0x50, 0xD2\n") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus frame_output_status;
    Frame frameOutput;
    const uint8_t input_data[20] = {0x01, 0x81, 0x01, 0x00, 0x83, 0x01, 0x80, 0x01,
        0x48, 0xCA, 0x01, 0x81, 0x01, 0x00, 0x83, 0x01, 0x80, 0x01, 0x50, 0xD2};

    WHEN("On input of 4 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 0; index < 4; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
        }
      }
    }
    WHEN("On input of 5th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 4; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
      }
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[4]);
        REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }

    WHEN("On input of 6 to 9 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 5; index++) {
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::available);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 5; index < 9; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
        }
      }
    }
    WHEN("On input of 10th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 9; index++) {
        if (index == 5) {
          frame_output_status = frame_receiver.output(frameOutput);
        }
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[9]);
        REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }

    WHEN("On input of 11 to 14 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 10; index++) {
        if (index == 5) {
          frame_output_status = frame_receiver.output(frameOutput);
        }
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::available);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 10; index < 14; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
        }
      }
    }
    WHEN("On input of 15th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 14; index++) {
        if (index % 5 == 0) {
          frame_output_status = frame_receiver.output(frameOutput);
        }
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[14]);
        REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }

    WHEN("On input of 16 to 19 bytes of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 15; index++) {
        if (index % 5 == 0) {
          frame_output_status = frame_receiver.output(frameOutput);
        }
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::available);
      THEN("frame_receiver::input should return waiting") {
        for (uint8_t index = 15; index < 19; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
        }
      }
    }
    WHEN("On input of 10th byte of INPUT_DATA to FrameReceiver ") {
      for (uint8_t index = 0; index < 19; index++) {
        if (index % 5 == 0) {
          frame_output_status = frame_receiver.output(frameOutput);
        }
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      frame_output_status = frame_receiver.output(frameOutput);
      REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::waiting);
      THEN("frame_receiver::input should return available") {
        frame_input_status = frame_receiver.input(input_data[19]);
        REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
        frame_output_status = frame_receiver.output(frameOutput);
        REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::available);
      }
    }
  }
}


//
SCENARIO("Validate FrameReceiver::Input and FrameReceiver::Output for first frames of data received ", "[NoninOEM3]") {

  GIVEN("A 4 bytes of valid data\n"
      "INPUT_DATA : 0x01 0x81 0x01 0x00 ") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus frame_output_status;
    uint8_t input_data[4] = {0x01, 0x81, 0x01, 0x00};

    WHEN("On input of 4 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("On Invoking 'FrameReceiver.input()' for first 4 input Byte data from INPUT_DATA shall return not available") {
        for (uint8_t index = 0; index < 4; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
        }
      }
    }
  }

  GIVEN("A 7 bytes of valid data contains complete first frame\n"
      "INPUT_DATA : 0x01 0x81\n"
      "0x01 0x81 0x01 0x00 0x83 ") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus frame_output_status;
    Frame frameOutput;
    uint8_t input_data[7] = {0x01, 0x81, 0x01, 0x81, 0x01, 0x00, 0x83};
    WHEN("On input of 6 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("On Invoking 'FrameReceiver.input()' for first 6 input Byte data from INPUT_DATA shall return waiting") {
        for (uint8_t index = 0; index < 6; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          if (index < 6) {
            REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
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
        REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }
    AND_WHEN("'FrameReceiver.input()' status is available on receiving 7 bytes of data") {
      for (uint8_t index = 0; index < 7; index++) {
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      THEN("Invoke FrameReceiver::output shall return available") {
        frame_output_status = frame_receiver.output(frameOutput);
        REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::available);
        REQUIRE(frameOutput[0] == input_data[2]);
        REQUIRE(frameOutput[1] == input_data[3]);
        REQUIRE(frameOutput[2] == input_data[4]);
        REQUIRE(frameOutput[3] == input_data[5]);
        REQUIRE(frameOutput[4] == input_data[6]);
      }
    }
  }

  GIVEN("A 6 bytes of valid data does not contain complete first frame\n"
      "INPUT_DATA : 0x01 0x81 0x01 0x81 0x01 0x00") {
    PF::Driver::Serial::Nonin::FrameReceiver frame_receiver;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus frame_input_status;
    PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus frame_output_status;
    Frame frameOutput;
    uint8_t input_data[7] = {0x01, 0x81, 0x01, 0x81, 0x01, 0x00, 0x83};
    WHEN("On input of 6 bytes of INPUT_DATA to FrameReceiver ") {
      THEN("On Invoking 'FrameReceiver.input()' for first 6 input Byte data from INPUT_DATA shall return waiting") {
        for (uint8_t index = 0; index < 6; index++) {
          frame_input_status = frame_receiver.input(input_data[index]);
          if (index < 6) {
            REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
          }
        }
      }
    }
    AND_WHEN("'FrameReceiver.input()' status is waiting on receiving 6 bytes of data") {
      for (uint8_t index = 0; index < 6; index++) {
        frame_input_status = frame_receiver.input(input_data[index]);
      }
      REQUIRE(frame_input_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::waiting);
      THEN("Invoke FrameReceiver::output shall return waiting") {
        frame_output_status = frame_receiver.output(frameOutput);
        REQUIRE(frame_output_status == PF::Driver::Serial::Nonin::FrameReceiver::FrameOutputStatus::waiting);
      }
    }
  }
}

SCENARIO("Validate function 'validate_start_of_frame' ", "[NoninOEM3]") {
  Frame frameBuffer{};

  GIVEN("A valid first frame") {
    frameBuffer = {0x01, 0x81, 0x02, 0x00, 0x84};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x84") {
      THEN("On invoking 'validate_start_of_frame()' for the frame with valid first frame data"
          "'validate_frame()' should return available") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x81) == 0x81)));
        REQUIRE((frameBuffer[4] == (frameBuffer[0] + frameBuffer[1] + frameBuffer[2] + frameBuffer[3]) % 256));
        REQUIRE(PF::Driver::Serial::Nonin::validate_start_of_frame(frameBuffer) == true);
      }
    }
  }

  GIVEN("A valid frame other than first frame") {
    frameBuffer = {0x01, 0x80, 0x02, 0x00, 0x83};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x83") {
      THEN("On invoking 'validate_frame()' for the frame with valid frame data other than first frame"
          "'validate_frame()' should return available") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x81) != 0x81)));
        REQUIRE(PF::Driver::Serial::Nonin::validate_start_of_frame(frameBuffer) == false);
      }
    }
  }

  GIVEN("A Frame having invalid checksum") {
    frameBuffer = {0x01, 0x81, 0x02, 0x00, 0x00};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x00") {
      THEN("On invoking 'validate_frame()' for the frame with invalid checksum data"
          "'validate_frame()' should return framing_error") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x81) == 0x81)));
        REQUIRE((frameBuffer[4] != (frameBuffer[0] + frameBuffer[1] + frameBuffer[2] + frameBuffer[3]) % 256));
        REQUIRE(PF::Driver::Serial::Nonin::validate_start_of_frame(frameBuffer) == false);
      }
    }
  }

  GIVEN("A Frame having invalid status Byte") {
    frameBuffer = {0x01, 0x7F, 0x02, 0x00, 0x82};
    WHEN("FRAME DATA : 0x01  0x7F 0x02  0x00  0x82") {
      THEN("On invoking 'validate_frame()' for the frame with invalid status byte data"
          "'validate_frame()' should return framing_error") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x81) != 0x81)));
        REQUIRE(PF::Driver::Serial::Nonin::validate_start_of_frame(frameBuffer) == false);
      }
    }
  }
}

SCENARIO("Validate function 'validate_frame' ", "[NoninOEM3]") {
  Frame frameBuffer{};

  GIVEN("A valid first frame") {
    frameBuffer = {0x01, 0x81, 0x02, 0x00, 0x84};
    WHEN("FRAME DATA : 0x01  0x81 0x02  0x00  0x84") {
      THEN("On invoking 'validate_frame()' for the frame with valid first frame data"
          "'validate_frame()' should return available") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x80) == 0x80)));
        REQUIRE((frameBuffer[4] == (frameBuffer[0] + frameBuffer[1] + frameBuffer[2] + frameBuffer[3]) % 256));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame(frameBuffer) ==
            PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }
  }

  GIVEN("A valid frame other than first frame") {
    frameBuffer = {0x01, 0x80, 0x02, 0x00, 0x83};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x83") {
      THEN("On invoking 'validate_frame()' for the frame with valid frame data other than first frame"
          "'validate_frame()' should return available") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x80) == 0x80)));
        REQUIRE((frameBuffer[4] == (frameBuffer[0] + frameBuffer[1] + frameBuffer[2] + frameBuffer[3]) % 256));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame(frameBuffer) ==
            PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::available);
      }
    }
  }

  GIVEN("A Frame having invalid checksum") {
    frameBuffer = {0x01, 0x80, 0x02, 0x00, 0x00};
    WHEN("FRAME DATA : 0x01  0x80 0x02  0x00  0x00") {
      THEN("On invoking 'validate_frame()' for the frame with invalid checksum data"
          "'validate_frame()' should return framing_error") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x80) == 0x80)));
        REQUIRE((frameBuffer[4] != (frameBuffer[0] + frameBuffer[1] + frameBuffer[2] + frameBuffer[3]) % 256));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame(frameBuffer) ==
            PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::framing_error);
      }
    }
  }

  GIVEN("A Frame having invalid status Byte") {
    frameBuffer = {0x01, 0x7F, 0x02, 0x00, 0x82};
    WHEN("FRAME DATA : 0x01  0x7F 0x02  0x00  0x82") {
      THEN("On invoking validate_frame() for the frame with invalid status byte data. "
          "validate_frame() should return framing_error") {
        REQUIRE(((frameBuffer[0] == 0x01) && ((frameBuffer[1] & 0x80) != 0x80)));
        REQUIRE(PF::Driver::Serial::Nonin::validate_frame(frameBuffer) ==
            PF::Driver::Serial::Nonin::FrameReceiver::FrameInputStatus::framing_error);
      }
    }
  }
}
