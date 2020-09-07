/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */
#include "Pufferfish/HAL/STM32/FrameSplitter.h"

namespace Pufferfish {
namespace HAL {

FrameSplitter::startOfPacketStatus FrameSplitter::validateStartOfPacket(const uint8_t newByte)
{
  if (packetStatus == startOfPacketStatus::notAvailable)
  {
    frameBuffer[bufferLength] = newByte;
    bufferLength++;
    if (bufferLength == 5)
    {
      if(frameBuffer[0] == 0x01 && (frameBuffer[1] &0x01) == 0x01 )
      {
        /* Checksum validation */
        if (frameBuffer[0]+frameBuffer[1]+frameBuffer[2]+frameBuffer[3] == (frameBuffer[4] % 256))
        {
          packetStatus = startOfPacketStatus::available;
          return packetStatus;
        }
      }
      frameBuffer[0] = frameBuffer[1];
      frameBuffer[1] = frameBuffer[2];
      frameBuffer[2] = frameBuffer[3];
      frameBuffer[3] = frameBuffer[4];
      bufferLength--;
    }
  }
  return packetStatus;
}

FrameSplitter::frameInputStatus FrameSplitter::input(const uint8_t newByte) {

  if( this-> validateStartOfPacket(newByte) == startOfPacketStatus::notAvailable)
  {
    return frameInputStatus::notAvailable;
  }

  frameBuffer[bufferLength] = newByte;
  bufferLength++;
  /* Validate the buffer length is equal to expected frame size */
  if (bufferLength == frameSize) {
    inputStatus = frameInputStatus::available;
    return inputStatus;
  } else {
    inputStatus = frameInputStatus::inputReady;
    return inputStatus;
  }
}

FrameSplitter::frameOutputStatus FrameSplitter::output(uint8_t *outputBuffer) {
  uint8_t frameCHK, byteIndex;
  /* Check for the frame availability in the buffer */
  if (inputStatus == frameInputStatus::available){
    /* Validate the Status byte */
    if ((frameBuffer[1] & 0xFE) != 0x00){
      packetStatus = startOfPacketStatus::notAvailable;
      /* Return status byte error */
      return frameOutputStatus::statusByteError;
    } else {
      frameCHK = (frameBuffer[0] +  frameBuffer[1] +  frameBuffer[2] +  frameBuffer[3]) % 256;

      /* Validate the checksum */
      if (frameCHK != frameBuffer[4]) {
        packetStatus = startOfPacketStatus::notAvailable;
        /* Return Checksum error */
        return frameOutputStatus::checksumError;
      }

      /* Update the outputBuffer with frameBuffer */
      for (byteIndex = 0; byteIndex < frameSize; byteIndex++) {
        outputBuffer[byteIndex] = frameBuffer[byteIndex];
      }

      /* Check the frame received is first frame in the packet */
      if ( frameBuffer[1] != 0x01) {
        /* Return first frame of packet */
        return frameOutputStatus::firstFrame;
      } else {
        /* Return frame is available */
        return frameOutputStatus::available;
      }
    }
  }
  else{
    return frameOutputStatus::waiting;
  }
}

} // HAL
} // Pufferfish
