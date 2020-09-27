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

// Splits delimited chunks from a stream
template <size_t buffer_size>
class ChunkSplitter {
 public:
  enum class InputStatus { input_ready = 0, output_ready, invalid_length };
  enum class OutputStatus { available = 0, waiting, invalid_length };

  explicit ChunkSplitter(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  // Call this until it returns available, then call output
  InputStatus input(uint8_t new_byte);
  OutputStatus output(Util::ByteVector<buffer_size> &output_buffer);

 private:
  Util::ByteVector<buffer_size> buffer_;
  const uint8_t delimiter;
  InputStatus input_status_ = InputStatus::input_ready;
};

static const size_t chunk_max_size = 256;  // including delimiter
using ChunkBuffer = Util::ByteVector<chunk_max_size>;
static const size_t frame_payload_max_size = chunk_max_size - 2;
using FrameChunkSplitter = ChunkSplitter<chunk_max_size>;

// Merges chunks into a stream by delimiting them
class ChunkMerger {
 public:
  enum class Status { ok = 0, invalid_length };

  explicit ChunkMerger(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  template <size_t buffer_size>
  Status transform(Util::ByteVector<buffer_size> &input_output_buffer) const;

 private:
  const uint8_t delimiter = 0x00;
};

// Decodes frames (length up to 255 bytes, excluding frame delimiter) with COBS
class COBSDecoder {
 public:
  enum class Status { ok = 0, invalid_length };

  template <size_t input_size, size_t output_size>
  Status transform(
      const Util::ByteVector<input_size> &input_buffer,
      Util::ByteVector<output_size> &output_buffer) const;
};

// Encodes payloads (length up to 254 bytes) with COBS; does not add the frame delimiter
class COBSEncoder {
 public:
  enum class Status { ok = 0, invalid_length };

  template <size_t input_size, size_t output_size>
  Status transform(
      const Util::ByteVector<input_size> &input_buffer,
      Util::ByteVector<output_size> &output_buffer) const;
};

class FrameReceiver {
 public:
  enum class InputStatus { input_ready = 0, output_ready, invalid_chunk_length };
  enum class OutputStatus { available = 0, waiting, invalid_chunk_length, invalid_cobs_length };

  // Call this until it returns available, then call output
  InputStatus input(uint8_t new_byte);
  OutputStatus output(ChunkBuffer &output_buffer);

 private:
  FrameChunkSplitter chunk_splitter_;
  const COBSDecoder cobs_decoder = COBSDecoder();
};

class FrameSender {
 public:
  enum class Status { ok = 0, invalid_cobs_length, invalid_chunk_length };

  Status transform(const ChunkBuffer &input_buffer, ChunkBuffer &output_buffer) const;

 private:
  const COBSEncoder cobs_encoder = COBSEncoder();
  const ChunkMerger chunk_merger = ChunkMerger();
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Frames.tpp"
