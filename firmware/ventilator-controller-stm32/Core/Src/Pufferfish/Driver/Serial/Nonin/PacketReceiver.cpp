/// PacketReceiver.cpp
/// Packet receiver takes the input frames and output the packet
/// measurements on availability

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

#include "Pufferfish/Driver/Serial/Nonin/PacketReceiver.h"

#include <cstddef>
#include <cstdint>

namespace Pufferfish::Driver::Serial::Nonin {

/*
 * @brief  Validates the status byte of frame
 * @param  frame index of packet received from sensor
 * @param  statusByte structure is updated based on input byteValue
 * @return None
 */
void read_status_byte(
    Sample &sensor_measurements, const size_t &frame_index, const uint8_t &byte_value) {
  sensor_measurements.sensor_disconnect[frame_index] = (byte_value & StatusMasks::snsd) != 0x00;
  sensor_measurements.artifact[frame_index] = (byte_value & StatusMasks::artf) != 0x00;
  sensor_measurements.out_of_track[frame_index] = (byte_value & StatusMasks::oot) != 0x00;
  sensor_measurements.sensor_alarm[frame_index] = (byte_value & StatusMasks::snsa) != 0x00;
  sensor_measurements.bit7[frame_index] = (byte_value & StatusMasks::start_of_frame) == 0x00;

  SignalQuality &signal_perfusion = sensor_measurements.signal_perfusion[frame_index];
  if ((byte_value & StatusMasks::yprf) == StatusMasks::yprf) {
    signal_perfusion = SignalQuality::yellow_perfusion;
  } else if ((byte_value & StatusMasks::rprf) == StatusMasks::rprf) {
    signal_perfusion = SignalQuality::red_perfusion;
  } else if ((byte_value & StatusMasks::gprf) == StatusMasks::gprf) {
    signal_perfusion = SignalQuality::green_perfusion;
  } else {
    signal_perfusion = SignalQuality::no_perfusion;
  }
}

void read_packet_measurements(Sample &sensor_measurements, const Packet &packet_data) {
  // SpO2 data
  sensor_measurements.spo2 = get_spo2_data(packet_data[ByteIndex::spo2][FrameBytes::data]);
  sensor_measurements.spo2_d = get_spo2_data(packet_data[ByteIndex::spo2_d][FrameBytes::data]);
  sensor_measurements.spo2_fast =
      get_spo2_data(packet_data[ByteIndex::spo2_fast][FrameBytes::data]);
  sensor_measurements.spo2_b_b = get_spo2_data(packet_data[ByteIndex::spo2_b_b][FrameBytes::data]);
  sensor_measurements.e_spo2 = get_spo2_data(packet_data[ByteIndex::e_spo2][FrameBytes::data]);
  sensor_measurements.e_spo2_d = get_spo2_data(packet_data[ByteIndex::e_spo2_d][FrameBytes::data]);
  // Firmware revision
  sensor_measurements.firmware_revision =
      packet_data[ByteIndex::firmware_revision][FrameBytes::data];
  // HR data
  sensor_measurements.hr = get_hr_data(
      packet_data[ByteIndex::hr][FrameBytes::data],
      packet_data[ByteIndex::hr + 1][FrameBytes::data]);
  sensor_measurements.e_hr = get_hr_data(
      packet_data[ByteIndex::e_hr][FrameBytes::data],
      packet_data[ByteIndex::e_hr + 1][FrameBytes::data]);
  sensor_measurements.hr_d = get_hr_data(
      packet_data[ByteIndex::hr_d][FrameBytes::data],
      packet_data[ByteIndex::hr_d + 1][FrameBytes::data]);
  sensor_measurements.e_hr_d = get_hr_data(
      packet_data[ByteIndex::e_hr_d][FrameBytes::data],
      packet_data[ByteIndex::e_hr_d + 1][FrameBytes::data]);

  for (size_t frame_index = 0; frame_index < packet_size; ++frame_index) {
    sensor_measurements.packet_pleth[frame_index] = packet_data[frame_index][FrameBytes::pleth];
    read_status_byte(
        sensor_measurements, frame_index, packet_data[frame_index][FrameBytes::status]);
  }
}

// Check if the sync bit is present at the start of each packet
bool check_packet_sync(const Frame &frame) {
  return (frame[FrameBytes::status] & StatusMasks::sync) == StatusMasks::sync;
}

PacketInputStatus PacketReceiver::input(const Frame &frame) {
  if (check_packet_sync(frame)) {
    if (received_length_ != packet_size) {
      received_length_ = 0;
      packet_data_[received_length_] = frame;
      received_length_ += 1;

      input_status_ = PacketInputStatus::frame_loss;
      return input_status_;
    }

    received_length_ = 0;
  }

  if (received_length_ >= packet_size) {
    input_status_ = PacketInputStatus::frame_loss;
    return input_status_;
  }

  packet_data_[received_length_] = frame;
  ++received_length_;

  if (received_length_ != packet_size) {
    input_status_ = PacketInputStatus::waiting;
    return input_status_;
  }

  input_status_ = PacketInputStatus::output_ready;
  return input_status_;
}

PacketOutputStatus PacketReceiver::output(Sample &sensor_measurements) {
  if (input_status_ != PacketInputStatus::output_ready) {
    return PacketOutputStatus::waiting;
  }

  read_packet_measurements(sensor_measurements, packet_data_);

  return PacketOutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
