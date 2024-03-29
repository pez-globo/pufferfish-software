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
 * Called based on at_start_of_frame_ private variable
 */
bool validate_frame_header(const Frame &new_frame, uint8_t mask) {
  return (new_frame[0] == 0x01 && (new_frame[1] & mask) == mask);
}

bool validate_frame_checksum(const Frame &new_frame) {
  return (
      (new_frame[0] + new_frame[1] + new_frame[2] + new_frame[3]) % (UINT8_MAX + 1) ==
      new_frame[4]);
}

FrameInputStatus FrameReceiver::update_frame_buffer(uint8_t new_byte) {
  Frame frame_buffer;

  switch (frame_buf_.input(new_byte)) {
    case BufferInputStatus::ok:
      return FrameInputStatus::ok;
    case BufferInputStatus::output_ready:
    // We want to check if the frame is valid, thus, it's not discarded here.
    case BufferInputStatus::full:
      break;
  }

  if (frame_buf_.output(frame_buffer) == BufferOutputStatus::waiting) {
    return FrameInputStatus::ok;
  }

  if (!at_start_of_frame_) {
    static const uint8_t mask_start_of_packet = 0b10000001;
    if (validate_frame_header(frame_buffer, mask_start_of_packet) &&
        validate_frame_checksum(frame_buffer)) {
      at_start_of_frame_ = true;
      return FrameInputStatus::output_ready;
    }
    frame_buf_.shift_left();
    return FrameInputStatus::ok;
  }

  static const uint8_t mask_start_of_frame = 0b10000000;
  if (!validate_frame_header(frame_buffer, mask_start_of_frame)) {
    at_start_of_frame_ = false;
    return FrameInputStatus::invalid_header;
  }

  if (!validate_frame_checksum(frame_buffer)) {
    at_start_of_frame_ = false;
    return FrameInputStatus::invalid_checksum;
  }

  return FrameInputStatus::output_ready;
}

FrameInputStatus FrameReceiver::input(const uint8_t new_byte) {
  input_status_ = update_frame_buffer(new_byte);

  return input_status_;
}

FrameOutputStatus FrameReceiver::output(Frame &frame) {
  if (input_status_ == FrameInputStatus::ok) {
    return FrameOutputStatus::waiting;
  }

  if (frame_buf_.output(frame) != BufferOutputStatus::ok) {
    return FrameOutputStatus::waiting;
  }

  frame_buf_.reset();

  return FrameOutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
