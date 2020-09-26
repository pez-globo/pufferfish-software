/*
 * Frames.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_FRAMES_H_
#define INC_PUFFERFISH_PROTOCOLS_FRAMES_H_

#include <stdint.h>
#include <stddef.h>
#include "Pufferfish/Util/ByteArray.h"

namespace Pufferfish { namespace Protocols {

// Splits delimited chunks from a stream
template<size_t BufferSize>
class ChunkSplitter {
public:
  enum class InputStatus {inputReady = 0, outputReady, invalidLength};
  enum class OutputStatus {available = 0, waiting, invalidLength};

  ChunkSplitter(uint8_t delimiter = 0x00);

  // Call this until it returns available, then call output
  InputStatus input(uint8_t newByte);
  OutputStatus output(Util::ByteArray<BufferSize> &outputBuffer);

protected:
  Util::ByteArray<BufferSize> buffer;
  const uint8_t delimiter;
  InputStatus inputStatus = InputStatus::inputReady;
};

static const size_t chunkMaxSize = 256; // including delimiter
using ChunkBuffer = Util::ByteArray<chunkMaxSize>;
static const size_t framePayloadMaxSize = chunkMaxSize - 2;
using FrameChunkSplitter = ChunkSplitter<chunkMaxSize>;

// Merges chunks into a stream by delimiting them
class ChunkMerger {
public:
  enum class Status {ok = 0, invalidLength};

  ChunkMerger(uint8_t delimiter = 0x00);

  template<size_t BufferSize>
  Status transform(Util::ByteArray<BufferSize> &inputOutputBuffer) const;

protected:
  const uint8_t delimiter;
};

// Decodes frames (length up to 255 bytes, excluding frame delimiter) with COBS
class COBSDecoder {
public:
  enum class Status {ok = 0, invalidLength};

  template<size_t InputSize, size_t OutputSize>
  Status transform(
      const Util::ByteArray<InputSize> &inputBuffer,
      Util::ByteArray<OutputSize> &outputBuffer
  ) const;
};

// Encodes payloads (length up to 254 bytes) with COBS; does not add the frame delimiter
class COBSEncoder {
public:
  enum class Status {ok = 0, invalidLength};

  template<size_t InputSize, size_t OutputSize>
  Status transform(
      const Util::ByteArray<InputSize> &inputBuffer,
      Util::ByteArray<OutputSize> &outputBuffer
  ) const;
};

class FrameReceiver {
public:
  enum class InputStatus {
    inputReady = 0, outputReady, invalidChunkLength
  };
  enum class OutputStatus {
    available = 0, waiting, invalidChunkLength, invalidCOBSLength
  };

  // Call this until it returns available, then call output
  InputStatus input(uint8_t newByte);
  OutputStatus output(ChunkBuffer &outputBuffer);

protected:
  FrameChunkSplitter chunkSplitter;
  const COBSDecoder cobsDecoder = COBSDecoder();
};

class FrameSender {
public:
  enum class Status {ok = 0, invalidCOBSLength, invalidChunkLength};

  Status transform(
      const ChunkBuffer &inputBuffer, ChunkBuffer &outputBuffer
  ) const;

protected:
  const COBSEncoder cobsEncoder = COBSEncoder();
  const ChunkMerger chunkMerger = ChunkMerger();
};

} }

#include "Frames.tpp"

#endif /* INC_PUFFERFISH_PROTOCOLS_FRAMES_H_ */
