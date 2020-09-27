/*
 * Frames.cpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Backend/Frames.h"

namespace Pufferfish::Driver::Serial::Backend {

// FrameReceiver

FrameReceiver::InputStatus FrameReceiver::input(uint8_t new_byte) {
  switch (chunk_splitter_.input(new_byte)) {
    case ChunkSplitter<chunk_max_size>::InputStatus::input_ready:
      return InputStatus::input_ready;
    case ChunkSplitter<chunk_max_size>::InputStatus::invalid_length:
      return InputStatus::invalid_chunk_length;
    case ChunkSplitter<chunk_max_size>::InputStatus::output_ready:
      break;
  }
  return InputStatus::output_ready;
}

FrameReceiver::OutputStatus FrameReceiver::output(ChunkBuffer &output_buffer) {
  ChunkBuffer temp_buffer;

  // Chunk
  switch (chunk_splitter_.output(temp_buffer)) {
    case ChunkSplitter<chunk_max_size>::OutputStatus::waiting:
      return OutputStatus::waiting;
    case ChunkSplitter<chunk_max_size>::OutputStatus::invalid_length:
      return OutputStatus::invalid_chunk_length;
    case ChunkSplitter<chunk_max_size>::OutputStatus::available:
      break;
  }

  // COBS
  switch (cobs_decoder.transform(temp_buffer, output_buffer)) {
    case COBSDecoder::Status::invalid_length:
      return OutputStatus::invalid_cobs_length;
    case COBSDecoder::Status::ok:
      break;
  }
  return OutputStatus::available;
}

// FrameSender

FrameSender::Status FrameSender::transform(
    const ChunkBuffer &input_buffer, ChunkBuffer &output_buffer) const {
  // COBS
  switch (cobs_encoder.transform(input_buffer, output_buffer)) {
    case COBSEncoder::Status::invalid_length:
      return Status::invalid_cobs_length;
    case COBSEncoder::Status::ok:
      break;
  }

  // Chunk
  switch (chunk_merger.transform(output_buffer)) {
    case ChunkMerger::Status::invalid_length:
      return Status::invalid_chunk_length;
    case ChunkMerger::Status::ok:
      break;
  }
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
