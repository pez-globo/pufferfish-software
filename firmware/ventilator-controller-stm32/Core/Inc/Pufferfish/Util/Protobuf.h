/*
 * Protobuf.h
 *
 *  Created on: May 22, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <array>
#include <cstddef>

#include "nanopb/pb_common.h"

namespace Pufferfish::Util {

using ProtobufDescriptor = const pb_msgdesc_t *;

template <size_t size>
using ProtobufDescriptors = std::array<ProtobufDescriptor, size>;

using UnrecognizedMessage = std::nullptr_t;

template <typename MessageType>
constexpr ProtobufDescriptor get_protobuf_descriptor() {
  return nanopb::MessageDescriptor<MessageType>::fields();
}

template <>
constexpr ProtobufDescriptor get_protobuf_descriptor<UnrecognizedMessage>() {
  return nullptr;
}

}  // namespace Pufferfish::Util
