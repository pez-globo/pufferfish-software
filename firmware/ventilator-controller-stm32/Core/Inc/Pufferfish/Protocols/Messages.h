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
#include "nanopb/pb_common.h"

namespace Pufferfish::Protocols {

// Messages

template <typename UnionMessage, size_t max_size>
class Message {
public:
 static const size_t type_offset = 0;
 static const size_t payload_offset = type_offset + sizeof(uint8_t);

 static const size_t header_size = payload_offset;
 static const size_t payload_max_size = max_size - header_size;

 enum class Status { ok = 0, length_error, type_error, encoding_error, decoding_error };

 uint8_t type = 0;
 UnionMessage payload{};

 template <size_t output_size, size_t num_descriptors>
 Status write(
     Util::ByteArray<output_size> &output_buffer,
     const Util::ProtobufDescriptors<num_descriptors> &pb_protobuf_descriptors) const;

 template <size_t input_size, size_t num_descriptors>
 Status parse(
     const Util::ByteArray<input_size> &input_buffer,
     const Util::ProtobufDescriptors<num_descriptors>
         &pb_protobuf_descriptors);  // updates type and payload fields
};

// Parses messages into payloads, with data integrity checking
template <typename Message, size_t num_descriptors>
class MessageReceiver {
 public:
  enum class Status { ok = 0, invalid_length, invalid_type, invalid_encoding };

  explicit MessageReceiver(const Util::ProtobufDescriptors<num_descriptors> &descriptors);

  template <size_t input_size>
  Status transform(const Util::ByteArray<input_size> &input_buffer, Message &output_message) const;

 private:
  const Util::ProtobufDescriptors<num_descriptors> &descriptors_;
};

// Generates messages from payloads
template <typename Message, size_t num_descriptors>
class MessageSender {
 public:
  enum class Status { ok = 0, invalid_length, invalid_type, invalid_encoding };

  explicit MessageSender(const Util::ProtobufDescriptors<num_descriptors> &descriptors);

  template <size_t output_size>
  Status transform(const Message &input_message, Util::ByteArray<output_size> &output_buffer) const;

 private:
  const Util::ProtobufDescriptors<num_descriptors> &descriptors_;
};

}  // namespace Pufferfish::Protocols

#include "Messages.tpp"
