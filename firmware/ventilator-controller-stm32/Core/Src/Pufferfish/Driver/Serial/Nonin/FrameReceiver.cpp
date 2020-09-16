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
  if (packetStatus != startOfPacketStatus::notAvailable)
  {
    /* Return the Start of packet status */
    return packetStatus;
  }

  /* Update the frame buffer with new byte received */
  frameBuffer[bufferLength] = newByte;

  /* Check the buffer length to frame size */
  if ((bufferLength+1) != frameSize)
  {
    /* Return the Start of packet status */
    return packetStatus;
  }

  /* Check for the byte 1 is 01 and 1st bit of byte 2 is set for the start of frame */
  if(frameBuffer[0] == 0x01 && (frameBuffer[1] & 0x01) == 0x01 )
  {
    /* Checksum validation */
    if (((frameBuffer[0]+frameBuffer[1]+frameBuffer[2]+frameBuffer[3]) % 256) == frameBuffer[4])
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

  /* Return the Start of packet status */
  return packetStatus;
}

FrameReceiver::FrameInputStatus FrameReceiver::input(const uint8_t newByte) {
  /* Validate the Start of Packet available*/
  if( this-> validateStartOfPacket(newByte) == startOfPacketStatus::notAvailable)
  {
    /* Increment the bufferLength by 1 */
    bufferLength++;
    /* Return the frame status is not available */
    return FrameInputStatus::notAvailable;
  }

  /* Fill the frame buffer with new byte received */
  frameBuffer[bufferLength] = newByte;

  /* Validate the buffer length is equal to expected frame size */
  if ((bufferLength+1) == frameSize) {
    /* On complete frame reception change the buffer length to zero */
    bufferLength = 0;
    /* Update the frame input status to available */
    inputStatus = FrameInputStatus::available;
    /* Return the input status */
    return inputStatus;
  }

  /* Increment the buffer length */
  bufferLength++;
  /* Return the frame input status to input ready to receive more bytes to fill frame*/
  inputStatus = FrameInputStatus::waiting;
  return inputStatus;
}

FrameReceiver::FrameOutputStatus FrameReceiver::output(Frame &frame, uint8_t &frameIndex) {
  uint8_t frameCHK;

  /* Check for the frame availability in the buffer */
  if (inputStatus != FrameInputStatus::available){
    return FrameOutputStatus::waiting;
  }

  /* Calculate the frame checksum */
  frameCHK = static_cast<uint8_t>((frameBuffer[0] +  frameBuffer[1] +  frameBuffer[2] +  frameBuffer[3]) % 256);

  /* Validate the checksum */
  if (frameCHK != frameBuffer[4]) {
    /* Reset the the packet status to not available to read the next packet start */
    packetStatus = startOfPacketStatus::notAvailable;
    bufferLength = 0;
    /* Return Checksum error */
    return FrameOutputStatus::checksumError;
  }

  /* Validate the Status byte by 0x80 */
  if ((frameBuffer[1] & 0x80) != 0x80){
    /* Reset the the packet status to not available to read the next packet start */
    packetStatus = startOfPacketStatus::notAvailable;
    bufferLength = 0;
    /* Return status byte error */
    return FrameOutputStatus::statusByteError;
  }
  /* Update the Output frame */
  frame = frameBuffer;

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
  return FrameOutputStatus::available;
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
