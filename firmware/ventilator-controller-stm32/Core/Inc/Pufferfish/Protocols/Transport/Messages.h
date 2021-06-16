/*
 * Messages.h
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Util/Containers/EnumMap.h"
#include "Pufferfish/Util/Containers/Vector.h"
#include "Pufferfish/Util/Protobuf.h"
#include "nanopb/pb_common.h"

namespace Pufferfish::Protocols::Transport {

enum class MessageStatus { ok = 0, invalid_length, invalid_type, invalid_encoding };

// Messages

template <typename EnumKey, size_t max_key>
using ProtobufDescriptors = Util::Containers::EnumMap<EnumKey, Util::ProtobufDescriptor, max_key>;

template <typename TaggedUnion, typename MessageTypes, size_t max_size>
class Message {
 public:
  template <size_t max_key>
  using ProtobufDescriptors = ProtobufDescriptors<typename MessageTypes::EnumType, max_key>;

  static const size_t type_offset = 0;
  // This clearly is static initialization of a static const - probably clang-tidy
  // false positive?
  // NOLINTNEXTLINE(bugprone-dynamic-static-initializers)
  static const size_t payload_offset = type_offset + sizeof(uint8_t);

  // This clearly is static initialization of a static const - probably clang-tidy
  // false positive?
  // NOLINTNEXTLINE(bugprone-dynamic-static-initializers)
  static const size_t header_size = payload_offset;
  // NOLINTNEXTLINE(bugprone-dynamic-static-initializers)
  static const size_t payload_max_size = max_size - header_size;

  uint8_t type = 0;
  TaggedUnion payload{};

  template <size_t output_size, size_t max_key>
  MessageStatus write(
      Util::Containers::ByteVector<output_size> &output_buffer,
      const ProtobufDescriptors<max_key> &pb_protobuf_descriptors);

  template <size_t input_size, size_t max_key>
  MessageStatus parse(
      const Util::Containers::ByteVector<input_size> &input_buffer,
      const ProtobufDescriptors<max_key>
          &pb_protobuf_descriptors);  // updates type and payload fields
};

// Parses messages into payloads, with data integrity checking
template <typename Message, size_t max_key>
class MessageReceiver {
 public:
  using ProtobufDescriptors = typename Message::template ProtobufDescriptors<max_key>;

  explicit MessageReceiver(const ProtobufDescriptors &descriptors) : descriptors_(descriptors) {}

  template <size_t input_size>
  MessageStatus transform(
      const Util::Containers::ByteVector<input_size> &input_buffer, Message &output_message) const;

 private:
  const ProtobufDescriptors &descriptors_;
};

// Generates messages from payloads
template <typename Message, typename TaggedUnion, size_t max_key>
class MessageSender {
 public:
  using ProtobufDescriptors = typename Message::template ProtobufDescriptors<max_key>;

  explicit MessageSender(const ProtobufDescriptors &descriptors) : descriptors_(descriptors) {}

  template <size_t output_size>
  MessageStatus transform(
      const TaggedUnion &payload, Util::Containers::ByteVector<output_size> &output_buffer) const;

 private:
  const ProtobufDescriptors &descriptors_;
};

}  // namespace Pufferfish::Protocols::Transport

#include "Messages.tpp"
