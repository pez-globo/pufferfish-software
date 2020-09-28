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

// ChunkSplitter

template <size_t buffer_size>
FrameProps::InputStatus ChunkSplitter<buffer_size>::input(uint8_t new_byte) {
  if (new_byte == delimiter) {
    input_status_ = FrameProps::InputStatus::output_ready;
    return input_status_;
  }

  if (buffer_.push_back(new_byte) != IndexStatus::ok) {
    input_status_ = FrameProps::InputStatus::invalid_length;
    return input_status_;
  }

  return FrameProps::InputStatus::ok;
}

template <size_t buffer_size>
FrameProps::OutputStatus ChunkSplitter<buffer_size>::output(
    Util::ByteVector<buffer_size> &output_buffer) {
  if (input_status_ == FrameProps::InputStatus::ok) {
    return FrameProps::OutputStatus::waiting;
  }

  output_buffer.copy_from(buffer_);
  buffer_.clear();
  FrameProps::OutputStatus output_status = FrameProps::OutputStatus::ok;
  if (input_status_ == FrameProps::InputStatus::invalid_length) {
    output_status = FrameProps::OutputStatus::invalid_length;
  }
  input_status_ = FrameProps::InputStatus::ok;
  return output_status;
}

// ChunkMerger

template <size_t buffer_size>
FrameProps::OutputStatus ChunkMerger::transform(
    Util::ByteVector<buffer_size> &input_output_buffer) const {
  if (input_output_buffer.push_back(delimiter) == IndexStatus::ok) {
    return FrameProps::OutputStatus::ok;
  }
  return FrameProps::OutputStatus::invalid_length;
}

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
