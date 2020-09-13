/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */
#include <Pufferfish/Driver/Serial/Nonin/FrameReceiver.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

FrameReceiver::startOfPacketStatus FrameReceiver::validateStartOfPacket(const uint8_t newByte)
{
  uint8_t byteIndex;
  /* Check for the start of packet status for available */
  if (packetStatus == startOfPacketStatus::notAvailable)
  {
    /* Update the frame buffer with new byte received */
    frameBuffer[bufferLength] = newByte;
    /* Check the buffer length to frame size */
    if ((bufferLength+1) == frameSize)
    {
      /* Check for the byte 1 is 01 and 1st bit of byte 2 is set for the start of frame */
      if(frameBuffer[0] == 0x01 && (frameBuffer[1] & 0x01) == 0x01 )
      {
        /* Checksum validation */
        if (frameBuffer[0]+frameBuffer[1]+frameBuffer[2]+frameBuffer[3] == (frameBuffer[4] % 256))
        {
          /* on successful validation update the start of packet status as available */
          packetStatus = startOfPacketStatus::available;
          /* return the start of packet status as available */
          return packetStatus;
        }
      }
      /* On start of frame is not available update the buffer and decrement the buffer size*/
      for (byteIndex = 0; byteIndex < 4;byteIndex ++ ){
        frameBuffer[byteIndex] = frameBuffer[byteIndex + 1];
      }
      bufferLength--;
    }
  }
  /* Return the Start of packet status */
  return packetStatus;
}

FrameReceiver::frameInputStatus FrameReceiver::input(const uint8_t newByte) {
  /* Validate the Start of Packet available*/
  if( this-> validateStartOfPacket(newByte) == startOfPacketStatus::notAvailable)
  {
    /* Increment the bufferLength by 1 */
    bufferLength++;
    /* Return the frame status is not available */
    return frameInputStatus::notAvailable;
  }

  /* Fill the frame buffer with new byte received */
  frameBuffer[bufferLength] = newByte;

  /* Validate the buffer length is equal to expected frame size */
  if ((bufferLength+1) == frameSize) {
    /* On complete frame reception change the buffer length to zero */
    bufferLength = 0;
    /* update the frame input status to available */
    inputStatus = frameInputStatus::available;
    /* return the input status */
    return inputStatus;
  } else {
    /* Increment the buffer length */
    bufferLength++;
    /* return the frame input status to input ready to receive more bytes to fill frame*/
    inputStatus = frameInputStatus::inputReady;
    return inputStatus;
  }
}

FrameReceiver::frameOutputStatus FrameReceiver::output(std::array<uint8_t, frameSize> &frame, uint8_t &frameIndex, uint8_t &statusByte) {
  uint8_t frameCHK, byteIndex;
  /* Check for the frame availability in the buffer */
  if (inputStatus == frameInputStatus::available){
    /* Update Status Byte */
    statusByte = frameBuffer[1];
    /* Validate the Status byte by 0x80 or 0x81 */
    if ((frameBuffer[1] & 0x78) != 0x00){
      packetStatus = startOfPacketStatus::notAvailable;
      /* Return status byte error */
      return frameOutputStatus::statusByteError;
    } else {

      /* Calculate the frame checksum */
      frameCHK = static_cast<uint8_t>((frameBuffer[0] +  frameBuffer[1] +  frameBuffer[2] +  frameBuffer[3]) % 256);

      /* Validate the checksum */
      if (frameCHK != frameBuffer[4]) {
        /* Update the start of packet status as not available */
        packetStatus = startOfPacketStatus::notAvailable;

        /* Return Checksum error */
        return frameOutputStatus::checksumError;
      }

      /* Update output frame buffer with frameBuffer */
      for (byteIndex = 0; byteIndex < frameSize; byteIndex++) {
        frame[byteIndex] = frameBuffer[byteIndex];
      }

      /* Check the frame received is first frame in the packet SYNC bit is 1 */
      if ( (frameBuffer[1] & 0x01) == 0x01) {
        /* Update the frame buffer index to 0 */
        frameBufferIndex = 0;
      } else {
        /* Update the frame buffer index */
        frameBufferIndex = frameBufferIndex + 1;
      }
      /* Update the frame index */
      frameIndex = frameBufferIndex;
      /* Return frame is available */
      return frameOutputStatus::available;
    }
  }
  else{
    /* Return waiting status to receive complete frame */
    return frameOutputStatus::waiting;
  }
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
