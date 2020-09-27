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

template<size_t size>
using ProtobufDescriptors = std::array<ProtobufDescriptor, size>;

using UnrecognizedMessage = std::nullptr_t;

template<class MessageType>
constexpr ProtobufDescriptor getProtobufDescriptor() {
  return nanopb::MessageDescriptor<MessageType>::fields();
}

template<>
constexpr ProtobufDescriptor getProtobufDescriptor<UnrecognizedMessage>() {
  return nullptr;
}

}
