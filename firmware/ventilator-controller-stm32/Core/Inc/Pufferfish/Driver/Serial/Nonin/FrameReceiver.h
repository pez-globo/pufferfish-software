/// FrameReceiver.h
/// this file has class and methods to take input from FrameBuffer and output the frame on availability.
///
/// More detailed description for the file, if necessary;
/// perhaps spanning multiple lines.

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

#pragma once

#include <Pufferfish/Driver/Serial/Nonin/FrameBuffer.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/*
 * FrameSplitter reads input data from sensor and Output the frame on available
 * on error throw error on output frame.
 */
class FrameReceiver {
 public:
  /* FrameReceiver input status return values */
  enum class FrameInputStatus {
    waiting = 0,      /// Input is ready to receive new bytes of sensor data
    notAvailable,     /// Input status is not available or error in frame
    framingError,     /// Error in checksum or status byte or in byte 1 of a frame
    available         /// frame is available
    };

  /* FrameReceiver output status return values */
  enum class FrameOutputStatus {
    available = 0,    /// frame output is available for packet fill
    waiting           /// frame output is waiting for complete frame
    };

  /**
   * Constructor for FrameReceiver
   */
  FrameReceiver() {
  }

  /**
   * @brief  Call input to fill frame until it returns available, then call output
   * @param  New byte data received from sensor
   * @return Frame input status on filling a frame
   */
  FrameInputStatus input(const uint8_t newByte);

  /**
   * @brief  output method updates the frame on available or throws error/waiting
   * @param  outputBuffer to receive frame from frame receiver
   * @param  frameIndex index of frame in a packet
   * @param  status byte of frame
   * @return Frame output status after validating the frame
   */
  FrameOutputStatus output(Frame &frame);

 private:
  FrameInputStatus updateFrameBuffer(uint8_t newByte);

  /* Frame input status */
  FrameInputStatus inputStatus;

  FrameBuffer frameBuf;

  /* Variable that validates the start of frame
     true : Start of frame packet is available reading the consicutive frame are possible
     false: Start of frame packet is not available , false is set in the beginning of reading bytes
            and on there is loss of bytes in a frame or noise occurred in recived frame due to which
            the validation of start of frame is called */
  bool startOfFrameStatus = false;

};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
