/*
 * Frames.cpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Driver/Serial/Backend/Frames.h"

namespace Pufferfish::Driver::Serial::Backend {

// FrameReceiver

FrameReceiver::InputStatus FrameReceiver::input(uint8_t newByte) {
  switch (chunkSplitter.input(newByte)) {
  case ChunkSplitter<chunkMaxSize>::InputStatus::inputReady:
    return InputStatus::inputReady;
  case ChunkSplitter<chunkMaxSize>::InputStatus::invalidLength:
    return InputStatus::invalidChunkLength;
  case ChunkSplitter<chunkMaxSize>::InputStatus::outputReady:
    break;
  }
  return InputStatus::outputReady;
}

FrameReceiver::OutputStatus FrameReceiver::output(ChunkBuffer &outputBuffer) {
  ChunkBuffer tempBuffer;

  // Chunk
  switch (chunkSplitter.output(tempBuffer)) {
  case ChunkSplitter<chunkMaxSize>::OutputStatus::waiting:
    return OutputStatus::waiting;
  case ChunkSplitter<chunkMaxSize>::OutputStatus::invalidLength:
    return OutputStatus::invalidChunkLength;
  case ChunkSplitter<chunkMaxSize>::OutputStatus::available:
    break;
  }

  // COBS
  switch (cobsDecoder.transform(tempBuffer, outputBuffer)) {
  case COBSDecoder::Status::invalidLength:
    return OutputStatus::invalidCOBSLength;
  case COBSDecoder::Status::ok:
    break;
  }
  return OutputStatus::available;
}

// FrameSender

FrameSender::Status FrameSender::transform(
    const ChunkBuffer &inputBuffer, ChunkBuffer &outputBuffer
) const {
  // COBS
  switch (cobsEncoder.transform(inputBuffer, outputBuffer)) {
  case COBSEncoder::Status::invalidLength:
    return Status::invalidCOBSLength;
  case COBSEncoder::Status::ok:
    break;
  }

  // Chunk
  switch (chunkMerger.transform(outputBuffer)) {
  case ChunkMerger::Status::invalidLength:
    return Status::invalidChunkLength;
  case ChunkMerger::Status::ok:
    break;
  }
  return Status::ok;
}

}
