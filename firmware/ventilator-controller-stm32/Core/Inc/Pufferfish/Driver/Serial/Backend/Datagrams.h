/*
 * Datagrams.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_DATAGRAMS_H_
#define INC_PUFFERFISH_PROTOCOLS_DATAGRAMS_H_

#include <stdint.h>
#include <stddef.h>
#include "Pufferfish/Util/ByteArray.h"
#include "Pufferfish/HAL/CRC.h"
#include "Frames.h"

namespace Pufferfish { namespace Protocols {

class Datagram {
public:
  static const size_t protectedOffset = sizeof(uint32_t);
  static const size_t seqOffset = protectedOffset;
  static const size_t lengthOffset = seqOffset + sizeof(uint8_t);
  static const size_t payloadOffset = lengthOffset + sizeof(uint8_t);

  static const size_t headerSize = payloadOffset;
  static const size_t payloadMaxSize = framePayloadMaxSize - headerSize;
  using PayloadBuffer = Util::ByteArray<payloadMaxSize>;

  Datagram(PayloadBuffer &payload);

  uint32_t crc = 0;
  uint8_t seq = 0;
  uint8_t length = 0;
  PayloadBuffer &payload;

  template<size_t OutputSize>
  IndexStatus write(
      Util::ByteArray<OutputSize> &outputBuffer, HAL::CRC32C &crc32c
  ); // updates length and crc fields

  template<size_t InputSize>
  IndexStatus parse(
      const Util::ByteArray<InputSize> &inputBuffer
  ); // updates all fields, including payload

protected:
  template<size_t OutputSize>
  IndexStatus writeProtected(Util::ByteArray<OutputSize> &outputBuffer) const;
};

// Parses datagrams into payloads, with data integrity checking
class DatagramReceiver {
public:
  enum class Status {
    ok = 0,
    invalidParse, invalidCRC, invalidLength,
    invalidSequence
  };

  DatagramReceiver(HAL::CRC32C &crc32c);

  template<size_t InputSize>
  Status transform(
      const Util::ByteArray<InputSize> &inputBuffer, Datagram &outputDatagram
  );

protected:
  uint8_t expectedSeq = 0;
  HAL::CRC32C crc32c;

  template<size_t InputSize>
  uint32_t computeCRC(const Util::ByteArray<InputSize> &inputBuffer);
};

// Generates datagrams from payloads
class DatagramSender {
public:
  enum class Status {ok = 0, invalidLength};

  DatagramSender(HAL::CRC32C &crc32c);

  template<size_t OutputSize>
  Status transform(
      const Datagram::PayloadBuffer &inputPayload,
      Util::ByteArray<OutputSize> &outputBuffer
  );

protected:
  uint8_t nextSeq = 0;
  HAL::CRC32C crc32c;
};

} }

#include "Datagrams.tpp"

#endif /* INC_PUFFERFISH_PROTOCOLS_DATAGRAMS_H_ */
