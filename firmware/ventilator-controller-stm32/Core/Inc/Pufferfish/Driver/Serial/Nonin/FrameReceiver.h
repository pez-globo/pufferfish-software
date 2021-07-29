/// FrameReceiver.h
/// This file has class and methods to take input from FrameBuffer
/// and output the frame on availability.

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

#include "Pufferfish/Driver/Serial/Nonin/FrameBuffer.h"
#include "Types.h"

namespace Pufferfish::Driver::Serial::Nonin {

/* FrameReceiver input status return values */
enum class FrameInputStatus {
  ok = 0,            /// Input is ready to receive new bytes of sensor data
  invalid_header,    /// Error in status byte or in byte 1 of a frame
  invalid_checksum,  /// Error in frame checksum
  output_ready       /// frame is available
};

/* FrameReceiver output status return values */
enum class FrameOutputStatus {
  ok = 0,  /// frame output is available for packet fill
  waiting  /// frame output is waiting for complete frame
};

/*
 * FrameSplitter reads input data from sensor and Output the frame on available
 * on error throw error on output frame.
 */
class FrameReceiver {
 public:
  FrameReceiver() = default;

  /**
   * @brief  Call input to fill frame until it returns available, then call
   * output
   * @param  New byte data received from sensor
   * @return Frame input status on filling a frame
   */
  FrameInputStatus input(uint8_t new_byte);

  /**
   * @brief  output method updates the frame on available or throws
   * error/waiting
   * @param  outputBuffer to receive frame from frame receiver
   * @param  frameIndex index of frame in a packet
   * @param  status byte of frame
   * @return Frame output status after validating the frame
   */
  FrameOutputStatus output(Frame &frame);

 private:
  static const uint8_t mask_start_of_packet = 0x81;

  FrameInputStatus update_frame_buffer(uint8_t new_byte);
  FrameInputStatus input_status_ = FrameInputStatus::ok;
  FrameBuffer frame_buf_;

  /* Variable that validates the start of frame
     true : Start of frame packet is available reading the consicutive frame are
     possible false: Start of frame packet is not available , false is set in
     the beginning of reading bytes and on there is loss of bytes in a frame or
     noise occurred in recived frame due to which the validation of start of
     frame is called */
  bool start_of_packet_status_ = false;
};

extern bool validate_frame_header(const Frame &new_frame, const uint8_t &mask);
extern bool validate_frame_checksum(const Frame &new_frame);

extern FrameInputStatus validate_frame(const Frame &new_frame);

}  // namespace Pufferfish::Driver::Serial::Nonin
