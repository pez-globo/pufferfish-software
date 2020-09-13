/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include <Pufferfish/Driver/Serial/Nonin/FrameReceiver.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/* Structure defines the sensor data in packet for measurements */
struct PacketMeasurements {
  uint16_t heartRate;
  uint8_t  SpO2;
  uint8_t SpO2D;
  uint8_t SpO2DFast;
  uint8_t SpO2DBeat;
  uint16_t eHeartRate;
  uint8_t eSpO2;
  uint8_t eSpO2D;
  uint16_t HeartRateD;
  uint16_t eHeartRateD;
  uint8_t  noninOEMRevision;
};

/*
 *
 */
class PacketParser {
 public:
  /* PacketParser Input status */
  enum class PacketInputStatus {
    available = 0,     /// Input is available to read output
    waiting,           /// Input is wait to read more bytes
    };

  /* PacketParser Output status */
  enum class PacketOutputStatus {
    available = 0,   /// Output measurements are available
    waiting          /// Output is waiting to receive more byte for measurements
    };

  /**
   * @brief  Constructor for PacketParser
   * @param  None
   */
  PacketParser(){
  }

  /**
   * @brief  Reset the packet on error occurred in input
   * @param  None
   * @return None
   */
  void reset();

  /**
   * @brief  Input is called until packet is filled
   * @param  frame input frame of 5 bytes
   * @param  frameIndex frame index of a packet
   * @return Packet input status on available of packet
   */
  PacketInputStatus input(std::array<uint8_t, FrameReceiver::frameSize> frame, uint8_t frameIndex);

  /**
   * @brief  Output is called after the input status is available to read measurements
   * @param  sensorMeasurements is updated on available of measurements
   * @return Packet Output status on available of measurements
   */
  PacketOutputStatus output(PacketMeasurements &sensorMeasurements);


 private:
  /* */
  void readPacketMeasurements(PacketMeasurements &SensorMeasurements);

  /* Size of Packet */
  static const uint8_t packetSize = 25;

  /* Size of Frame */
  static const uint8_t PacketframeSize = FrameReceiver::frameSize;

  std::array<std::array<uint8_t, FrameReceiver::frameSize>, packetSize> packetData;

  /* Frame Buffer stores bytes of data received from PacketParser input */
  uint8_t PacketBuffer[packetSize * PacketframeSize];

  /* Packet Buffer length red from PacketParser input */
  uint8_t packetBufferLength;

  /* Input status for a packet */
  PacketInputStatus inputStatus;
};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
