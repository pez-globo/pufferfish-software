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

namespace Pufferfish::Protocols {

enum class ChunkInputStatus { ok = 0, output_ready, invalid_length };
enum class ChunkOutputStatus { ok = 0, waiting, invalid_length };

// Splits delimited chunks from a stream
template <size_t buffer_size>
class ChunkSplitter {
 public:
  explicit ChunkSplitter(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  // Call this until it returns available, then call output
  ChunkInputStatus input(uint8_t new_byte);
  ChunkOutputStatus output(Util::ByteVector<buffer_size> &output_buffer);

 private:
  Util::ByteVector<buffer_size> buffer_;
  const uint8_t delimiter;
  ChunkInputStatus input_status_ = ChunkInputStatus::ok;
};

// Merges chunks into a stream by delimiting them
class ChunkMerger {
 public:
  explicit ChunkMerger(uint8_t delimiter = 0x00) : delimiter(delimiter) {}

  template <size_t buffer_size>
  ChunkOutputStatus transform(Util::ByteVector<buffer_size> &input_output_buffer) const;

 private:
  const uint8_t delimiter = 0x00;
};

}  // namespace Pufferfish::Protocols

#include "Chunks.tpp"