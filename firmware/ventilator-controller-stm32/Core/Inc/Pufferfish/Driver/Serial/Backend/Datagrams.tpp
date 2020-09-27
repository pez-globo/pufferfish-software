/*
 * Datagrams.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Datagrams.h"
#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/HAL/STM32/Endian.h"

namespace Pufferfish::Driver::Serial::Backend {

// Datagram

template <size_t output_size>
IndexStatus Datagram::write(Util::ByteArray<output_size> &output_buffer, HAL::CRC32C &crc32c) {
  length_ = static_cast<uint8_t>(payload_.size());
  if (write_protected(output_buffer) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }

  crc_ = crc32c.compute(
      output_buffer.buffer + protected_offset,  // exclude the CRC field
      output_buffer.size() - sizeof(uint32_t)   // exclude the size of the CRC field
  );
  uint32_t network_endian_crc = HAL::hton(crc_);
  memcpy(output_buffer.buffer, &network_endian_crc, sizeof(uint32_t));
  return IndexStatus::ok;
}

template <size_t output_size>
IndexStatus Datagram::write_protected(Util::ByteArray<output_size> &output_buffer) const {
  if (output_buffer.resize(header_size + payload_.size()) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }

  output_buffer.buffer[seq_offset] = seq_;
  output_buffer.buffer[length_offset] = length_;
  output_buffer.copy_from(payload_.buffer, payload_.size(), payload_offset);
  return IndexStatus::ok;
}

template <size_t input_size>
IndexStatus Datagram::parse(const Util::ByteArray<input_size> &input_buffer) {
  if (input_buffer.size() < header_size) {
    return IndexStatus::out_of_bounds;
  }
  uint32_t network_endian_crc = 0;
  memcpy(&network_endian_crc, input_buffer.buffer, sizeof(uint32_t));
  crc_ = HAL::ntoh(network_endian_crc);
  seq_ = input_buffer.buffer[seq_offset];
  length_ = input_buffer.buffer[length_offset];
  payload_.copy_from(input_buffer.buffer + payload_offset, input_buffer.size() - payload_offset);
  return IndexStatus::ok;
}

// DatagramReceiver

template <size_t input_size>
DatagramReceiver::Status DatagramReceiver::transform(
    const Util::ByteArray<input_size> &input_buffer, Datagram &output_datagram) {
  if (output_datagram.parse(input_buffer) != IndexStatus::ok) {
    return Status::invalid_parse;
  }

  if (compute_crc(input_buffer) != output_datagram.crc()) {
    return Status::invalid_crc;
  }

  if (output_datagram.payload().size() != output_datagram.length()) {
    return Status::invalid_length;
  }

  if (expected_seq_ != output_datagram.seq()) {
    expected_seq_ = output_datagram.seq() + 1;
    return Status::invalid_sequence;
  }

  ++expected_seq_;
  return Status::ok;
}

template <size_t input_size>
uint32_t DatagramReceiver::compute_crc(const Util::ByteArray<input_size> &input_buffer) {
  return crc32c_.compute(
      input_buffer.buffer + Datagram::protected_offset,  // exclude the CRC field
      input_buffer.size() - sizeof(uint32_t)             // exclude the size of the CRC field
  );
}

// DatagramSender

template <size_t output_size>
DatagramSender::Status DatagramSender::transform(
    const Datagram::PayloadBuffer &input_payload, Util::ByteArray<output_size> &output_buffer) {
  Datagram datagram(
      const_cast<Datagram::PayloadBuffer &>(input_payload),
      next_seq_);  // we promise not to call the parse metod
  if (datagram.write(output_buffer, crc32c_) != IndexStatus::ok) {
    return Status::invalid_length;
  }

  ++next_seq_;
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
