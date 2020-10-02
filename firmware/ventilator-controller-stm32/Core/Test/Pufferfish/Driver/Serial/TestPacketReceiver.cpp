/// TestFrameReceiver.cpp
/// Unit tests to confirm behavior of PacketReceiver checks.

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

#include "catch2/catch.hpp"
#include "Pufferfish/Driver/Serial/Nonin/PacketReceiver.h"

namespace PF = Pufferfish;

SCENARIO("validate the read_status_byte function") {
  PF::Driver::Serial::Nonin::PacketMeasurements sensor_measurements;
  uint8_t byte_value = 0;
  const uint8_t frame_index = 1;

  GIVEN("A Status Byte ") {
    byte_value = 0x82;
    WHEN("Status Byte value is 0x82") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.signal_perfusion[1] == PF::Driver::Serial::Nonin::SignalAmplitude::green_perfusion);
      }
    }

    byte_value = 0x84;
    WHEN("Status Byte value is 0x84") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.signal_perfusion[1] == PF::Driver::Serial::Nonin::SignalAmplitude::red_perfusion);
      }
    }

    byte_value = 0x86;
    WHEN("Status Byte value is 0x86") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.signal_perfusion[1] == PF::Driver::Serial::Nonin::SignalAmplitude::yellow_perfusion);
      }
    }

    byte_value = 0x88;
    WHEN("Status Byte value is 0x88") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.sensor_alarm[1] == true);
      }
    }

    byte_value = 0x90;
    WHEN("Status Byte value is 0x90") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.out_of_track[1] == true);
      }
    }

    byte_value = 0xA0;
    WHEN("Status Byte value is 0xA0") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.artifact[1] == true);
      }
    }

    byte_value = 0xC0;
    WHEN("Status Byte value is 0xC0") {
      PF::Driver::Serial::Nonin::read_status_byte(sensor_measurements, frame_index, byte_value);
      THEN("Signal perfusion in PacketMeasurements shall be Green Perfusion") {
        REQUIRE(sensor_measurements.sensor_disconnect[1] == true);
      }
    }
  }
}


SCENARIO("Validate the valid first Packet", "[NoninOem3]") {
  GIVEN("A Valid First Packet") {
    const PF::Driver::Serial::Nonin::Packet test_packet = {
        0x01, 0x81, 0x01, 0x00, 0x83,  /// HR MSB
        0x01, 0x80, 0x01, 0x48, 0xCA,  /// HR LSB
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// SpO2
        0x01, 0x80, 0x01, 0x30, 0xB2,  /// REV
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// SpO2-D
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// SpO2 Fast
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// SpO2 B-B
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// E-HR MSB
        0x01, 0x80, 0x01, 0x48, 0xCA,  /// E-HR LSB
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// E-SpO2
        0x01, 0x80, 0x01, 0x61, 0xE3,  /// E-SpO2-D
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82,  /// HR-D MSB
        0x01, 0x80, 0x01, 0x48, 0xCA,  /// HR-D LSB
        0x01, 0x80, 0x01, 0x00, 0x82,  /// E-HR-D MSB
        0x01, 0x80, 0x01, 0x48, 0xCA,  /// E-HR-D LSB
        0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
        0x01, 0x80, 0x01, 0x00, 0x82   /// reserved
    };
    uint8_t index;

    PF::Driver::Serial::Nonin::PacketReceiver packet_receiver;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus packetInputStatus;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus packetOutputStatus;
    PF::Driver::Serial::Nonin::PacketMeasurements sensor_measurements;
    WHEN("Receiving first frame of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_packet[0]);
        REQUIRE(0x01 == ((test_packet[0][1]) & 0x01));
        THEN("Packet Input status shall return waiting status"){
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
    }

    WHEN("Receiving 0-24 frames of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_packet[0]);
      THEN("For the frames 2 to 24 Packet Input status shall return waiting status") {
        for(index = 1; index < (PF::Driver::Serial::Nonin::packet_size - 1); index++) {
          packetInputStatus = packet_receiver.input(test_packet[index]);
          REQUIRE((test_packet[index][1] & 0x01) == 0x00);
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
      }
    }

    WHEN("Receiving 25 frames of data from FrameReceiver") {
      for(index = 0; index < (PF::Driver::Serial::Nonin::packet_size - 1); index++) {
        packetInputStatus = packet_receiver.input(test_packet[index]);
      }
      THEN("For the 25th frame Packet Input status shall return available status") {
        packetInputStatus = packet_receiver.input(test_packet[24]);
        REQUIRE((test_packet[index][1] & 0x01) == 0x00);
        REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      }
    }
    AND_WHEN("packetInputStatus is available, invoke PacketReceiver::output to get the measurements") {
      for(index = 0; index < PF::Driver::Serial::Nonin::packet_size; index++) {
        packetInputStatus = packet_receiver.input(test_packet[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      packetOutputStatus = packet_receiver.output(sensor_measurements);
      REQUIRE(packetOutputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus::available);
      THEN("Validate the Heart Rate and SpO2") {
        REQUIRE(sensor_measurements.heart_rate == 72);
        REQUIRE(sensor_measurements.heart_rate_d == 72);
        REQUIRE(sensor_measurements.e_heart_rate == 72);
        REQUIRE(sensor_measurements.e_heart_rate_d == 72);
        REQUIRE(sensor_measurements.spo2 == 97);
        REQUIRE(sensor_measurements.spo2_d == 97);
        REQUIRE(sensor_measurements.e_spo2 == 97);
        REQUIRE(sensor_measurements.e_spo2_d == 97);
        REQUIRE(sensor_measurements.spo2_d_beat == 97);
        REQUIRE(sensor_measurements.spo2_d_fast == 97);
        REQUIRE(sensor_measurements.nonin_oem_revision == 48);
      }
    }
  }

  GIVEN("A valid Packet with status byte errors set ") {
    const PF::Driver::Serial::Nonin::Packet test_packet = {
      0x01, 0x81, 0x01, 0x00, 0x83,  /// HR MSB
      0x01, 0x82, 0x01, 0x48, 0xCC,  /// HR LSB
      0x01, 0x86, 0x01, 0x61, 0xE9,  /// SpO2
      0x01, 0x84, 0x01, 0x30, 0xB6,  /// REV
      0x01, 0x88, 0x01, 0x00, 0x8A,  /// reserved
      0x01, 0x90, 0x01, 0x00, 0x92,  /// reserved
      0x01, 0xA0, 0x01, 0x00, 0xA2,  /// reserved
      0x01, 0xC0, 0x01, 0x00, 0xC2,  /// reserved
      0x01, 0x00, 0x01, 0x00, 0x63,  /// SpO2-D
      0x01, 0x80, 0x01, 0x00, 0xE3,  /// SpO2 Fast
      0x01, 0x80, 0x01, 0x00, 0xE3,  /// SpO2 B-B
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
      0x01, 0x80, 0x01, 0x00, 0x82,  /// E-HR MSB
      0x01, 0x80, 0x01, 0x00, 0xCA,  /// E-HR LSB
      0x01, 0x80, 0x01, 0x00, 0xE3,  /// E-SpO2
      0x01, 0x80, 0x01, 0x00, 0xE3,  /// E-SpO2-D
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
      0x01, 0x80, 0x01, 0x00, 0x82,  /// HR-D MSB
      0x01, 0x80, 0x01, 0x00, 0xCA,  /// HR-D LSB
      0x01, 0x80, 0x01, 0x00, 0x82,  /// E-HR-D MSB
      0x01, 0x80, 0x01, 0x00, 0xCA,  /// E-HR-D LSB
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
      0x01, 0x80, 0x01, 0x00, 0x82,  /// reserved
    };
    uint8_t index;

    PF::Driver::Serial::Nonin::PacketReceiver packet_receiver;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus packetInputStatus;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus packetOutputStatus;
    PF::Driver::Serial::Nonin::PacketMeasurements SensorMeasurements;
    WHEN("Receiving first frame of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_packet[0]);
        REQUIRE(0x01 == ((test_packet[0][1]) & 0x01));
        THEN("Packet Input status shall return waiting status"){
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
    }

    WHEN("Receiving 0-24 frames of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_packet[0]);
      THEN("For the frames 2 to 24 Packet Input status shall return waiting status") {
        for(index = 1; index < (PF::Driver::Serial::Nonin::packet_size - 1); index++) {
          packetInputStatus = packet_receiver.input(test_packet[index]);
          REQUIRE((test_packet[index][1] & 0x01) != 0x01);
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
      }
    }

    WHEN("Receiving 25 frames of data from FrameReceiver") {
      for(index = 0; index < (PF::Driver::Serial::Nonin::packet_size - 1); index++) {
        packetInputStatus = packet_receiver.input(test_packet[index]);
      }
      THEN("For the 25th frame Packet Input status shall return available status") {
        packetInputStatus = packet_receiver.input(test_packet[24]);
        REQUIRE((test_packet[index][1] & 0x01) != 0x01);
        REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      }
    }
    AND_WHEN("packetInputStatus is available, invoke PacketReceiver::output to get the measurements") {
      for(index = 0; index < PF::Driver::Serial::Nonin::packet_size; index++) {
        packetInputStatus = packet_receiver.input(test_packet[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      packetOutputStatus = packet_receiver.output(SensorMeasurements);
      REQUIRE(packetOutputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus::available);
      THEN("Status Byte errors set") {
        REQUIRE(SensorMeasurements.signal_perfusion[0] == PF::Driver::Serial::Nonin::SignalAmplitude::no_perfusion);
        REQUIRE(SensorMeasurements.signal_perfusion[1] == PF::Driver::Serial::Nonin::SignalAmplitude::green_perfusion);
        REQUIRE(SensorMeasurements.signal_perfusion[2] == PF::Driver::Serial::Nonin::SignalAmplitude::yellow_perfusion);
        REQUIRE(SensorMeasurements.signal_perfusion[3] == PF::Driver::Serial::Nonin::SignalAmplitude::red_perfusion);
        REQUIRE(SensorMeasurements.sensor_alarm[4] == true);
        REQUIRE(SensorMeasurements.out_of_track[5] == true);
        REQUIRE(SensorMeasurements.artifact[6] == true);
        REQUIRE(SensorMeasurements.sensor_disconnect[7] == true);
        REQUIRE(SensorMeasurements.bit7[8] == true);
      }
    }
  }
}

SCENARIO("Validate the packets data with invalid data") {
  GIVEN("Valid 28 frames with 25 Frames of first packet and 2 invalid noise and 1 first frame of next packet ") {
    const Frame test_frames[58] = {
      /// First 23 frames
      {0x01, 0x81, 0x01, 0x00, 0x83},  /// HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2
      {0x01, 0x80, 0x01, 0x30, 0xB2},  /// REV
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2-D
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 Fast
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 B-B
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2-D
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR-D LSB
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR-D LSB
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      /// 2 frames of noise
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      /// New packet of first frames
      {0x01, 0x81, 0x01, 0x00, 0x83},  /// HR MSB
    };
    uint8_t index;

    PF::Driver::Serial::Nonin::PacketReceiver packet_receiver;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus packetInputStatus;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus packetOutputStatus;
    PF::Driver::Serial::Nonin::PacketMeasurements sensor_measurements;

    WHEN("Receiving first frame of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
        REQUIRE(0x01 == ((test_frames[0][1]) & 0x01));
        THEN("Packet Input status shall return waiting status"){
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
    }
    AND_WHEN("Receiving 2-24 frames of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
      THEN("For the frames 2 to 24 Packet Input status shall return waiting status") {
        for(index = 1; index < 24; index++) {
          packetInputStatus = packet_receiver.input(test_frames[index]);
          REQUIRE((test_frames[index][1] & 0x01) != 0x01);
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
      }
    }
    AND_WHEN("Valid 25th frame is received") {
      for(index = 0; index < 24; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      packetInputStatus = packet_receiver.input(test_frames[index]);
      THEN("packetInputStatus shall return available") {
        REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      }
    }
    AND_WHEN("packetInputStatus is available, invoke PacketReceiver::output to get the measurements") {
      for(index = 0; index < PF::Driver::Serial::Nonin::packet_size; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      packetOutputStatus = packet_receiver.output(sensor_measurements);
      REQUIRE(packetOutputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus::available);
      THEN("Validate the Heart Rate and SpO2") {
        REQUIRE(sensor_measurements.heart_rate == 72);
        REQUIRE(sensor_measurements.heart_rate_d == 72);
        REQUIRE(sensor_measurements.e_heart_rate == 72);
        REQUIRE(sensor_measurements.e_heart_rate_d == 72);
        REQUIRE(sensor_measurements.spo2 == 97);
        REQUIRE(sensor_measurements.spo2_d == 97);
        REQUIRE(sensor_measurements.e_spo2 == 97);
        REQUIRE(sensor_measurements.e_spo2_d == 97);
        REQUIRE(sensor_measurements.spo2_d_beat == 97);
        REQUIRE(sensor_measurements.spo2_d_fast == 97);
        REQUIRE(sensor_measurements.nonin_oem_revision == 48);
      }
    }
    AND_WHEN("Packet input status is available on input of 25 frames") {
      for(index = 0; index < 25; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      THEN("Input of 26th and 27th noise frame shall return missed_data") {
        for(index = 25; index < 27; index++) {
          packetInputStatus = packet_receiver.input(test_frames[index]);
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::missed_data);
        }
      }
    }
    AND_WHEN("Packet input status missed_data on input of 27 frames") {
      for(index = 0; index < 27; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::missed_data);
      THEN("Input of 28th frame (first frame of next packet) shall return waiting") {
        packetInputStatus = packet_receiver.input(test_frames[27]);
        REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
      }
    }
  }

  GIVEN("Valid 58 frames with 23 Frames of first packet and 25 frames of second packet ") {
    const Frame test_frames[58] = {
      /// First 23 frames
      {0x01, 0x81, 0x01, 0x00, 0x83},  /// HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2
      {0x01, 0x80, 0x01, 0x30, 0xB2},  /// REV
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2-D
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 Fast
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 B-B
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2-D
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR-D LSB
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR-D LSB
      /// New packet of 25 frames
      {0x01, 0x81, 0x01, 0x00, 0x83},  /// HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2
      {0x01, 0x80, 0x01, 0x30, 0xB2},  /// REV
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2-D
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 Fast
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2 B-B
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// E-SpO2-D
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR-D LSB
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// E-HR-D MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// E-HR-D LSB
      {0x01, 0x80, 0x01, 0x00, 0x82},  /// reserved
      {0x01, 0x80, 0x01, 0x00, 0x82}   /// reserved
    };
    uint8_t index;

    PF::Driver::Serial::Nonin::PacketReceiver packet_receiver;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus packetInputStatus;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus packetOutputStatus;
    PF::Driver::Serial::Nonin::PacketMeasurements SensorMeasurements;

    WHEN("Receiving first frame of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
        REQUIRE(0x01 == ((test_frames[0][1]) & 0x01));
        THEN("Packet Input status shall return waiting status"){
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
    }

    WHEN("Receiving 0-23 frames of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
        THEN("For the frames 2 to 23 Packet Input status shall return waiting status") {
          for(index = 1; index < 23; index++) {
            packetInputStatus = packet_receiver.input(test_frames[index]);
            REQUIRE((test_frames[index][1] & 0x01) != 0x01);
            REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
          }
        }
    }

    WHEN("Receiving 24 frames of data from FrameReceiver, on 24th frame packet Input Status shall be missed_data") {
     for(index = 0; index < 23; index++) {
       packetInputStatus = packet_receiver.input(test_frames[index]);
       REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
     }
     THEN("on 24th frame packet Input Status shall be missed_data")
     packetInputStatus = packet_receiver.input(test_frames[23]);
     REQUIRE((test_frames[index][1] & 0x01) == 0x01);
     REQUIRE(23 != PF::Driver::Serial::Nonin::packet_size);
     REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::missed_data);
    }

    WHEN("Receiving 47 frames of data from FrameReceiver, from 25th to 47 frames packet Input Status shall be waiting") {
      for(index = 0; index < 24; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      THEN("on 24th frame packet Input Status shall be missed_data")
      for(index = 24; index < 47; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
        REQUIRE((test_frames[index][1] & 0x01) != 0x01);
        REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
      }
    }
    WHEN("Receiving 47 frames of data from FrameReceiver, from 25th to 47 frames packet Input Status shall be waiting") {
      for(index = 0; index < 47; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      THEN("on 48th frame packet Input Status shall be available")
      packetInputStatus = packet_receiver.input(test_frames[index]);
      REQUIRE((test_frames[index][1] & 0x01) != 0x01);
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
    }
    AND_WHEN("packetInputStatus is available, invoke PacketReceiver::output to get the measurements") {
      for(index = 0; index < 48; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::available);
      packetOutputStatus = packet_receiver.output(SensorMeasurements);
      REQUIRE(packetOutputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus::available);
      THEN("Validate the Heart Rate and SpO2") {
        REQUIRE(SensorMeasurements.heart_rate == 72);
        REQUIRE(SensorMeasurements.heart_rate_d == 72);
        REQUIRE(SensorMeasurements.e_heart_rate == 72);
        REQUIRE(SensorMeasurements.e_heart_rate_d == 72);
        REQUIRE(SensorMeasurements.spo2 == 97);
        REQUIRE(SensorMeasurements.spo2_d == 97);
        REQUIRE(SensorMeasurements.e_spo2 == 97);
        REQUIRE(SensorMeasurements.e_spo2_d == 97);
        REQUIRE(SensorMeasurements.spo2_d_beat == 97);
        REQUIRE(SensorMeasurements.spo2_d_fast == 97);
        REQUIRE(SensorMeasurements.nonin_oem_revision == 48);
      }
    }
  }

  GIVEN("5 valid frames") {
    const Frame test_frames[5] = {
      {0x01, 0x81, 0x01, 0x00, 0x83},  /// HR MSB
      {0x01, 0x80, 0x01, 0x48, 0xCA},  /// HR LSB
      {0x01, 0x80, 0x01, 0x61, 0xE3},  /// SpO2
      {0x01, 0x80, 0x01, 0x30, 0xB2},  /// REV
      {0x01, 0x80, 0x01, 0x00, 0x82}   /// reserved
    };
    uint8_t index;

    PF::Driver::Serial::Nonin::PacketReceiver packet_receiver;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus packetInputStatus;
    PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus packetOutputStatus;
    PF::Driver::Serial::Nonin::PacketMeasurements SensorMeasurements;

    WHEN("Receiving first frame of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
        REQUIRE(0x01 == ((test_frames[0][1]) & 0x01));
        THEN("Packet Input status shall return waiting status"){
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
    }

    WHEN("Receiving 0-3 frames of data from FrameReceiver") {
      packetInputStatus = packet_receiver.input(test_frames[0]);
      THEN("For the frames 2 to 5 Packet Input status shall return waiting status") {
        for(index = 1; index < 5; index++) {
          packetInputStatus = packet_receiver.input(test_frames[index]);
          REQUIRE((test_frames[index][1] & 0x01) != 0x01);
          REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
        }
      }
    }
    AND_WHEN("packetInputStatus is waiting, invoke PacketReceiver::output shall return waiting") {
      for(index = 0; index < 5; index++) {
        packetInputStatus = packet_receiver.input(test_frames[index]);
      }
      REQUIRE(packetInputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketInputStatus::waiting);
      THEN("On invoking PacketReceiver::output shall return waiting") {
        packetOutputStatus = packet_receiver.output(SensorMeasurements);
        REQUIRE(packetOutputStatus == PF::Driver::Serial::Nonin::PacketReceiver::PacketOutputStatus::waiting);
      }
    }
  }
}
