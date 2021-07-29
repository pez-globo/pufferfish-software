/// Device.cpp
/// This file has methods to output the sensor measurements of NoninOEM
/// 3 sensor based on packet received.

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

#include "Pufferfish/Driver/Serial/Nonin/Device.h"

namespace Pufferfish::Driver::Serial::Nonin {

PacketStatus Device::output(Sample &sensor_measurements) {
  uint8_t read_byte = 0;
  Frame frame_buffer;

  if (nonin_uart_.read(read_byte) == BufferStatus::empty) {
    return PacketStatus::waiting;
  }

  switch (frame_receiver_.input(read_byte)) {
    case FrameInputStatus::invalid_header:
      return PacketStatus::invalid_header;
    case FrameInputStatus::invalid_checksum:
      return PacketStatus::invalid_checksum;
    case FrameInputStatus::ok:
      return PacketStatus::waiting;
    case FrameInputStatus::output_ready:
      break;
  }

  if (frame_receiver_.output(frame_buffer) == FrameOutputStatus::waiting) {
    return PacketStatus::waiting;
  }

  switch (packet_receiver_.input(frame_buffer)) {
    case PacketInputStatus::waiting:
      return PacketStatus::waiting;
    case PacketInputStatus::frame_loss:
      return PacketStatus::frame_loss;
    case PacketInputStatus::output_ready:
      break;
  }

  switch (packet_receiver_.output(sensor_measurements)) {
    case PacketOutputStatus::waiting:
      return PacketStatus::waiting;
    case PacketOutputStatus::ok:
      break;
  }

  return PacketStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
