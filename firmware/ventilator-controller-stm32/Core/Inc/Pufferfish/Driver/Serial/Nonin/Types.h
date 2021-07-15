/// Types.h

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

namespace Pufferfish::Driver::Serial::Nonin {

/* PacketReceiver Input status */
enum class PacketStatus {
  available = 0,  /// Packet/measurements is available
  waiting,        /// Packet/measurements is waiting to receive more bytes of data
  not_available,  /// Packet/measurements are not available
  framing_error,  /// Error in checksum or status byte or in byte 1 of a frame
  missed_data     /// Missed a packet due loss of frames
};

/* FrameReceiver input status return values */
enum class FrameInputStatus {
  waiting = 0,    /// Input is ready to receive new bytes of sensor data
  framing_error,  /// Error in checksum or status byte or in byte 1 of a frame
  available       /// frame is available
};

/* FrameReceiver output status return values */
enum class FrameOutputStatus {
  available = 0,  /// frame output is available for packet fill
  waiting         /// frame output is waiting for complete frame
};

/* PacketReceiver Input status */
enum class PacketInputStatus {
  available = 0,  /// Input is available to read output
  waiting,        /// Input is wait to read more bytes
  missed_data     /// missed one or more frames in previous received packet
};

/* PacketReceiver Output status */
enum class PacketOutputStatus {
  available = 0,  /// Output measurements are available
  waiting         /// Output is waiting to receive more byte for measurements
};

/* Enum class for amplitude representation of signal quality  */
enum class SignalAmplitude {
  no_perfusion = 0,  /// No loss in signal quality
  red_perfusion,     /// Red Perfusion – Amplitude representation of low signal
                     /// quality
  yellow_perfusion,  /// YPRF: Yellow Perfusion – Amplitude representation of
                     /// medium signal quality
  green_perfusion    /// Green Perfusion – Amplitude representation of high signal
                     /// quality
};

/* Frame */
const uint8_t frame_max_size = 5;
using Frame = std::array<uint8_t, frame_max_size>;

/* Structure defines the sensor data in packet for measurements */
struct StatusByteStruct {
  bool bit7;
  bool sensor_disconnect;
  bool artifact;
  bool out_of_track;
  bool sensor_alarm;
  SignalAmplitude signal_perfusion;
};

/* Packet of 25 frames */
static const size_t packet_size = 25;
using Packet = std::array<Frame, packet_size>;
/* Status Byte error of 25 frames */
using StatusByteError = std::array<StatusByteStruct, packet_size>;
/* PLETH for 25 frames */
using Pleth = std::array<uint8_t, packet_size>;

/* Structure defines the sensor data in packet for measurements */
struct PacketMeasurements {
  /* Heart Rate and SpO2 measurements */
  uint16_t heart_rate;
  uint8_t spo2;
  uint8_t spo2_d;
  uint8_t spo2_d_fast;
  uint8_t spo2_d_beat;
  uint16_t e_heart_rate;
  uint8_t e_spo2;
  uint8_t e_spo2_d;
  uint16_t heart_rate_d;
  uint16_t e_heart_rate_d;
  uint8_t nonin_oem_revision;
  /* PLETH measurements */
  Pleth packet_pleth;
  /* StatusByteErrors measurements */
  std::array<bool, packet_size> bit7;
  std::array<bool, packet_size> sensor_disconnect;
  std::array<bool, packet_size> artifact;
  std::array<bool, packet_size> out_of_track;
  std::array<bool, packet_size> sensor_alarm;
  std::array<SignalAmplitude, packet_size> signal_perfusion;
};

static const uint8_t mask_6bit = 0x7F;

// Status Mask
static const uint8_t mask_start_of_frame = 0x80;
static const uint8_t mask_snsd = 0x40;
static const uint8_t mask_artf = 0x20;
static const uint8_t mask_oot = 0x10;
static const uint8_t mask_snsa = 0x08;
static const uint8_t mask_yprf = 0x06;
static const uint8_t mask_rprf = 0x04;
static const uint8_t mask_gprf = 0x02;

// Byte Index
static const size_t heart_rate_index = 0;
static const size_t spo2_index = 2;
static const size_t nonin_oem_revision_index = 3;
static const size_t spo2_d_index = 8;
static const size_t spo2_d_fast_index = 9;
static const size_t spo2_d_beat_index = 10;
static const size_t e_heart_rate_index = 13;
static const size_t e_spo2_index = 15;
static const size_t e_spo2_d_index = 16;
static const size_t heart_rate_d_index = 19;
static const size_t e_heart_rate_d_index = 21;

}  // namespace Pufferfish::Driver::Serial::Nonin
