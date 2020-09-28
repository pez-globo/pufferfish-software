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

  crc_ = crc32c.compute(
      output_buffer.buffer() + DatagramProps::protected_offset,  // exclude the CRC field
      output_buffer.size() - sizeof(uint32_t)                    // exclude the size of the CRC field
  );
  Util::write_hton(crc_, output_buffer.buffer());
  return IndexStatus::ok;
}

template <typename PayloadBuffer>
template <size_t output_size>
IndexStatus Datagram<PayloadBuffer>::write_protected(
    Util::ByteVector<output_size> &output_buffer) const {
  if (output_buffer.resize(DatagramProps::header_size + payload_.size()) != IndexStatus::ok) {
    return IndexStatus::out_of_bounds;
  }

  output_buffer[DatagramProps::seq_offset] = seq_;
  output_buffer[DatagramProps::length_offset] = length_;
  output_buffer.copy_from(payload_.buffer(), payload_.size(), DatagramProps::payload_offset);
  return IndexStatus::ok;
}

template <typename PayloadBuffer>
template <size_t input_size>
IndexStatus Datagram<PayloadBuffer>::parse(
    const Util::ByteVector<input_size> &input_buffer) {
  static_assert(
      !std::is_const<PayloadBuffer>::value,
      "Parse method unavailable for Datagrams with const PayloadBuffer type");

  if (input_buffer.size() < DatagramProps::header_size) {
    return IndexStatus::out_of_bounds;
  }
  Util::read_ntoh(input_buffer.buffer(), crc_);
  seq_ = input_buffer[DatagramProps::seq_offset];
  length_ = input_buffer[DatagramProps::length_offset];
  payload_.copy_from(
      input_buffer.buffer() + DatagramProps::payload_offset,
      input_buffer.size() - DatagramProps::payload_offset);
  return IndexStatus::ok;
}


// DatagramReceiver

template <size_t input_size>
DatagramReceiver::Status DatagramReceiver::transform(
    const Util::ByteVector<input_size> &input_buffer, ParsedDatagram &output_datagram) {
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
uint32_t DatagramReceiver::compute_crc(const Util::ByteVector<input_size> &input_buffer) {
  return crc32c_.compute(
      input_buffer.buffer() + DatagramProps::protected_offset,  // exclude the CRC field
      input_buffer.size() - sizeof(uint32_t)               // exclude the size of the CRC field
  );
}

// DatagramSender

template <size_t output_size>
DatagramSender::Status DatagramSender::transform(
    const DatagramProps::PayloadBuffer &input_payload,
    Util::ByteVector<output_size> &output_buffer) {
  ConstructedDatagram datagram(input_payload, next_seq_);
  if (datagram.write(output_buffer, crc32c_) != IndexStatus::ok) {
    return Status::invalid_length;
  }

  ++next_seq_;
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
