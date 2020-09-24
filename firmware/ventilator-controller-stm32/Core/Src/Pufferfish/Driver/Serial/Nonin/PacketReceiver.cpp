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

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/*
 * @brief  Validates the status byte of frame
 * @param  frame index of packet received from sensor
 * @param  statusByte structure is updated based on input byteValue
 * @return None
 */
void read_status_byte(PacketMeasurements &sensor_measurements,
                      const size_t &frame_index, const uint8_t &byte_value) {
  /* BIT7: Always high */
  sensor_measurements.bit7[frame_index] = (byte_value & 0x80) == 0x00;
  /* BIT6: SNSD: Sensor Disconnect – Sensor is not connected to oximeter or
   * sensor is inoperable */
  sensor_measurements.sensor_disconnect[frame_index] =
      (byte_value & 0x40) != 0x00;
  /* BIT5: ARTF: Artifact – A detected pulse beat didn’t match the current pulse
   * interval */
  sensor_measurements.artifact[frame_index] = (byte_value & 0x20) != 0x00;
  /* BIT4: OOT: Out Of Track – An absence of consecutive good pulse signals */
  sensor_measurements.out_of_track[frame_index] = (byte_value & 0x10) != 0x00;
  /* BIT3: SNSA: Sensor Alarm – Sensor is providing unusable data for analysis
   */
  sensor_measurements.sensor_alarm[frame_index] = (byte_value & 0x08) != 0x00;

  /**
   * BIT2 and BIT1: YPRF: Yellow Perfusion – Amplitude representation of medium
   * signal quality. BIT2: RPRF: Red Perfusion – Amplitude representation of low
   * signal quality. BIT1: GPRF: Green Perfusion – Amplitude representation of
   * high signal quality.
   */
  if ((byte_value & 0x06) != 0) {
    sensor_measurements.signal_perfusion[frame_index] =
        SignalAmplitude::yellow_perfusion;
  } else if ((byte_value & 0x04) != 0) {
    sensor_measurements.signal_perfusion[frame_index] =
        SignalAmplitude::red_perfusion;
  } else if ((byte_value & 0x02) != 0) {
    sensor_measurements.signal_perfusion[frame_index] =
        SignalAmplitude::green_perfusion;
  } else {
    sensor_measurements.signal_perfusion[frame_index] =
        SignalAmplitude::no_perfusion;
  }
}

void read_packet_measurements(PacketMeasurements &sensor_measurements,
                              const Packet &packet_data) {
  /* 4th byte of a frame contains heart beat and spo2 data */
  const uint8_t byte1 = 1;
  const uint8_t byte2 = 2;
  const uint8_t byte3 = 3;
  size_t frame_index = 0;

  /**
   * Heart Rate: 4-beat average values in standard mode.
   * E-HR MSB : sensorData 1st frame of packet contains HR-Bit8 and HR-Bit7 in
   * bit1 and bit0 E-HR LSB : sensorData 2nd frame of packet contains HR-Bit6 t0
   * HR-Bit0 in bit6 to bit0
   */
  sensor_measurements.heart_rate =
      get9_bit_data(packet_data[0][byte3], packet_data[1][byte3]);

  /**
   * spo2: 4-beat average values in standard mode.
   * sensorData 3rd frame of packet contains spo2-Bit6 t0 spo2-Bit0 in bit6 to
   * bit0
   */
  sensor_measurements.spo2 = get6_bit_data(packet_data[2][byte3]);

  /**
   * Nonin Oximeter Firmware Revision Level
   * sensorData 4th frame of packet contains Firmware Revision Level
   */
  sensor_measurements.nonin_oem_revision = packet_data[3][byte3];

  /**
   * spo2-D: 4-beat average displayed values in display mode
   * sensorData 9th frame of packet contains spo2-D-Bit6 t0 spo2-D-Bit0 in bit6
   * to bit0
   */
  sensor_measurements.spo2_d = get6_bit_data(packet_data[8][byte3]);

  /**
   * spo2 Fast: Non-slew limited saturation with 4-beat averaging in standard
   * mode. sensorData 10th frame of packet contains spo2-D-Bit6 t0 spo2-D-Bit0
   * in bit6 to bit0
   */
  sensor_measurements.spo2_d_fast = get6_bit_data(packet_data[9][byte3]);

  /**
   * spo2 B-B: Un-averaged, non-slew limited, beat to beat value in standard
   * mode sensorData 11th frame of packet contains spo2-D-Bit6 t0 spo2-D-Bit0 in
   * bit6 to bit0
   */
  sensor_measurements.spo2_d_beat = get6_bit_data(packet_data[10][byte3]);

  /**
   * E-Heart Rate: 8-beat average values in standard mode.
   * E-HR MSB : sensorData 14th frame of packet contains HR-Bit8 and HR-Bit7 in
   * bit1 and bit0 E-HR LSB : sensorData 15th frame of packet contains HR-Bit6
   * t0 HR-Bit0 in bit6 to bit0
   */
  sensor_measurements.e_heart_rate =
      get9_bit_data(packet_data[13][byte3], packet_data[14][byte3]);

  /**
   * E-spo2: 8-beat average values in standard mode.
   * sensorData 16th frame of packet contains spo2-Bit6 t0 spo2-Bit0 in bit6 to
   * bit0
   */
  sensor_measurements.e_spo2 = get6_bit_data(packet_data[15][byte3]);

  /**
   * E-spo2-D: 8-beat average displayed values in display mode
   * sensorData 17th frame of packet contains spo2-D-Bit6 to E-spo2-D-Bit0 in
   * bit6 to bit0
   */
  sensor_measurements.e_spo2_d = get6_bit_data(packet_data[16][byte3]);

  /**
   * HR-D: 8-beat average values in standard mode.
   * HR-D MSB : sensorData 20th frame of packet contains HR-Bit8 and HR-Bit7 in
   * bit1 and bit0 HR-D LSB : sensorData 21st frame of packet contains HR-Bit6
   * t0 HR-Bit0 in bit6 to bit0
   */
  sensor_measurements.heart_rate_d =
      get9_bit_data(packet_data[19][byte3], packet_data[20][byte3]);

  /**
   * E-HR-D: 8-beat average displayed values in display mode
   * E-HR-D MSB : sensorData 22nd frame of packet contains HR-Bit8 and HR-Bit7
   * in bit1 and bit0 E-HR-D LSB : sensorData 23rd frame of packet contains
   * HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensor_measurements.e_heart_rate_d =
      get9_bit_data(packet_data[21][byte3], packet_data[22][byte3]);

  /* Update the PLETH and Status byte errors into the packet measurements */
  for (frame_index = 0; frame_index < PacketReceiver::packet_size;
       frame_index++) {
    sensor_measurements.packet_pleth[frame_index] =
        packet_data[frame_index][byte2];
    read_status_byte(sensor_measurements, frame_index,
                     packet_data[frame_index][byte1]);
  }
}

PacketReceiver::PacketInputStatus PacketReceiver::input(const Frame &frame) {
  /* Check the frame received is first frame in the packet SYNC bit is 1 */
  if ((frame[1] & 0x01) == 0x01) {
    if (received_length_ != 25) {
      /* Update the frame index to 0 */
      received_length_ = 0;
      /* Update the frame received to packet */
      packet_data_[received_length_] = frame;
      /* Increment the frame index */
      received_length_ = received_length_ + 1;

      /* Update input status to missedData to report few frames of data are
       * missed in previous packet */
      input_status_ = PacketInputStatus::missed_data;
      return input_status_;
    }
    /* Update the frame index to 0 */
    received_length_ = 0;
  }

  /* Update the frame received to packet */
  packet_data_[received_length_] = frame;
  /* Increment the frame index */
  received_length_ = received_length_ + 1;

  /* Check for the packet data is complete */
  if (received_length_ != packet_size) {
    /* Update input status is waiting to receive more frames */
    input_status_ = PacketInputStatus::waiting;
    return input_status_;
  }

  /* Update input status is available for measurements */
  input_status_ = PacketInputStatus::available;
  return input_status_;
}

PacketReceiver::PacketOutputStatus PacketReceiver::output(
    PacketMeasurements &sensor_measurements) {
  /* Check for the frame availability in the buffer */
  if (input_status_ != PacketInputStatus::available) {
    /* Return PacketOutputStatus as waiting to receive packet data */
    return PacketOutputStatus::waiting;
  }
  /* Read PacketBuffer and Update the measurements */
  read_packet_measurements(sensor_measurements, packet_data_);

  /* Return Packet Output status as available */
  return PacketOutputStatus::available;
}

}  // namespace Nonin
}  // namespace Serial
}  // namespace Driver
}  // namespace Pufferfish
