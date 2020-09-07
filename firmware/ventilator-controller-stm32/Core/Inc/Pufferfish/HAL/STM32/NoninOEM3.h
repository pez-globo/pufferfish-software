/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "Pufferfish/HAL/STM32/PacketParser.h"


namespace Pufferfish {
namespace HAL {

class NoninOEM {

 public:

  /* PacketParser Input status */
  enum class NoninPacketStatus {
    available = 0,
    waiting
  };

  struct PacketData{
    uint16_t HeartRate;
    uint8_t SpO2;
  };


  /*
   * Constructor for PacketParser
   */
  NoninOEM(noninOEMUART &uart) : frame(uart){
  }

  /**
   * Input is called until packet is filled
   */
  NoninPacketStatus output(PacketMeasurements SensorMeasurements);

 private:

  /* Create a private bufferredUART<256, 256> reference */
  noninOEMUART &frame;

  /* Create an object of PacketParser */
  PacketParser packetData;
};

} // HAL
} // Pufferfish
