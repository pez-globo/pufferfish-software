/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include <Pufferfish/Driver/Serial/Nonin/FrameBuffer.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

BufferStatus FrameBuffer::input(const uint8_t byte) {
  /* Validate the frame buffer is already full */
  if (frameIndex == frameMaxSize) {
    return BufferStatus::full;
  }
  /* Update the frameBuffer with new byte received */
  frameBuffer[frameIndex] = byte;

  /* Increment the frame buffer index */
  frameIndex = frameIndex + 1;

  /* On frame index is equal to frame size reture frame status as Ok */
  if (frameIndex == frameMaxSize) {
    return BufferStatus::ok;
  }
  
  /* Return the frame buffer status as partial and frame buffer should be updated with new bytes */
  return BufferStatus::partial;
}

BufferStatus FrameBuffer::output(Frame &frame) {
  /* Check for available of frame */
  if (frameIndex != frameMaxSize) {
    /* Return frame buffer is partial update */
    return BufferStatus::partial;
  }
  /* Update the frame buffer */
  frame = frameBuffer;

  /* Return ok on frame buffer updated */
  return BufferStatus::ok;
}

void FrameBuffer::reset(){
  /* Update the index of frame buffer to zero */
  frameIndex = 0;
}

void FrameBuffer::updateSOF(){
  uint8_t index;
  /* Update the frame buffer and index to receive new byte data */
  for(index = 0; index < (frameMaxSize-1);index++){
    frameBuffer[index] = frameBuffer[index+1];
  }
  frameIndex = frameMaxSize-1;
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
