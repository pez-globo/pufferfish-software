/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include <stdint.h>
#include "Pufferfish/HAL/STM32/FrameSplitter.h"


namespace Pufferfish {
namespace HAL {

/* Structure defines the sensor data in packet for measurements */
typedef struct _PacketMeasurements{
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
}PacketMeasurements;

/*
 *
 */
class PacketParser {

 public:
  /* PacketParser Input status */
  enum class PacketInputStatus {
    available = 0,
    inputReady,
    notAvailable
    };

  /* PacketParser Output status */
  enum class PacketOutputStatus {
    available = 0,
    waiting,
    invalidLength
    };

  /*
   * Constructor for PacketParser
   */
  PacketParser(){
  }

  void reset();
  /**
   * input is called until packet is filled
   */
  PacketInputStatus input(uint8_t &readByte);

  PacketOutputStatus output(PacketMeasurements SensorMeasurements);

  PacketMeasurements readPacketMeasurements(uint8_t *PacketBuffer);

 private:

  /* Create an object of FrameSplitter */
  FrameSplitter frames;

  /* Size of Packet */
  static const uint8_t PacketSize = 25;

  /* Size of Frame */
  static const uint8_t PacketframeSize = FrameSplitter::frameSize;

  /* Frame Buffer stores bytes of data received from PacketParser input */
  uint8_t PacketBuffer[PacketSize * PacketframeSize];

  PacketMeasurements mMeasurements;

  /* Frame Buffer length red from PacketParser input */
  uint8_t PacketLength;

  PacketInputStatus inputStatus;
};

} // HAL
} // Pufferfish
