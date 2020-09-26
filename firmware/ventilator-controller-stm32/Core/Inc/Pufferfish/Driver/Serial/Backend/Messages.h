/*
 * Messages.h
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_MESSAGES_H_
#define INC_PUFFERFISH_PROTOCOLS_MESSAGES_H_

#include <stdint.h>
#include <stddef.h>

#include "Pufferfish/Util/ByteArray.h"
#include "Datagrams.h"
#include "nanopb/pb.h"

namespace Pufferfish { namespace Protocols {

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

  template<size_t OutputSize>
  Status write(
      Util::ByteArray<OutputSize> &outputBuffer,
      const pb_msgdesc_t **pbMessageDescriptors,
      size_t numPBMessageDescriptors
  ) const;

  template<size_t InputSize>
  Status parse(
      const Util::ByteArray<InputSize> &inputBuffer,
      const pb_msgdesc_t **pbMessageDescriptors,
      size_t numPBMessageDescriptors
  ); // updates type and payload fields
};

// Parses messages into payloads, with data integrity checking
template<class UnionMessage>
class MessageReceiver {
public:
  enum class Status {
    ok = 0,
    invalidLength, invalidType, invalidEncoding
  };

  MessageReceiver(const pb_msgdesc_t **descriptors, size_t numDescriptors);

  template<size_t InputSize>
  Status transform(
      const Util::ByteArray<InputSize> &inputBuffer,
      Message<UnionMessage> &outputMessage
  ) const;

protected:
  const pb_msgdesc_t **descriptors;
  size_t numDescriptors;
};

// Generates messages from payloads
template<class UnionMessage>
class MessageSender {
public:
  enum class Status {ok = 0, invalidLength, invalidType, invalidEncoding};

  MessageSender(const pb_msgdesc_t **descriptors, size_t numDescriptors);

  template<size_t OutputSize>
  Status transform(
      const Message<UnionMessage> &inputMessage,
      Util::ByteArray<OutputSize> &outputBuffer
  ) const;

protected:
  const pb_msgdesc_t **descriptors;
  size_t numDescriptors;
};

} }

#include "Messages.tpp"

#endif /* INC_PUFFERFISH_PROTOCOLS_MESSAGES_H_ */
