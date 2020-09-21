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

NoninOEM::NoninPacketStatus NoninOEM::output(PacketMeasurements &sensorMeasurements,
                                             StatusByteError &frameErrorStatus){
  uint8_t readByte;

  /* Read a byte from BufferedUART */
  if(noninUART.read(readByte) == BufferStatus::empty)
  {
    /* Return waiting to receive new bytes of data from sensor */
    return NoninPacketStatus::waiting;
  }

  /* FrameReceiver */
  /* Input byte to frame receiver and validate the frame available */
  switch(frameReceiver.input(readByte)){
    /* Return sensor status is waiting to receive more bytes of data */
    case FrameReceiver::FrameInputStatus::checksumError : return NoninPacketStatus::checksumError;

    /* Return sensor status is waiting to receive more bytes of data */
    case FrameReceiver::FrameInputStatus::waiting : return NoninPacketStatus::waiting;

    /* Return the frame status is not available */
    case FrameReceiver::FrameInputStatus::notAvailable : return NoninPacketStatus::notAvailable;

    /* On PacketInputStatus available continue */
    case FrameReceiver::FrameInputStatus::available    : break;
  }

  if (frameReceiver.input(readByte) != FrameReceiver::FrameInputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* On frame input available invoke output method to receive frame */
  if (frameReceiver.output(frameBuffer) == FrameReceiver::FrameOutputStatus::waiting)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* PaketParser */
  /* Input frame to packet and validate the frame available */
  switch(packetReceiver.input(frameBuffer))
  {
    /* Return sensor status is waiting to receive more frames of data */
    case PacketReceiver::PacketInputStatus::waiting      : return NoninPacketStatus::waiting;

    /* Discard the packet due to status byte error, wait for the new packet to receive */
    case PacketReceiver::PacketInputStatus::notAvailable : return NoninPacketStatus::notAvailable;

    /* On PacketInputStatus available continue */
    case PacketReceiver::PacketInputStatus::available    : break;
  }

  /* On packet input available invoke output method to read sensor measurements  */
  if (packetReceiver.output(sensorMeasurements, frameErrorStatus) !=  PacketReceiver::PacketOutputStatus::available)
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
