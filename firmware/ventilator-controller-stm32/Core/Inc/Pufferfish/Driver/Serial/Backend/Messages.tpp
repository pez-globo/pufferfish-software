/*
 * Messages.tpp
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "nanopb/pb_decode.h"
#include "nanopb/pb_encode.h"
#include "Messages.h"

namespace Pufferfish::Driver::Serial::Backend {

// Message

template<class UnionMessage>
template<size_t OutputSize, size_t NumDescriptors>
typename Message<UnionMessage>::Status
Message<UnionMessage>::write(
    Util::ByteArray<OutputSize> &outputBuffer,
    const Util::ProtobufDescriptors<NumDescriptors> &pbProtobufDescriptors
) const {
  if (type > pbProtobufDescriptors.size()) {
    return Status::typeError;
  }

  const pb_msgdesc_t *fields = pbProtobufDescriptors[type];
  if (fields == Util::getProtobufDescriptor<Util::UnrecognizedMessage>()) {
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
template<size_t InputSize, size_t NumDescriptors>
typename Message<UnionMessage>::Status
Message<UnionMessage>::parse(
    const Util::ByteArray<InputSize> &inputBuffer,
    const Util::ProtobufDescriptors<NumDescriptors> &pbProtobufDescriptors
) {
  if (inputBuffer.size() < Message::headerSize) {
    return Status::lengthError;
  }

  type = inputBuffer.buffer[Message::typeOffset];
  if (type > pbProtobufDescriptors.size()) {
    return Status::typeError;
  }

  const pb_msgdesc_t *fields = pbProtobufDescriptors[type];
  if (fields == Util::getProtobufDescriptor<Util::UnrecognizedMessage>()) {
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

template<class UnionMessage, size_t NumDescriptors>
MessageReceiver<UnionMessage, NumDescriptors>::MessageReceiver(
    const Util::ProtobufDescriptors<NumDescriptors> &descriptors
) : descriptors(descriptors) {}

template<class UnionMessage, size_t NumDescriptors>
template<size_t InputSize>
typename MessageReceiver<UnionMessage, NumDescriptors>::Status
MessageReceiver<UnionMessage, NumDescriptors>::transform(
    const Util::ByteArray<InputSize> &inputBuffer,
    Message<UnionMessage> &outputMessage
) const {
  using MessageStatus = typename Message<UnionMessage>::Status;
  switch (outputMessage.parse(inputBuffer, descriptors)) {
  case MessageStatus::lengthError:
    return Status::invalidLength;
  case MessageStatus::typeError:
    return Status::invalidType;
  case MessageStatus::encodingError:
  case MessageStatus::decodingError:
    return Status::invalidEncoding;
  case MessageStatus::ok:
  return Status::ok;
  default:
    return Status::ok;
  }
}

// MessageSender

template<class UnionMessage, size_t NumDescriptors>
MessageSender<UnionMessage, NumDescriptors>::MessageSender(
    const Util::ProtobufDescriptors<NumDescriptors> &descriptors
) : descriptors(descriptors) {}

template<class UnionMessage, size_t NumDescriptors>
template<size_t OutputSize>
typename MessageSender<UnionMessage, NumDescriptors>::Status
MessageSender<UnionMessage, NumDescriptors>::transform(
    const Message<UnionMessage> &inputMessage,
    Util::ByteArray<OutputSize> &outputBuffer
) const {
  using MessageStatus = typename Message<UnionMessage>::Status;
  switch (inputMessage.write(outputBuffer, descriptors)) {
  case MessageStatus::lengthError:
    return Status::invalidLength;
  case MessageStatus::typeError:
    return Status::invalidType;
  case MessageStatus::encodingError:
  case MessageStatus::decodingError:
    return Status::invalidEncoding;
  case MessageStatus::ok:
    return Status::ok;
  default:
    return Status::ok;
  }
}

}
