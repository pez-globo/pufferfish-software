/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include "Pufferfish/Driver/Serial/Nonin/NoninOEM3.h"

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

NoninOEM::NoninPacketStatus NoninOEM::output(PacketMeasurements &sensorMeasurements){
  uint8_t readByte;
  /* */
  FrameReceiver::frameInputStatus returnFrameInputStatus;
  FrameReceiver::frameOutputStatus returnFrameOutputStatus;
  /* */
  PacketParser::PacketInputStatus returnPacketInputStatus;
  PacketParser::PacketOutputStatus returnPacketOutputStatus;

  /* Read a byte from BufferedUART */
  if(noninUART.read(readByte) == BufferStatus::empty)
  {
    /* Return waiting to receive new bytes of data from sensor */
    return NoninPacketStatus::waiting;
  }

  /* FrameReceiver */
  /* Input byte to frame receiver and validate the frame available */
  returnFrameInputStatus = frameData.input(readByte);
  if (returnFrameInputStatus != FrameReceiver::frameInputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* On frame input available invoke output method to receive frame */
  returnFrameOutputStatus = frameData.output(frameBuffer, frameIndex, statusByte);
  if (returnFrameOutputStatus != FrameReceiver::frameOutputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* PaketParser */
  /* Input frame to packet and validate the frame available */
  returnPacketInputStatus = packetData.input(frameBuffer, frameIndex);
  if(returnPacketInputStatus != PacketParser::PacketInputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* On packet input available invoke output method to read sensor measurements  */
  returnPacketOutputStatus = packetData.output(sensorMeasurements);
  if (returnPacketOutputStatus !=  PacketParser::PacketOutputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* Return available on measurements are available */
  return NoninPacketStatus::available;
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
