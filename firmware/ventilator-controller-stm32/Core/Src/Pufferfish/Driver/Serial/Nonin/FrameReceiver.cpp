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
 * Called based on start_of_frame_status_ private variable
 */
bool validate_start_of_frame(const Frame &new_frame) {
  static const uint8_t mask_start_of_packet = 0x81;
  if (new_frame[0] == 0x01 && (new_frame[1] & mask_start_of_packet) == mask_start_of_packet) {
    if (((new_frame[0] + new_frame[1] + new_frame[2] + new_frame[3]) % (UINT8_MAX + 1)) ==
        new_frame[4]) {
      return true;
    }
  }
  return false;
}

/**
 * validateFrame function is called to validated the every frame for Status byte
 * and Checksum.
 */
FrameInputStatus validate_frame(const Frame &new_frame) {
  static const uint8_t mask_start_of_frame = 0x80;
  if (new_frame[0] == 0x01 && (new_frame[1] & mask_start_of_frame) == mask_start_of_frame) {
    if (((new_frame[0] + new_frame[1] + new_frame[2] + new_frame[3]) % (UINT8_MAX + 1)) ==
        new_frame[4]) {
      return FrameInputStatus::available;
    }
  }

  return FrameInputStatus::checksum_failed;
}

FrameInputStatus FrameReceiver::update_frame_buffer(uint8_t new_byte) {
  Frame frame_buffer;

  if (frame_buf_.input(new_byte) == BufferStatus::partial) {
    return FrameInputStatus::waiting;
  }

  if (frame_buf_.output(frame_buffer) != BufferStatus::ok) {
    return FrameInputStatus::waiting;
  }

  if (!start_of_frame_status_) {
    if (validate_start_of_frame(frame_buffer)) {
      start_of_frame_status_ = true;
      return FrameInputStatus::available;
    }
    frame_buf_.shift_left();
    return FrameInputStatus::waiting;
  }

  input_status_ = validate_frame(frame_buffer);
  if (input_status_ == FrameInputStatus::checksum_failed) {
    start_of_frame_status_ = false;
  }

  return input_status_;
}

FrameInputStatus FrameReceiver::input(const uint8_t new_byte) {
  input_status_ = this->update_frame_buffer(new_byte);

  return input_status_;
}

FrameOutputStatus FrameReceiver::output(Frame &frame) {
  if (input_status_ != FrameInputStatus::available) {
    return FrameOutputStatus::waiting;
  }

  if (frame_buf_.output(frame) != BufferStatus::ok) {
    return FrameOutputStatus::waiting;
  }

  frame_buf_.reset();

  return FrameOutputStatus::available;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
