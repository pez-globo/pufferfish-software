/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_SERIAL_H_
#define INC_PUFFERFISH_PROTOCOLS_SERIAL_H_

#include <stdint.h>
#include "Pufferfish/Util/ByteArray.h"
#include "Pufferfish/HAL/CRC.h"
#include "Frames.h"
#include "Datagrams.h"
#include "Messages.h"
#include "Application.h"

namespace Pufferfish { namespace Protocols {

class SerialReceiver {
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

  SerialReceiver(HAL::CRC32C &crc32c);

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t newByte);
  OutputStatus output(ApplicationMessage &outputMessage);

protected:
  FrameReceiver frame;
  DatagramReceiver datagram;
  ApplicationMessageReceiver message;
};

class SerialSender {
public:
  enum class Status {
    ok = 0,
    invalidMessageLength, invalidMessageType, invalidMessageEncoding,
    invalidDatagramLength,
    invalidFrameCOBSLength, invalidFrameChunkLength
  };

  SerialSender(HAL::CRC32C &crc32c);

  Status transform(
      const ApplicationMessage &inputMessage, ChunkBuffer &outputBuffer
  );

protected:
  DatagramSender datagram;
  FrameSender frame;
  ApplicationMessageSender message;
};

} }

#include "Serial.tpp"

#endif /* INC_PUFFERFISH_PROTOCOLS_SERIAL_H_ */
