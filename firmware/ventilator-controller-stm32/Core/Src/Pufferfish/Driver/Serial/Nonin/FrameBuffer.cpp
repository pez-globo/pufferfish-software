/// FrameBuffer.cpp
/// This file has methods to input the byte received from sensor and
/// output the frame on availability.

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

#include "Pufferfish/Driver/Serial/Nonin/FrameBuffer.h"

namespace Pufferfish::Driver::Serial::Nonin {

BufferStatus FrameBuffer::input(const uint8_t byte) {
  if (received_length_ == frame_max_size) {
    return BufferStatus::full;
  }

  frame_[received_length_] = byte;
  ++received_length_;

  if (received_length_ == frame_max_size) {
    return BufferStatus::ok;
  }

  return BufferStatus::partial;
}

BufferStatus FrameBuffer::output(Frame &frame) {
  if (received_length_ != frame_max_size) {
    return BufferStatus::partial;
  }
  frame = frame_;

  return BufferStatus::ok;
}

void FrameBuffer::reset() {
  received_length_ = 0;
}

void FrameBuffer::shift_left() {
  if (received_length_ == 0) {
    return;
  }

  for (size_t index = 0; index < (received_length_ - 1); ++index) {
    frame_[index] = frame_[index + 1];
  }
  --received_length_;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
