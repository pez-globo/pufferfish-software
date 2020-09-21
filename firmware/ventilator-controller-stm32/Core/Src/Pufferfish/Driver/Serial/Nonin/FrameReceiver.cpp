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

bool validateStartOfFrame(const Frame newFrame, FrameBuffer &frameBuf)
{
  /* Check for the byte 1 is 01 and 1st bit of byte 2 is set for the start of frame */
  if(newFrame[0] == 0x01 && (newFrame[1] & 0x81) == 0x81 )
  {
    /* Checksum validation */
    if (((newFrame[0]+newFrame[1]+newFrame[2]+newFrame[3]) % 256) == newFrame[4])
    {
      /* return the start of packet status as available */
      return true;
    }
  }

  /* Update the FrameBuffer to receive next byte data */
  frameBuf.updateSOF();

  /* Return the Start of packet status */
  return false;
}

bool FrameReceiver::updateFrameBuffer(uint8_t newByte)
{
  /* Input the new byte received and check for frame availability */
  if(frameBuf.input(newByte) == BufferStatus::partial)
  {
    /* return false on frame is not available */
    return false;
  }

  /* On frame available update the frameBuffer with new frame available */
  if(frameBuf.output(frameBuffer) == BufferStatus::partial)
  {
    /* return false on frame is not available */
    return false;
  }

  /* Return true once frame is available */
  return true;
}

FrameReceiver::FrameInputStatus FrameReceiver::input(const uint8_t newByte) {
  /* Update the frame buffer with new byte received */
  if(this->updateFrameBuffer(newByte) == false)
  {
    /* On more bytes are required to fill the frame return the inputStatus as waiting */
    inputStatus = FrameInputStatus::waiting;
    return inputStatus;
  }

  /* Check for Start of frame availability */
  if(startOfFrameStatus == false)
  {
    /* On Start of frame not available invoke validateStartOfFrame */
    if(validateStartOfFrame(frameBuffer, frameBuf) == false)
    {
      /* Return the frame status is not available */
      inputStatus = FrameInputStatus::notAvailable;
      return inputStatus;
    }
    /* On available of start of frame update the status to true */
    startOfFrameStatus = true;
  }

  /* Validate the checksum */
  if (static_cast<uint8_t>((frameBuffer[0] +  frameBuffer[1] +  frameBuffer[2] +  frameBuffer[3]) % 256) != frameBuffer[4]) {
    /* Reset the the packet status to not available to read the next packet start */
    startOfFrameStatus = false;
    /* Return Checksum error */
    return FrameInputStatus::checksumError;
  }

  /* Return the frame input status to input ready to receive more bytes to fill frame*/
  inputStatus = FrameInputStatus::available;
  return inputStatus;
}

FrameReceiver::FrameOutputStatus FrameReceiver::output(Frame &frame) {

  /* Check for the frame availability in the buffer */
  if (inputStatus != FrameInputStatus::available){
    return FrameOutputStatus::waiting;
  }

  /* Update the Output frame */
  frame = frameBuffer;

  frameBuf.reset();


  /* Return frame is available */
  return FrameOutputStatus::available;
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
