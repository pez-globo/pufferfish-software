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
                                             PacketReceiver::StatusByteError &frameErrorStatus,
                                             SignalPerfusion &perfusionStatus){
  uint8_t readByte;

  /* Read a byte from BufferedUART */
  if(noninUART.read(readByte) == BufferStatus::empty)
  {
    /* Return waiting to receive new bytes of data from sensor */
    return NoninPacketStatus::waiting;
  }

  /* FrameReceiver */
  /* Input byte to frame receiver and validate the frame available */
  if (frameReceiver.input(readByte) != FrameReceiver::FrameInputStatus::available)
  {
    /* Return sensor status is waiting to receive more bytes of data */
    return NoninPacketStatus::waiting;
  }

  /* On frame input available invoke output method to receive frame */
  switch(frameReceiver.output(frameBuffer, frameIndex)) {
    /* Return sensor status as checksum error */
    case FrameReceiver::FrameOutputStatus::checksumError   : return NoninPacketStatus::checksumError;

    /* Error or Noise in receiving status byte from sensor */
    case FrameReceiver::FrameOutputStatus::statusByteError : return NoninPacketStatus::statusByteError;

    /* Needs to receive more bytes from sensor */
    case FrameReceiver::FrameOutputStatus::waiting         : return NoninPacketStatus::waiting;

    /* On FrameOutputStatus available continue */
    case FrameReceiver::FrameOutputStatus::available       : break;
  }

  /* PaketParser */
  /* Input frame to packet and validate the frame available */
  switch(packetReceiver.input(frameBuffer, frameIndex, frameErrorStatus, perfusionStatus))
  {
    /* Return sensor status is waiting to receive more frames of data */
    case PacketReceiver::PacketInputStatus::waiting      : return NoninPacketStatus::waiting;

    /* Discard the packet due to status byte error, wait for the new packet to receive */
    case PacketReceiver::PacketInputStatus::notAvailable : return NoninPacketStatus::notAvailable;

    /* On PacketInputStatus available continue */
    case PacketReceiver::PacketInputStatus::available    : break;
  }

  /* On packet input available invoke output method to read sensor measurements  */
  if (packetReceiver.output(sensorMeasurements) !=  PacketReceiver::PacketOutputStatus::available)
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
