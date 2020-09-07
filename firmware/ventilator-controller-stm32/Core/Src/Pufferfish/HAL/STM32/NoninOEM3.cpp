/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include <stdint.h>
#include "Pufferfish/HAL/STM32/NoninOEM3.h"


namespace Pufferfish {
namespace HAL {

NoninOEM::NoninPacketStatus NoninOEM::output(PacketMeasurements SensorMeasurements){
  uint8_t readByte;
  if(frame.read(readByte) == BufferStatus::empty)
  {
    return NoninPacketStatus::waiting;
  }

  if (packetData.input(readByte) == PacketParser::PacketInputStatus::available)
  {
    if (packetData.output(SensorMeasurements) == PacketParser::PacketOutputStatus::available)
    {
      return NoninPacketStatus::available;
    }
  }
  return NoninPacketStatus::waiting;
}

} // HAL
} // Pufferfish
