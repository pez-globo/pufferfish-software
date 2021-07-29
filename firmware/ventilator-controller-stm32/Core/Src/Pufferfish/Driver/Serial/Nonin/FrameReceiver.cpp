/// FrameReceiver.cpp
/// This file has methods to take input from FrameBuffer and output the
/// frame on availability.

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

namespace Pufferfish::Driver::Serial::Nonin {

/**
 * validateStartOfFrame function is called on beginning to get the first frame
 * and on there is a loss of bytes or noise in the bytes of frame received.
 * Called based on start_of_packet_status_ private variable
 */
bool validate_frame_header(const Frame &new_frame, const uint8_t &mask) {
  return (new_frame[0] == 0x01 && (new_frame[1] & mask) == mask);
}

bool validate_frame_checksum(const Frame &new_frame) {
  return (
      (new_frame[0] + new_frame[1] + new_frame[2] + new_frame[3]) % (UINT8_MAX + 1) ==
      new_frame[4]);
}

/**
 * validateFrame function is called to validated the every frame for Status byte
 * and Checksum.
 */
FrameInputStatus validate_frame(const Frame &new_frame) {
  static const uint8_t mask_start_of_frame = 0x80;
  if (!validate_frame_header(new_frame, mask_start_of_frame)) {
    return FrameInputStatus::invalid_header;
  }

  if (!validate_frame_checksum(new_frame)) {
    return FrameInputStatus::invalid_checksum;
  }
  return FrameInputStatus::output_ready;
}

FrameInputStatus FrameReceiver::update_frame_buffer(uint8_t new_byte) {
  Frame frame_buffer;

  if (frame_buf_.input(new_byte) == FrameBufferStatus::waiting) {
    return FrameInputStatus::ok;
  }

  if (frame_buf_.output(frame_buffer) != FrameBufferStatus::ok) {
    return FrameInputStatus::ok;
  }

  if (!start_of_packet_status_) {
    if (validate_frame_header(frame_buffer, mask_start_of_packet) &&
        validate_frame_checksum(frame_buffer)) {
      start_of_packet_status_ = true;
      return FrameInputStatus::output_ready;
    }
    frame_buf_.shift_left();
    return FrameInputStatus::ok;
  }

  input_status_ = validate_frame(frame_buffer);
  if (input_status_ == FrameInputStatus::invalid_header ||
      input_status_ == FrameInputStatus::invalid_checksum) {
    start_of_packet_status_ = false;
  }

  return input_status_;
}

FrameInputStatus FrameReceiver::input(const uint8_t new_byte) {
  input_status_ = update_frame_buffer(new_byte);

  return input_status_;
}

FrameOutputStatus FrameReceiver::output(Frame &frame) {
  if (input_status_ == FrameInputStatus::ok) {
    return FrameOutputStatus::waiting;
  }

  if (frame_buf_.output(frame) != FrameBufferStatus::ok) {
    return FrameOutputStatus::waiting;
  }

  frame_buf_.reset();

  return FrameOutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
