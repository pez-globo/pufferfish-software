/*
 * Messages.tpp
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_MESSAGES_TPP_
#define INC_PUFFERFISH_PROTOCOLS_MESSAGES_TPP_

#include "nanopb/pb_decode.h"
#include "nanopb/pb_encode.h"

#include "Messages.h"

namespace Pufferfish { namespace Protocols {

// Message

template<class UnionMessage>
template<size_t OutputSize>
typename Message<UnionMessage>::Status
Message<UnionMessage>::write(
    Util::ByteArray<OutputSize> &outputBuffer,
    const pb_msgdesc_t *pbMessageDescriptors[],
    size_t numPBMessageDescriptors
) const {
  if (pbMessageDescriptors == nullptr || type > numPBMessageDescriptors) {
    return Status::typeError;
  }

  const pb_msgdesc_t *fields = pbMessageDescriptors[type];
  if (fields == nullptr) {
    return Status::typeError;
  }

  size_t encodedSize;
  if (!pb_get_encoded_size(&encodedSize, fields, &payload)) {
    return Status::encodingError;
  }

  if (outputBuffer.resize(headerSize + encodedSize) != IndexStatus::ok) {
    return Status::lengthError;
  }

  outputBuffer.buffer[typeOffset] = type;
  pb_ostream_t stream = pb_ostream_from_buffer(
      outputBuffer.buffer + headerSize, outputBuffer.size() - headerSize
  );
  if (!pb_encode(&stream, fields, &payload)) {
    return Status::encodingError;
  }

  return Status::ok;
}

template<class UnionMessage>
template<size_t InputSize>
typename Message<UnionMessage>::Status
Message<UnionMessage>::parse(
    const Util::ByteArray<InputSize> &inputBuffer,
    const pb_msgdesc_t *pbMessageDescriptors[],
    size_t numPBMessageDescriptors
) {
  if (inputBuffer.size() < Message::headerSize) {
    return Status::lengthError;
  }

  type = inputBuffer.buffer[Message::typeOffset];
  if (pbMessageDescriptors == nullptr || type > numPBMessageDescriptors) {
    return Status::typeError;
  }

  const pb_msgdesc_t *fields = pbMessageDescriptors[type];
  if (fields == nullptr) {
    return Status::typeError;
  }

  pb_istream_t stream = pb_istream_from_buffer(
      inputBuffer.buffer + headerSize, inputBuffer.size() - headerSize
  );
  if (!pb_decode(&stream, fields, &payload)) {
    return Status::decodingError;
  }

  return Status::ok;
}

// MessageReceiver

template<class UnionMessage>
MessageReceiver<UnionMessage>::MessageReceiver(
    const pb_msgdesc_t **descriptors, size_t numDescriptors
) : descriptors(descriptors), numDescriptors(numDescriptors) {}

template<class UnionMessage>
template<size_t InputSize>
typename MessageReceiver<UnionMessage>::Status
MessageReceiver<UnionMessage>::transform(
    const Util::ByteArray<InputSize> &inputBuffer,
    Message<UnionMessage> &outputMessage
) const {
  switch (outputMessage.parse(inputBuffer, descriptors, numDescriptors)) {
  case Message<UnionMessage>::Status::lengthError:
    return Status::invalidLength;
  case Message<UnionMessage>::Status::typeError:
    return Status::invalidType;
  case Message<UnionMessage>::Status::encodingError:
  case Message<UnionMessage>::Status::decodingError:
    return Status::invalidEncoding;
  case Message<UnionMessage>::Status::ok:
    break;
  }
  return Status::ok;
}

// MessageSender

template<class UnionMessage>
MessageSender<UnionMessage>::MessageSender(
    const pb_msgdesc_t **descriptors, size_t numDescriptors
) : descriptors(descriptors), numDescriptors(numDescriptors) {}

template<class UnionMessage>
template<size_t OutputSize>
typename MessageSender<UnionMessage>::Status
MessageSender<UnionMessage>::transform(
    const Message<UnionMessage> &inputMessage,
    Util::ByteArray<OutputSize> &outputBuffer
) const {
  switch (inputMessage.write(outputBuffer, descriptors, numDescriptors)) {
  case Message<UnionMessage>::Status::lengthError:
    return Status::invalidLength;
  case Message<UnionMessage>::Status::typeError:
    return Status::invalidType;
  case Message<UnionMessage>::Status::encodingError:
  case Message<UnionMessage>::Status::decodingError:
    return Status::invalidEncoding;
  case Message<UnionMessage>::Status::ok:
    break;
  }
  return Status::ok;
}

} }

#endif /* INC_PUFFERFISH_PROTOCOLS_MESSAGES_TPP_ */
