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
typename ChunkSplitter<buffer_size>::InputStatus ChunkSplitter<buffer_size>::input(
    uint8_t new_byte) {
  if (new_byte == delimiter) {
    input_status_ = InputStatus::output_ready;
    return input_status_;
  }

  if (buffer_.push_back(new_byte) != IndexStatus::ok) {
    input_status_ = InputStatus::invalid_length;
    return input_status_;
  }

  return InputStatus::input_ready;
}

template <size_t buffer_size>
typename ChunkSplitter<buffer_size>::OutputStatus ChunkSplitter<buffer_size>::output(
    Util::ByteArray<buffer_size> &output_buffer) {
  if (input_status_ == InputStatus::input_ready) {
    return OutputStatus::waiting;
  }

  output_buffer.copy_from(buffer_);
  buffer_.clear();
  OutputStatus output_status = OutputStatus::available;
  if (input_status_ == InputStatus::invalid_length) {
    output_status = OutputStatus::invalid_length;
  }
  input_status_ = InputStatus::input_ready;
  return output_status;
}

// ChunkMerger

template <size_t buffer_size>
ChunkMerger::Status ChunkMerger::transform(
    Util::ByteArray<buffer_size> &input_output_buffer) const {
  if (input_output_buffer.push_back(delimiter) == IndexStatus::ok) {
    return Status::ok;
  }
  return Status::invalid_length;
}

// COBSDecoder

template <size_t input_size, size_t output_size>
COBSDecoder::Status COBSDecoder::transform(
    const Util::ByteArray<input_size> &input_buffer,
    Util::ByteArray<output_size> &output_buffer) const {
  if (input_buffer.size() > frame_payload_max_size + 1) {
    return Status::invalid_length;
  }

  if (output_buffer.max_size < input_buffer.size()) {
    return Status::invalid_length;
  }

  output_buffer.resize(
      Util::decode_cobs(input_buffer.buffer, input_buffer.size(), output_buffer.buffer));
  return Status::ok;
}

// COBSEncoder

template <size_t input_size, size_t output_size>
COBSEncoder::Status COBSEncoder::transform(
    const Util::ByteArray<input_size> &input_buffer,
    Util::ByteArray<output_size> &output_buffer) const {
  if (input_buffer.size() > frame_payload_max_size) {
    return Status::invalid_length;
  }

  size_t encoded_size = Util::get_encoded_cobs_buffer_size(input_buffer.size());
  if (output_buffer.max_size < encoded_size) {
    return Status::invalid_length;
  }

  output_buffer.resize(
      Util::encode_cobs(input_buffer.buffer, input_buffer.size(), output_buffer.buffer));
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
