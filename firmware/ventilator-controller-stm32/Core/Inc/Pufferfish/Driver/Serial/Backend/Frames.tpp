/*
 * Frames.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Frames.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/COBS.h"

namespace Pufferfish::Driver::Serial::Backend {

// COBSDecoder

template <size_t input_size, size_t output_size>
IndexStatus COBSDecoder::transform(
    const Util::ByteVector<input_size> &input_buffer,
    Util::ByteVector<output_size> &output_buffer) const {
  static_assert(
      Util::ByteVector<input_size>::max_size() <= FrameProps::encoded_max_size,
      "COBSDecoder unavailable as the input buffer size is too large");
  static_assert(
      Util::ByteVector<output_size>::max_size() <= FrameProps::payload_max_size,
      "COBSDecoder unavailable as the output buffer size is too large");

  if (Util::decode_cobs(input_buffer, output_buffer) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }
  return IndexStatus::ok;
}

// COBSEncoder

template <size_t input_size, size_t output_size>
IndexStatus COBSEncoder::transform(
    const Util::ByteVector<input_size> &input_buffer,
    Util::ByteVector<output_size> &output_buffer) const {
  static_assert(
      Util::ByteVector<input_size>::max_size() <= FrameProps::encoded_max_size,
      "COBSDecoder unavailable as the input buffer size is too large");

  size_t encoded_size = Util::get_encoded_cobs_buffer_size(input_buffer.size());
  if (output_buffer.max_size() < encoded_size) {
    return IndexStatus::out_of_bounds;
  }

  if (Util::encode_cobs(input_buffer, output_buffer) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }
  return IndexStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
