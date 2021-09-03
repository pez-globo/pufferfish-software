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

/**
 * Frame
 *
 * We are using  the Serial Data Format #2
 * https://upverter.com/datasheet/b7745aca802c392558dd63669771a93cafb3ee90.pdf pg6
 */
const uint8_t frame_max_size = 5;

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
/* PLETH for 25 frames */
using PlethPulseAmplitudes = std::array<uint8_t, packet_size>;
using Flags = std::array<bool, packet_size>;

/* Structure defines the sensor data in packet for measurements */
struct Sample {
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

namespace StatusMasks {
static const uint8_t start_of_frame = 0b1000'0000;
static const uint8_t snsd = 0b0100'0000;
static const uint8_t artf = 0b0010'0000;
static const uint8_t oot = 0b0001'0000;
static const uint8_t snsa = 0b1000;
static const uint8_t yprf = 0b0110;
static const uint8_t rprf = 0b0100;
static const uint8_t gprf = 0b0010;
static const uint8_t sync = 0b0001;
}  // namespace StatusMasks

namespace ByteIndex {
static const size_t hr = 0;
static const size_t spo2 = 2;
static const size_t firmware_revision = 3;
static const size_t spo2_d = 8;
static const size_t spo2_fast = 9;
static const size_t spo2_b_b = 10;
static const size_t e_hr = 13;
static const size_t e_spo2 = 15;
static const size_t e_spo2_d = 16;
static const size_t hr_d = 19;
static const size_t e_hr_d = 21;

}  // namespace ByteIndex

namespace FrameBytes {
static const uint8_t header = 0;
static const uint8_t status = 1;
static const uint8_t pleth = 2;
static const uint8_t data = 3;
static const uint8_t checksum = 4;

}  // namespace FrameBytes

namespace Measurements {
static const uint8_t spo2_mask = 0b0111'1111;
static const uint8_t hr_mask_lsb = 0b0111'1111;
static const uint8_t hr_mask_msb = 0b0000'0011;
static const uint8_t hr_shift_msb = 7;
static const uint16_t hr_mask = 0b1'1111'1111;
}  // namespace Measurements

namespace ErrorConstants {
static const uint8_t spo2_missing = 127;
static const uint16_t hr_missing = 511;
}  // namespace ErrorConstants

}  // namespace Pufferfish::Driver::Serial::Nonin
