/*
 * Datagrams.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <type_traits>

#include "Datagrams.h"
#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/Util/Endian.h"

namespace Pufferfish::Driver::Serial::Backend {

// Datagram

template <typename PayloadBuffer>
template <size_t output_size>
IndexStatus Datagram<PayloadBuffer>::write(
    Util::ByteVector<output_size> &output_buffer, HAL::CRC32C &crc32c) {
  length_ = static_cast<uint8_t>(payload_.size());
  if (write_protected(output_buffer) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }

  crc_ = compute_body_crc(output_buffer, crc32c);
  Util::write_hton(crc_, output_buffer.buffer());
  return IndexStatus::ok;
}

template <typename PayloadBuffer>
template <size_t output_size>
IndexStatus Datagram<PayloadBuffer>::write_protected(
    Util::ByteVector<output_size> &output_buffer) const {
  if (output_buffer.resize(DatagramHeaderProps::header_size + payload_.size()) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }

  output_buffer[DatagramHeaderProps::seq_offset] = seq_;
  output_buffer[DatagramHeaderProps::length_offset] = length_;
  output_buffer.copy_from(payload_.buffer(), payload_.size(), DatagramHeaderProps::payload_offset);
  return IndexStatus::ok;
}

template <typename PayloadBuffer>
template <size_t input_size>
IndexStatus Datagram<PayloadBuffer>::parse(const Util::ByteVector<input_size> &input_buffer) {
  static_assert(
      !std::is_const<PayloadBuffer>::value,
      "Parse method unavailable for Datagrams with const PayloadBuffer type");

  if (input_buffer.size() < DatagramHeaderProps::header_size) {
    return IndexStatus::out_of_bounds;
  }
  Util::read_ntoh(input_buffer.buffer(), crc_);
  seq_ = input_buffer[DatagramHeaderProps::seq_offset];
  length_ = input_buffer[DatagramHeaderProps::length_offset];
  payload_.copy_from(
      input_buffer.buffer() + DatagramHeaderProps::payload_offset,
      input_buffer.size() - DatagramHeaderProps::payload_offset);
  return IndexStatus::ok;
}

template <typename PayloadBuffer>
template <size_t buffer_size>
uint32_t Datagram<PayloadBuffer>::compute_body_crc(
    const Util::ByteVector<buffer_size> &buffer, HAL::CRC32C &crc32c) {
  return crc32c.compute(
      buffer.buffer() + DatagramHeaderProps::protected_offset,  // exclude the CRC field
      buffer.size() - sizeof(uint32_t)                          // exclude the size of the CRC field
  );
}

// DatagramReceiver

template <size_t body_max_size>
template <size_t input_size>
typename DatagramReceiver<body_max_size>::Status DatagramReceiver<body_max_size>::transform(
    const Util::ByteVector<input_size> &input_buffer,
    ParsedDatagram<body_max_size> &output_datagram) {
  if (output_datagram.parse(input_buffer) != IndexStatus::ok) {
    return Status::invalid_parse;
  }

  if (ParsedDatagram<body_max_size>::compute_body_crc(input_buffer, crc32c_) !=
      output_datagram.crc()) {
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

// DatagramSender

template <size_t body_max_size>
template <size_t output_size>
typename DatagramSender<body_max_size>::Status DatagramSender<body_max_size>::transform(
    const typename Props::PayloadBuffer &input_payload,
    Util::ByteVector<output_size> &output_buffer) {
  ConstructedDatagram<body_max_size> datagram(input_payload, next_seq_);
  if (datagram.write(output_buffer, crc32c_) != IndexStatus::ok) {
    return Status::invalid_length;
  }

  ++next_seq_;
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
