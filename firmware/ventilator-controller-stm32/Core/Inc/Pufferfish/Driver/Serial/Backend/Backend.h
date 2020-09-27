/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <stdint.h>
#include "Pufferfish/Util/ByteArray.h"
#include "Pufferfish/HAL/CRC.h"
#include "Frames.h"
#include "Datagrams.h"
#include "Messages.h"
#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::Serial::Backend {

class BackendReceiver {
public:
  enum class InputStatus {
    inputReady = 0, outputReady, invalidFrameChunkLength
  };
  enum class OutputStatus {
    available = 0, waiting,
    invalidFrameChunkLength, invalidFrameCOBSLength,
    invalidDatagramParse, invalidDatagramCRC, invalidDatagramLength,
    invalidDatagramSequence,
    invalidMessageLength, invalidMessageType, invalidMessageEncoding
  };

  BackendReceiver(HAL::CRC32C &crc32c);

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t newByte);
  OutputStatus output(Application::Message &outputMessage);

protected:
  FrameReceiver frame;
  DatagramReceiver datagram;
  Application::MessageReceiver message;
};

class BackendSender {
public:
  enum class Status {
    ok = 0,
    invalidMessageLength, invalidMessageType, invalidMessageEncoding,
    invalidDatagramLength,
    invalidFrameCOBSLength, invalidFrameChunkLength
  };

  BackendSender(HAL::CRC32C &crc32c);

  Status transform(
      const Application::Message &inputMessage, ChunkBuffer &outputBuffer
  );

protected:
  Application::MessageSender message;
  DatagramSender datagram;
  FrameSender frame;
};

}

#include "Backend.tpp"
