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
FrameProps::OutputStatus COBSDecoder::transform(
    const Util::ByteVector<input_size> &input_buffer,
    Util::ByteVector<output_size> &output_buffer) const {
  if (input_buffer.size() > FrameProps::payload_max_size + 1) {
    return FrameProps::OutputStatus::invalid_length;
  }

  if (output_buffer.max_size() < input_buffer.size()) {
    return FrameProps::OutputStatus::invalid_length;
  }

  output_buffer.resize(
      Util::decode_cobs(input_buffer.buffer(), input_buffer.size(), output_buffer.buffer()));
  return FrameProps::OutputStatus::ok;
}

// COBSEncoder

template <size_t input_size, size_t output_size>
FrameProps::OutputStatus COBSEncoder::transform(
    const Util::ByteVector<input_size> &input_buffer,
    Util::ByteVector<output_size> &output_buffer) const {
  if (input_buffer.size() > FrameProps::payload_max_size) {
    return FrameProps::OutputStatus::invalid_length;
  }

  size_t encoded_size = Util::get_encoded_cobs_buffer_size(input_buffer.size());
  if (output_buffer.max_size() < encoded_size) {
    return FrameProps::OutputStatus::invalid_length;
  }

  output_buffer.resize(
      Util::encode_cobs(input_buffer.buffer(), input_buffer.size(), output_buffer.buffer()));
  return FrameProps::OutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
