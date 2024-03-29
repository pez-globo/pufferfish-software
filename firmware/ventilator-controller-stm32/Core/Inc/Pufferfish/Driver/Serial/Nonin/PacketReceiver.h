/// PacketReceiver.h
/// This file has call and methos for packet receiver takes the input frames and
/// output the packet measurements on availability

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

#include <array>

#include "FrameBuffer.h"
#include "FrameReceiver.h"
#include "Types.h"

namespace Pufferfish::Driver::Serial::Nonin {

enum class PacketInputStatus {
  output_ready = 0,  /// Input is available to read output
  waiting,           /// Input is wait to read more bytes
  frame_loss         /// missed one or more frames in previous received packet
};
using Packet = std::array<Frame, packet_size>;
using PacketOutputStatus = Pufferfish::Driver::Serial::Nonin::FrameOutputStatus;

/**
 * @brief  Inline function to get the SpO2 data
 * @param  ByteData - Byte of SpO2 Data received from packet
 * @return Masked value of SpO2 from input Spo2Data
 */
inline uint16_t get_spo2_data(uint8_t byte_data) {
  return byte_data & Measurements::spo2_mask;
}

/**
 * @brief  Inline function to get the Heart rate from HR MSB and HR LSB
 * @param  msbByte - MSB to extract 2 bits
 * @param  lsbByte - MSB to extract 5 bits
 * @return Calculated 9 bit data from MSB and LSB
 */
inline uint16_t get_hr_data(uint8_t msb_byte, uint8_t lsb_byte) {
  /* Pack 2 bits of MSB and 7 bits of LSB for 9 bits of heart rate data  */
  uint16_t msb = static_cast<uint16_t>(msb_byte & Measurements::hr_mask_msb)
                 << Measurements::hr_shift_msb;
  uint16_t lsb = lsb_byte & Measurements::hr_mask_lsb;

  return static_cast<uint16_t>(msb | lsb) & Measurements::hr_mask;
}

/*
 * PacketReceiver class for
 */
class PacketReceiver {
 public:
  PacketReceiver() = default;

  /**
   * @brief  Input is called until packet is filled
   * @param  frame input frame of 5 bytes
   * @param  frameIndex frame index of a packet
   * @return Packet input status on available of packet
   */
  PacketInputStatus input(const Frame &frame);

  /**
   * @brief  Output is called after the input status is available to read
   * measurements
   * @param  sensorMeasurements is updated on available of measurements
   * @return Packet Output status on available of measurements
   */
  PacketOutputStatus output(Sample &sensor_measurements);

 private:
  Packet packet_data_{};
  size_t received_length_ = packet_size;
  PacketInputStatus input_status_ = PacketInputStatus::waiting;
};

extern void read_status_byte(
    Sample &sensor_measurements, const size_t &frame_index, const uint8_t &byte_value);

bool check_packet_sync(const Frame &frame);

}  // namespace Pufferfish::Driver::Serial::Nonin
