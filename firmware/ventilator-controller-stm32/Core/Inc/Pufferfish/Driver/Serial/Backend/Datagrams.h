/*
 * Datagrams.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Frames.h"
#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/Util/ByteArray.h"

namespace Pufferfish::Driver::Serial::Backend {

class Datagram {
 public:
  static const size_t protected_offset = sizeof(uint32_t);
  static const size_t seq_offset = protected_offset;
  static const size_t length_offset = seq_offset + sizeof(uint8_t);
  static const size_t payload_offset = length_offset + sizeof(uint8_t);

  static const size_t header_size = payload_offset;
  static const size_t payload_max_size = frame_payload_max_size - header_size;
  using PayloadBuffer = Util::ByteArray<payload_max_size>;

  explicit Datagram(PayloadBuffer &payload)
      : length(static_cast<uint8_t>(payload.size())), payload(payload) {}

  uint32_t crc = 0;
  uint8_t seq = 0;
  uint8_t length = 0;
  PayloadBuffer &payload;

  template <size_t output_size>
  IndexStatus write(
      Util::ByteArray<output_size> &output_buffer,
      HAL::CRC32C &crc32c);  // updates length and crc fields

  template <size_t input_size>
  IndexStatus parse(
      const Util::ByteArray<input_size> &input_buffer);  // updates all fields, including payload

 protected:
  template <size_t output_size>
  IndexStatus write_protected(Util::ByteArray<output_size> &output_buffer) const;
};

// Parses datagrams into payloads, with data integrity checking
class DatagramReceiver {
 public:
  enum class Status { ok = 0, invalid_parse, invalid_crc, invalid_length, invalid_sequence };

  explicit DatagramReceiver(HAL::CRC32C &crc32c) : crc32c_(crc32c) {}

  template <size_t input_size>
  Status transform(const Util::ByteArray<input_size> &input_buffer, Datagram &output_datagram);

 protected:
  uint8_t expected_seq_ = 0;
  HAL::CRC32C crc32c_;

  template <size_t input_size>
  uint32_t compute_crc(const Util::ByteArray<input_size> &input_buffer);
};

// Generates datagrams from payloads
class DatagramSender {
 public:
  enum class Status { ok = 0, invalid_length };

  explicit DatagramSender(HAL::CRC32C &crc32c) : crc32c_(crc32c) {}

  template <size_t output_size>
  Status transform(
      const Datagram::PayloadBuffer &input_payload, Util::ByteArray<output_size> &output_buffer);

 protected:
  uint8_t next_seq_ = 0;
  HAL::CRC32C crc32c_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Datagrams.tpp"
