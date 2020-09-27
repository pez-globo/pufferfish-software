/*
 * Messages.tpp
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Messages.h"
#include "nanopb/pb_decode.h"
#include "nanopb/pb_encode.h"

namespace Pufferfish::Protocols {

// Message

template <typename UnionMessage, size_t max_size>
template <size_t output_size, size_t num_descriptors>
typename Message<UnionMessage, max_size>::Status Message<UnionMessage, max_size>::write(
    Util::ByteVector<output_size> &output_buffer,
    const Util::ProtobufDescriptors<num_descriptors> &pb_protobuf_descriptors) const {
  if (type > pb_protobuf_descriptors.size()) {
    return Status::type_error;
  }

  const pb_msgdesc_t *fields = pb_protobuf_descriptors[type];
  if (fields == Util::get_protobuf_descriptor<Util::UnrecognizedMessage>()) {
    return Status::type_error;
  }

  size_t encoded_size = 0;
  if (!pb_get_encoded_size(&encoded_size, fields, &payload)) {
    return Status::encoding_error;
  }

  if (output_buffer.resize(header_size + encoded_size) != IndexStatus::ok) {
    return Status::length_error;
  }

  output_buffer[type_offset] = type;
  pb_ostream_t stream = pb_ostream_from_buffer(
      output_buffer.buffer() + header_size, output_buffer.size() - header_size);
  if (!pb_encode(&stream, fields, &payload)) {
    return Status::encoding_error;
  }

  return Status::ok;
}

template <typename UnionMessage, size_t max_size>
template <size_t input_size, size_t num_descriptors>
typename Message<UnionMessage, max_size>::Status Message<UnionMessage, max_size>::parse(
    const Util::ByteVector<input_size> &input_buffer,
    const Util::ProtobufDescriptors<num_descriptors> &pb_protobuf_descriptors) {
  if (input_buffer.size() < Message::header_size) {
    return Status::length_error;
  }

  type = input_buffer[Message::type_offset];
  if (type > pb_protobuf_descriptors.size()) {
    return Status::type_error;
  }

  const pb_msgdesc_t *fields = pb_protobuf_descriptors[type];
  if (fields == Util::get_protobuf_descriptor<Util::UnrecognizedMessage>()) {
    return Status::type_error;
  }

  pb_istream_t stream =
      pb_istream_from_buffer(input_buffer.buffer() + header_size, input_buffer.size() - header_size);
  if (!pb_decode(&stream, fields, &payload)) {
    return Status::decoding_error;
  }

  return Status::ok;
}

// MessageReceiver

template <typename Message, size_t num_descriptors>
MessageReceiver<Message, num_descriptors>::MessageReceiver(
    const Util::ProtobufDescriptors<num_descriptors> &descriptors)
    : descriptors_(descriptors) {}

template <typename Message, size_t num_descriptors>
template <size_t input_size>
typename MessageReceiver<Message, num_descriptors>::Status
MessageReceiver<Message, num_descriptors>::transform(
    const Util::ByteVector<input_size> &input_buffer, Message &output_message) const {
  using MessageStatus = typename Message::Status;
  switch (output_message.parse(input_buffer, descriptors_)) {
    case MessageStatus::length_error:
      return Status::invalid_length;
    case MessageStatus::type_error:
      return Status::invalid_type;
    case MessageStatus::encoding_error:
    case MessageStatus::decoding_error:
      return Status::invalid_encoding;
    case MessageStatus::ok:
      return Status::ok;
    default:
      return Status::ok;
  }
}

// MessageSender

template <typename Message, size_t num_descriptors>
MessageSender<Message, num_descriptors>::MessageSender(
    const Util::ProtobufDescriptors<num_descriptors> &descriptors)
    : descriptors_(descriptors) {}

template <typename Message, size_t num_descriptors>
template <size_t output_size>
typename MessageSender<Message, num_descriptors>::Status
MessageSender<Message, num_descriptors>::transform(
    const Message &input_message, Util::ByteVector<output_size> &output_buffer) const {
  using MessageStatus = typename Message::Status;
  switch (input_message.write(output_buffer, descriptors_)) {
    case MessageStatus::length_error:
      return Status::invalid_length;
    case MessageStatus::type_error:
      return Status::invalid_type;
    case MessageStatus::encoding_error:
    case MessageStatus::decoding_error:
      return Status::invalid_encoding;
    case MessageStatus::ok:
      return Status::ok;
    default:
      return Status::ok;
  }
}

}  // namespace Pufferfish::Protocols
