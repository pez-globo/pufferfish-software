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

/* Enum class for amplitude representation of signal quality  */
enum class SignalQuality {
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
struct Status {
  bool sensor_disconnect;
  bool artifact;
  bool out_of_track;
  bool sensor_alarm;
  SignalQuality signal_perfusion;
  bool bit7;
};

/* Packet of 25 frames */
static const size_t packet_size = 25;
using Packet = std::array<Frame, packet_size>;
/* PLETH for 25 frames */
using PlethPulseAmplitudes = std::array<uint8_t, packet_size>;
using Flags = std::array<bool, packet_size>;

/* Structure defines the sensor data in packet for measurements */
struct PacketMeasurements {
  /* Heart Rate and SpO2 measurements */
  uint16_t hr;
  uint8_t spo2;
  uint8_t firmware_revision;
  uint8_t spo2_d;
  uint8_t spo2_fast;
  uint8_t spo2_b_b;
  uint16_t e_hr;
  uint8_t e_spo2;
  uint8_t e_spo2_d;
  uint16_t hr_d;
  uint16_t e_hr_d;
  /* PLETH measurements */
  PlethPulseAmplitudes packet_pleth;
  /* measurements Status */
  Flags sensor_disconnect;
  Flags artifact;
  Flags out_of_track;
  Flags sensor_alarm;
  std::array<SignalQuality, packet_size> signal_perfusion;
  Flags bit7;
};

// Status Mask
static const uint8_t start_of_frame = 0b1000'0000;
static const uint8_t snsd = 0b0100'0000;
static const uint8_t artf = 0b0010'0000;
static const uint8_t oot = 0b0001'0000;
static const uint8_t snsa = 0b1000;
static const uint8_t yprf = 0b0110;
static const uint8_t rprf = 0b0100;
static const uint8_t gprf = 0b0010;

// Byte Index
static const size_t hr_index = 0;
static const size_t spo2_index = 2;
static const size_t firmware_revision_index = 3;
static const size_t spo2_d_index = 8;
static const size_t spo2_fast_index = 9;
static const size_t spo2_b_b_index = 10;
static const size_t e_hr_index = 13;
static const size_t e_spo2_index = 15;
static const size_t e_spo2_d_index = 16;
static const size_t hr_d_index = 19;
static const size_t e_hr_d_index = 21;

}  // namespace Pufferfish::Driver::Serial::Nonin
