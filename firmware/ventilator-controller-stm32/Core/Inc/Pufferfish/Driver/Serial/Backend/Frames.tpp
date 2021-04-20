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
      Util::ByteVector<output_size>::max_size() >= FrameProps::payload_max_size,
      "COBSDecoder unavailable as the output buffer size is too small");

  return Util::decode_cobs(input_buffer, output_buffer);
}

// COBSEncoder

template <size_t input_size, size_t output_size>
IndexStatus COBSEncoder::transform(
    const Util::ByteVector<input_size> &input_buffer,
    Util::ByteVector<output_size> &output_buffer) const {
  static_assert(
      Util::ByteVector<input_size>::max_size() <= FrameProps::payload_max_size,
      "COBSEncoder unavailable as the input buffer size is too large");
  static_assert(
      Util::ByteVector<output_size>::max_size() >= FrameProps::encoded_max_size,
      "COBSEncoder unavailable as the output buffer size is too small");

  return Util::encode_cobs(input_buffer, output_buffer);
}

}  // namespace Pufferfish::Driver::Serial::Backend
