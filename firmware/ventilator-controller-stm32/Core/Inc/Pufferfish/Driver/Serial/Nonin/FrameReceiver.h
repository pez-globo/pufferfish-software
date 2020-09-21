/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include <Pufferfish/Driver/Serial/Nonin/FrameBuffer.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/*
 * FrameSplitter reads input data from sensor and Output the frame on available
 * on error throw error on output frame.
 */
class FrameReceiver {
 public:
  /* FrameReceiver input status return values */
  enum class FrameInputStatus {
    waiting = 0,   /// Input is ready to receive new bytes of sensor data
    notAvailable,     /// Input status is not available or error in frame
    checksumError,    /// Error in checksum of a frame
    available         /// frame is available
    };

  /* FrameReceiver output status return values */
  enum class FrameOutputStatus {
    available = 0,    /// frame output is available for packet fill
    waiting           /// frame output is waiting for complete frame
    };

  /**
   * Constructor for FrameReceiver
   */
  FrameReceiver() {
  }

  /**
   * @brief  Call input to fill frame until it returns available, then call output
   * @param  New byte data received from sensor
   * @return Frame input status on filling a frame
   */
  FrameInputStatus input(const uint8_t newByte);

  /**
   * @brief  output method updates the frame on available or throws error/waiting
   * @param  outputBuffer to receive frame from frame receiver
   * @param  frameIndex index of frame in a packet
   * @param  status byte of frame
   * @return Frame output status after validating the frame
   */
  FrameOutputStatus output(Frame &frame);

 private:
  bool updateFrameBuffer(uint8_t newByte);

  /* Frame Buffer stores bytes of data received from sensor */
  Frame frameBuffer;

  /* Frame input status */
  FrameInputStatus inputStatus;

  FrameBuffer frameBuf;

  /* Start of packet status */
  bool startOfFrameStatus = false;

};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
