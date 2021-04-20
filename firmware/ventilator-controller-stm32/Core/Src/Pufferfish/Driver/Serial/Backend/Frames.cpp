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

  switch (status) {
    case Protocols::ChunkInputStatus::output_ready:
      return FrameProps::InputStatus::output_ready;
    case Protocols::ChunkInputStatus::invalid_length:
      return FrameProps::InputStatus::invalid_length;
    case Protocols::ChunkInputStatus::ok:
      break;
  }

  return FrameProps::InputStatus::ok;
}

FrameProps::OutputStatus FrameReceiver::output(FrameProps::PayloadBuffer &output_buffer) {
  FrameProps::EncodedBuffer temp_buffer;

  // Chunk
  auto status = chunk_splitter_.output(temp_buffer);
  switch (status) {
    case Protocols::ChunkOutputStatus::invalid_length:
      return FrameProps::OutputStatus::invalid_length;
    case Protocols::ChunkOutputStatus::waiting:
      return FrameProps::OutputStatus::waiting;
    case Protocols::ChunkOutputStatus::ok:
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
    return FrameProps::OutputStatus::invalid_cobs;
  }

  // Chunk
  auto status = chunk_merger.transform(output_buffer);
  switch (status) {
    case Protocols::ChunkOutputStatus::invalid_length:
      return FrameProps::OutputStatus::invalid_length;
    case Protocols::ChunkOutputStatus::waiting:
      return FrameProps::OutputStatus::waiting;
    case Protocols::ChunkOutputStatus::ok:
      break;
  }

  return FrameProps::OutputStatus::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
