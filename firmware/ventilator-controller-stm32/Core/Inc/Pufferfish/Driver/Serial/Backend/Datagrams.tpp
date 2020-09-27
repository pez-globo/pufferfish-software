/*
 * Datagrams.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/HAL/STM32/Endian.h"
#include "Datagrams.h"

namespace Pufferfish::Driver::Serial::Backend {

// Datagram

template<size_t OutputSize>
IndexStatus Datagram::write(
    Util::ByteArray<OutputSize> &outputBuffer, HAL::CRC32C &crc32c
) {
  length = static_cast<uint8_t>(payload.size());
  if (writeProtected(outputBuffer) != IndexStatus::ok) {
    return IndexStatus::outOfBounds;
  }

  crc = crc32c.compute(
      outputBuffer.buffer + protectedOffset, // exclude the CRC field
      outputBuffer.size() - sizeof(uint32_t) // exclude the size of the CRC field
  );
  uint32_t networkEndianCRC = HAL::hton(crc);
  memcpy(outputBuffer.buffer, &networkEndianCRC, sizeof(uint32_t));
  return IndexStatus::ok;
}

template<size_t OutputSize>
IndexStatus Datagram::writeProtected(
    Util::ByteArray<OutputSize> &outputBuffer
) const {
  if (outputBuffer.resize(headerSize + payload.size()) != IndexStatus::ok) {
    return IndexStatus::outOfBounds;
  }

  outputBuffer.buffer[seqOffset] = seq;
  outputBuffer.buffer[lengthOffset] = length;
  outputBuffer.copyFrom(payload.buffer, payload.size(), payloadOffset);
  return IndexStatus::ok;
}

template<size_t InputSize>
IndexStatus Datagram::parse(const Util::ByteArray<InputSize> &inputBuffer) {
  if (inputBuffer.size() < headerSize) {
    return IndexStatus::outOfBounds;
  }
  uint32_t networkEndianCRC;
  memcpy(&networkEndianCRC, inputBuffer.buffer, sizeof(uint32_t));
  crc = HAL::ntoh(networkEndianCRC);
  seq = inputBuffer.buffer[seqOffset];
  length = inputBuffer.buffer[lengthOffset];
  payload.copyFrom(
      inputBuffer.buffer + payloadOffset,
      inputBuffer.size() - payloadOffset
  );
  return IndexStatus::ok;
}

// DatagramReceiver

template<size_t InputSize>
DatagramReceiver::Status DatagramReceiver::transform(
    const Util::ByteArray<InputSize> &inputBuffer,
    Datagram &outputDatagram
) {
  if (outputDatagram.parse(inputBuffer) != IndexStatus::ok) {
    return Status::invalidParse;
  }

  if (computeCRC(inputBuffer) != outputDatagram.crc) {
    return Status::invalidCRC;
  }

  if (outputDatagram.payload.size() != outputDatagram.length) {
    return Status::invalidLength;
  }

  if (expectedSeq != outputDatagram.seq) {
    expectedSeq = outputDatagram.seq + 1;
    return Status::invalidSequence;
  }

  ++expectedSeq;
  return Status::ok;
}


template<size_t InputSize>
uint32_t DatagramReceiver::computeCRC(
    const Util::ByteArray<InputSize> &inputBuffer
) {
  return crc32c.compute(
      inputBuffer.buffer + Datagram::protectedOffset, // exclude the CRC field
      inputBuffer.size() - sizeof(uint32_t) // exclude the size of the CRC field
  );
}

// DatagramSender

template<size_t OutputSize>
DatagramSender::Status DatagramSender::transform(
    const Datagram::PayloadBuffer &inputPayload,
    Util::ByteArray<OutputSize> &outputBuffer
) {
  Datagram datagram(const_cast<Datagram::PayloadBuffer &>(inputPayload)); // we promise not to call the parse metod
  datagram.seq = nextSeq;
  if (datagram.write(outputBuffer, crc32c) != IndexStatus::ok) {
      return Status::invalidLength;
    }

  ++nextSeq;
  return Status::ok;
}

}
