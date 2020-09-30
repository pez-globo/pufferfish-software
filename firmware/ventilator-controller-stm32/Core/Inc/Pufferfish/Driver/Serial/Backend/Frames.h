/*
 * Frames.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Pufferfish/Util/Vector.h"

namespace Pufferfish::Driver::Serial::Backend {

struct FrameProps {
  static const size_t payload_max_size = 254;
  static const size_t chunk_max_size = payload_max_size + 2;  // including delimiter
  using ChunkBuffer = Util::ByteVector<chunk_max_size>;

  enum class InputStatus { ok = 0, output_ready, invalid_length };
  enum class OutputStatus { ok = 0, waiting, invalid_length };
};

// Splits delimited chunks from a stream
template <size_t buffer_size>
class ChunkSplitter {
 public:
  explicit ChunkSplitter(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  // Call this until it returns available, then call output
  FrameProps::InputStatus input(uint8_t new_byte);
  FrameProps::OutputStatus output(Util::ByteVector<buffer_size> &output_buffer);

 private:
  Util::ByteVector<buffer_size> buffer_;
  const uint8_t delimiter;
  FrameProps::InputStatus input_status_ = FrameProps::InputStatus::ok;
};

using FrameChunkSplitter = ChunkSplitter<FrameProps::chunk_max_size>;

// Merges chunks into a stream by delimiting them
class ChunkMerger {
 public:
  explicit ChunkMerger(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  template <size_t buffer_size>
  FrameProps::OutputStatus transform(Util::ByteVector<buffer_size> &input_output_buffer) const;

 private:
  const uint8_t delimiter = 0x00;
};

// Decodes frames (length up to 255 bytes, excluding frame delimiter) with COBS
class COBSDecoder {
 public:
  COBSDecoder() = default;

  template <size_t input_size, size_t output_size>
  FrameProps::OutputStatus transform(
      const Util::ByteVector<input_size> &input_buffer,
      Util::ByteVector<output_size> &output_buffer) const;
};

// Encodes payloads (length up to 254 bytes) with COBS; does not add the frame delimiter
class COBSEncoder {
 public:
  COBSEncoder() = default;

  template <size_t input_size, size_t output_size>
  FrameProps::OutputStatus transform(
      const Util::ByteVector<input_size> &input_buffer,
      Util::ByteVector<output_size> &output_buffer) const;
};

class FrameReceiver {
 public:
  FrameReceiver() = default;

  // Call this until it returns available, then call output
  FrameProps::InputStatus input(uint8_t new_byte);
  FrameProps::OutputStatus output(FrameProps::ChunkBuffer &output_buffer);

 private:
  FrameChunkSplitter chunk_splitter_;
  const COBSDecoder cobs_decoder = COBSDecoder();
};

class FrameSender {
 public:
  FrameSender() = default;

  FrameProps::OutputStatus transform(
      const FrameProps::ChunkBuffer &input_buffer, FrameProps::ChunkBuffer &output_buffer) const;

 private:
  const COBSEncoder cobs_encoder = COBSEncoder();
  const ChunkMerger chunk_merger = ChunkMerger();
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Frames.tpp"
