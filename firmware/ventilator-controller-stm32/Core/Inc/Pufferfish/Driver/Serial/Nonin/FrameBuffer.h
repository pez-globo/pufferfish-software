/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  An interrupt-safe byte stream backed by a circular buffer.
 *  Inspired by https://hackaday.com/2015/10/29/embed-with-elliot-going-round-with-circular-buffers/
 */

#pragma once

#include <stddef.h>
#include <array>
#include <stdint.h>
#include "Pufferfish/Statuses.h"
#include "Pufferfish/HAL/Types.h"

/* Frame */
const uint8_t frameMaxSize = 5;
using Frame = std::array<uint8_t, frameMaxSize>;

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/**
 * FrameBuffer class to update buffer
 */
class FrameBuffer {
public:
  FrameBuffer(){
  }

  /**
   * @brief  Input method to update byte data into frame
   * @param  readByte byte data input
   * @return Frame buffer input status
   */
  BufferStatus input(const uint8_t readByte);

  /**
   * @brief  Output method to invoked on frame available
   * @param  frame data output
   * @return Frame buffer output status
   */
  BufferStatus output(Frame &frame);

  /**
   * @brief  reset the frame
   * @param  None
   * @return None
   */
  void reset();

  /**
   * @brief  Update the frame for Start of frame validation
   * @param  None
   * @return None
   */
  void updateSOF();

private:
  /* frame buffer */
  Frame frameBuffer;
  /* frame byte index */
  uint8_t frameIndex = 0;
};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
