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
  sensor_measurements.sensor_disconnect[frame_index] = (byte_value & snsd) != 0x00;
  sensor_measurements.artifact[frame_index] = (byte_value & artf) != 0x00;
  sensor_measurements.out_of_track[frame_index] = (byte_value & oot) != 0x00;
  sensor_measurements.sensor_alarm[frame_index] = (byte_value & snsa) != 0x00;
  sensor_measurements.bit7[frame_index] = (byte_value & start_of_frame) == 0x00;

  if ((byte_value & yprf) == yprf) {
    sensor_measurements.signal_perfusion[frame_index] = SignalQuality::yellow_perfusion;
  } else if ((byte_value & rprf) == rprf) {
    sensor_measurements.signal_perfusion[frame_index] = SignalQuality::red_perfusion;
  } else if ((byte_value & gprf) == gprf) {
    sensor_measurements.signal_perfusion[frame_index] = SignalQuality::green_perfusion;
  } else {
    sensor_measurements.signal_perfusion[frame_index] = SignalQuality::no_perfusion;
  }
}

void read_packet_measurements(Sample &sensor_measurements, const Packet &packet_data) {
  const uint8_t byte1 = 1;
  const uint8_t byte2 = 2;
  const uint8_t byte3 = 3;

  // SpO2 data
  sensor_measurements.spo2 = get_spo2_data(packet_data[spo2_index][byte3]);
  sensor_measurements.spo2_d = get_spo2_data(packet_data[spo2_d_index][byte3]);
  sensor_measurements.spo2_fast = get_spo2_data(packet_data[spo2_fast_index][byte3]);
  sensor_measurements.spo2_b_b = get_spo2_data(packet_data[spo2_b_b_index][byte3]);
  sensor_measurements.e_spo2 = get_spo2_data(packet_data[e_spo2_index][byte3]);
  sensor_measurements.e_spo2_d = get_spo2_data(packet_data[e_spo2_d_index][byte3]);
  // Firmware revision
  sensor_measurements.firmware_revision = packet_data[firmware_revision_index][byte3];
  // HR data
  sensor_measurements.hr =
      get_hr_data(packet_data[hr_index][byte3], packet_data[hr_index + 1][byte3]);
  sensor_measurements.e_hr =
      get_hr_data(packet_data[e_hr_index][byte3], packet_data[e_hr_index + 1][byte3]);
  sensor_measurements.hr_d =
      get_hr_data(packet_data[hr_d_index][byte3], packet_data[hr_d_index + 1][byte3]);
  sensor_measurements.e_hr_d =
      get_hr_data(packet_data[e_hr_d_index][byte3], packet_data[e_hr_d_index + 1][byte3]);

  for (size_t frame_index = 0; frame_index < packet_size; ++frame_index) {
    sensor_measurements.packet_pleth[frame_index] = packet_data[frame_index][byte2];
    read_status_byte(sensor_measurements, frame_index, packet_data[frame_index][byte1]);
  }
}

PacketInputStatus PacketReceiver::input(const Frame &frame) {
  if ((frame[1] & 0x01U) == 0x01) {
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
  received_length_ += 1;

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
