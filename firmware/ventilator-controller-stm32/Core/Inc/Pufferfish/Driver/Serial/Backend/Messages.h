/*
 * Messages.h
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Util/ByteArray.h"
#include "Pufferfish/Util/Protobuf.h"
#include "Datagrams.h"
#include "nanopb/pb_common.h"

namespace Pufferfish::Driver::Serial::Backend {

// Messages

template<class UnionMessage>
class Message {
public:
  static const size_t typeOffset = 0;
  static const size_t payloadOffset = typeOffset + sizeof(uint8_t);

  static const size_t headerSize = payloadOffset;
  static const size_t payloadMaxSize = Datagram::payloadMaxSize - headerSize;

  enum class Status {
      ok = 0, lengthError, typeError, encodingError, decodingError
  };

  uint8_t type = 0;
  UnionMessage payload;

  template<size_t OutputSize, size_t NumDescriptors>
  Status write(
      Util::ByteArray<OutputSize> &outputBuffer,
      const Util::ProtobufDescriptors<NumDescriptors> &pbProtobufDescriptors
  ) const;

  template<size_t InputSize, size_t NumDescriptors>
  Status parse(
      const Util::ByteArray<InputSize> &inputBuffer,
      const Util::ProtobufDescriptors<NumDescriptors> &pbProtobufDescriptors
  ); // updates type and payload fields
};

// Parses messages into payloads, with data integrity checking
template<class UnionMessage, size_t NumDescriptors>
class MessageReceiver {
public:
  enum class Status {
    ok = 0,
    invalidLength, invalidType, invalidEncoding
  };

  MessageReceiver(const Util::ProtobufDescriptors<NumDescriptors> &descriptors);

  template<size_t InputSize>
  Status transform(
      const Util::ByteArray<InputSize> &inputBuffer,
      Message<UnionMessage> &outputMessage
  ) const;

protected:
  const Util::ProtobufDescriptors<NumDescriptors> &descriptors;
};

// Generates messages from payloads
template<class UnionMessage, size_t NumDescriptors>
class MessageSender {
public:
  enum class Status {ok = 0, invalidLength, invalidType, invalidEncoding};

  MessageSender(const Util::ProtobufDescriptors<NumDescriptors> &descriptors);

  template<size_t OutputSize>
  Status transform(
      const Message<UnionMessage> &inputMessage,
      Util::ByteArray<OutputSize> &outputBuffer
  ) const;

protected:
  const Util::ProtobufDescriptors<NumDescriptors> &descriptors;
};

}

#include "Messages.tpp"
