/*
 * Frames.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Util/COBS.h"
#include "Pufferfish/Statuses.h"
#include "Frames.h"

namespace Pufferfish::Driver::Serial::Backend {

// ChunkSplitter

template<size_t BufferSize>
ChunkSplitter<BufferSize>::ChunkSplitter(uint8_t delimiter) :
  delimiter(delimiter) {}

template<size_t BufferSize>
typename ChunkSplitter<BufferSize>::InputStatus
ChunkSplitter<BufferSize>::input(uint8_t newByte) {
  if (newByte == delimiter) {
    inputStatus = InputStatus::outputReady;
    return inputStatus;
  }

  if (buffer.pushBack(newByte) != IndexStatus::ok) {
    inputStatus = InputStatus::invalidLength;
    return inputStatus;
  }

  return InputStatus::inputReady;
}

template<size_t BufferSize>
typename ChunkSplitter<BufferSize>::OutputStatus
ChunkSplitter<BufferSize>::output(
    Util::ByteArray<BufferSize> &outputBuffer
) {
  if (inputStatus == InputStatus::inputReady) {
    return OutputStatus::waiting;
  }

  outputBuffer.copyFrom(buffer);
  buffer.clear();
  OutputStatus outputStatus = OutputStatus::available;
  if (inputStatus == InputStatus::invalidLength) {
    outputStatus = OutputStatus::invalidLength;
  }
  inputStatus = InputStatus::inputReady;
  return outputStatus;
}

// ChunkMerger

template<size_t BufferSize>
ChunkMerger::Status ChunkMerger::transform(
    Util::ByteArray<BufferSize> &inputOutputBuffer
) const {
  if (inputOutputBuffer.pushBack(delimiter) == IndexStatus::ok) {
    return Status::ok;
  } else {
    return Status::invalidLength;
  }
}

// COBSDecoder

template<size_t InputSize, size_t OutputSize>
COBSDecoder::Status COBSDecoder::transform(
    const Util::ByteArray<InputSize> &inputBuffer,
    Util::ByteArray<OutputSize> &outputBuffer
) const {
  if (inputBuffer.size() > framePayloadMaxSize + 1) {
    return Status::invalidLength;
  }

  if (outputBuffer.maxSize < inputBuffer.size()) {
    return Status::invalidLength;
  }

  outputBuffer.resize(
      Util::decodeCOBS(inputBuffer.buffer, inputBuffer.size(), outputBuffer.buffer)
  );
  return Status::ok;
}

// COBSEncoder

template<size_t InputSize, size_t OutputSize>
COBSEncoder::Status COBSEncoder::transform(
    const Util::ByteArray<InputSize> &inputBuffer,
    Util::ByteArray<OutputSize> &outputBuffer
) const {
  if (inputBuffer.size() > framePayloadMaxSize) {
    return Status::invalidLength;
  }

  size_t encodedSize = Util::getEncodedCOBSBufferSize(inputBuffer.size());
  if (outputBuffer.maxSize < encodedSize) {
    return Status::invalidLength;
  }

  outputBuffer.resize(
      Util::encodeCOBS(inputBuffer.buffer, inputBuffer.size(), outputBuffer.buffer)
  );
  return Status::ok;
}

}
