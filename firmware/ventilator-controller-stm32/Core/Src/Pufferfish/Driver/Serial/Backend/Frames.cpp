/*
 * Frames.cpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Backend/Frames.h"

namespace Pufferfish::Driver::Serial::Backend {

// FrameReceiver

FrameProps::InputStatus FrameReceiver::input(uint8_t new_byte) {
  bool input_overwritten = false;
  auto status = chunk_splitter_.input(new_byte, input_overwritten);
  if (input_overwritten) {
    return FrameProps::InputStatus::input_overwritten;
  }

  using ChunkInputStatus = Protocols::Transport::ChunkInputStatus;
  switch (status) {
    case ChunkInputStatus::output_ready:
      return FrameProps::InputStatus::output_ready;
    case ChunkInputStatus::invalid_length:
      return FrameProps::InputStatus::invalid_length;
    case ChunkInputStatus::ok:
      break;
  }

  return FrameProps::InputStatus::ok;
}

FrameProps::OutputStatus FrameReceiver::output(FrameProps::PayloadBuffer &output_buffer) {
  FrameProps::EncodedBuffer temp_buffer;

  // Chunk
  using ChunkOutputStatus = Protocols::Transport::ChunkOutputStatus;
  auto status = chunk_splitter_.output(temp_buffer);
  switch (status) {
    case ChunkOutputStatus::invalid_length:
      return FrameProps::OutputStatus::invalid_length;
    case ChunkOutputStatus::waiting:
      return FrameProps::OutputStatus::waiting;
    case ChunkOutputStatus::ok:
      break;
  }

  // COBS
  if (cobs_decoder.transform(temp_buffer, output_buffer) != IndexStatus::ok) {
    return FrameProps::OutputStatus::invalid_cobs;
  }

  return FrameProps::OutputStatus::ok;
}

// FrameSender

FrameProps::OutputStatus FrameSender::transform(
    const FrameProps::PayloadBuffer &input_buffer, FrameProps::ChunkBuffer &output_buffer) const {
  // COBS
  if (cobs_encoder.transform(input_buffer, output_buffer) != IndexStatus::ok) {
    // does not return invalid_cobs status
    return FrameProps::OutputStatus::invalid_cobs;
  }

  // Chunk
  using ChunkOutputStatus = Protocols::Transport::ChunkOutputStatus;
  auto status = chunk_merger.transform(output_buffer);
  switch (status) {
    case ChunkOutputStatus::invalid_length:
      return FrameProps::OutputStatus::invalid_length;
    // does not return waiting status
    case ChunkOutputStatus::waiting:
      return FrameProps::OutputStatus::waiting;
    case ChunkOutputStatus::ok:
      break;
  }

  return FrameProps::OutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
