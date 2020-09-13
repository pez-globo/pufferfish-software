/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <array>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/*
 * FrameSplitter reads input data from PacketParser and Output the frame on available
 * on error throw error on output frame.
 */
class FrameReceiver {
 public:
  /* Size of Frame */
  static const uint8_t frameSize = 5;

  /* FrameSplitter input status return values */
  enum class frameInputStatus {
    inputReady = 0,   /// Input is ready to receive new bytes of sensor data
    notAvailable,     /// Input status is not available or error in frame
    available         /// frame is available
    };

  /* FrameSplitter output status return values */
  enum class frameOutputStatus {
    available = 0,    /// frame output is available for packet fill
    waiting,          /// frame output is waiting for complete frame
    checksumError,    /// Error in checksum of a frame
    statusByteError,  /// Error is status byte readings
    frameCountError   /// Missing the 1 or more frames
    };

  /*
   * Constructor for FrameSplitter
   */
  FrameReceiver() {
  }

  /**
   * @brief  Call input to fill frame until it returns available, then call output
   * @param  New byte data received from sensor
   * @return Frame input status on filling a frame
   */
  frameInputStatus input(const uint8_t newByte);

  /*
   * @brief  output method updates the frame on available or throws error/waiting
   * @param  outputBuffer to reciev frame from frame receiver
   * @parma  frameIndex index of frame in a packet
   * @parma  status byte of frame
   * @return Frame output status after validating the frame
   */
  frameOutputStatus output(std::array<uint8_t, frameSize> &frame, uint8_t &frameIndex, uint8_t &statusByte);

 private:
  /* enum class for the start of packet status */
  enum class startOfPacketStatus {
    available = 0,  /// Status of start of packet is available
    notAvailable    /// Status of start of packet is not available
    };

  /*
   * @brief  Validates the start of packet
   * @param  Input byte received
   * @return Status of packet status on new byte input
   */
  startOfPacketStatus validateStartOfPacket(const uint8_t newByte);

  /* Frame Buffer stores bytes of data received from PacketParser input */
  std::array<uint8_t, frameSize> frameBuffer;
  //uint8_t frameBuffer[frameSize];

  /* Frame Buffer length read from UART buffer */
  uint8_t bufferLength;

  /* Frame Buffer index of a packet*/
  uint8_t frameBufferIndex;

  /* Frame input status */
  frameInputStatus inputStatus;

  /* Start of packet status */
  startOfPacketStatus packetStatus = startOfPacketStatus::notAvailable;

};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
