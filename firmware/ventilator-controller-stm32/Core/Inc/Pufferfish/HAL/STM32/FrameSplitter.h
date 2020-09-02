/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "PacketParser.h"

namespace Pufferfish {
namespace HAL {

/*
 * FrameSplitter reads input data from PacketParser and Output the frame on available
 * on error throw error on output frame.
 */
class FrameSplitter {
 public:

  /* FrameSplitter input status return values */
  enum class frameInputStatus {
    inputReady = 0,
    available
    };

  /* FrameSplitter output status return values */
  enum class frameOutputStatus {
    available = 0,
    waiting,
    firstFrame,
    checksumError,
    statusByteError
    };

  /*
   * Constructor for FrameSplitter
   */
  FrameSplitter() {
  }

  /**
   * Call this until it returns available, then call output
   */
  frameInputStatus input(uint8_t newByte);

  /*
   * output method updates the frame on available or throws error/waiting
   */
  frameOutputStatus output(uint8_t *outputBuffer);

 private:
  /* Size of Frame */
  static const size_t frameSize = 5;

  /* Packet parser object is created to read input byte */
  PacketParser PacketRead;

  /* Frame Buffer stores bytes of data received from PacketParser input */
  uint8_t frameBuffer[frameSize];

  /* Frame Buffer length red from PacketParser input */
  size_t bufferLength;

  frameInputStatus inputStatus;

  };

} // HAL
} // Pufferfish
