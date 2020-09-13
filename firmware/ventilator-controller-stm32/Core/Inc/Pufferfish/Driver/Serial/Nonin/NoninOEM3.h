/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "Pufferfish/Driver/Serial/Nonin/FrameReceiver.h"
#include "Pufferfish/Driver/Serial/Nonin/PacketParser.h"
#include "Pufferfish/HAL/STM32/BufferedUART.h"

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/* Define noninOEMUART for BufferedUART with 512 bytes of received buffer */
using noninOEMUART = HAL::BufferedUART<512, 10>;

/**
 * NoninOEM class to receive a byte from Nonin OEM III using UART and calculates the
 * measurements on complete packet availability and returns the measurements
 */
class NoninOEM {
 public:
  /* PacketParser Input status */
  enum class NoninPacketStatus {
    available = 0,  /// Packet/measurements is available
    waiting         /// Packet/measurements is waiting to receive more bytes of data
  };

  /**
   * Constructor for NoninOEM
   * @param  noninOEMUART BufferredUART with 512 bytes reception buffer
   */
  NoninOEM(volatile noninOEMUART &uart) : noninUART(uart){
  }

  /**
   * @brief  Method inputs the byte to packet and reads the packet measurements on availability
   * @param  sensorMeasurements is updated on available of packet/measurements
   * @return returns the status of Nonin OEM III packet measurements
   */
  NoninPacketStatus output(PacketMeasurements &sensorMeasurements);

 private:
  /* Create an object bufferredUART with 512 bytes of reception buffer */
  volatile noninOEMUART &noninUART;

  /* Create an object of FrameReceiver */
  FrameReceiver frameData;

  /* Create an object of PacketParser */
  PacketParser packetData;

  /* Frame Buffer stores bytes of data received from PacketParser input */
  std::array<uint8_t, FrameReceiver::frameSize> frameBuffer;

  uint8_t frameIndex, statusByte;

};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
