/*
 * Datagrams.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/Util/Vector.h"

namespace Pufferfish::Driver::Serial::Backend {

struct DatagramHeaderProps {
  static const size_t protected_offset = sizeof(uint32_t);
  static const size_t seq_offset = protected_offset;
  static const size_t length_offset = seq_offset + sizeof(uint8_t);
  static const size_t payload_offset = length_offset + sizeof(uint8_t);

  static const size_t header_size = payload_offset;
};

template <size_t body_max_size>
struct DatagramProps {
  static const size_t payload_max_size = body_max_size - DatagramHeaderProps::header_size;
  using PayloadBuffer = Util::ByteVector<payload_max_size>;
};

template <typename PayloadBuffer>
class Datagram {
 public:
  explicit Datagram(PayloadBuffer &payload, uint8_t seq)
      : seq_(seq), length_(static_cast<uint8_t>(payload.size())), payload_(payload) {}

  explicit Datagram(PayloadBuffer &payload) : Datagram(payload, 0) {}

  [[nodiscard]] uint32_t crc() const { return crc_; }
  [[nodiscard]] uint8_t seq() const { return seq_; }
  [[nodiscard]] uint8_t length() const { return length_; }
  [[nodiscard]] const PayloadBuffer &payload() const { return payload_; }

  template <size_t output_size>
  IndexStatus write(
      Util::ByteVector<output_size> &output_buffer,
      HAL::CRC32C &crc32c);  // updates length and crc fields

  template <size_t input_size>
  IndexStatus parse(
      const Util::ByteVector<input_size> &input_buffer);  // updates all fields, including payload

  template <size_t buffer_size>
  static uint32_t compute_body_crc(
      const Util::ByteVector<buffer_size> &buffer, HAL::CRC32C &crc32c);

 private:
  uint32_t crc_ = 0;
  uint8_t seq_ = 0;
  uint8_t length_ = 0;
  PayloadBuffer &payload_;

  template <size_t output_size>
  IndexStatus write_protected(Util::ByteVector<output_size> &output_buffer) const;
};

// In this Datagram, the payload can only be set through the constructor, so
// a const payload can be given in the constructor. However, the parse method
// is not available, as it would modify the payload given in the constructor.
template <size_t body_max_size>
using ParsedDatagram = Datagram<typename DatagramProps<body_max_size>::PayloadBuffer>;

// In this Datagram, the payload can be modified through the parse method, so
// a const payload cannot be given in the constructor.
template <size_t body_max_size>
using ConstructedDatagram = Datagram<const typename DatagramProps<body_max_size>::PayloadBuffer>;

// Parses datagrams into payloads, with data integrity checking
template <size_t body_max_size>
class DatagramReceiver {
 public:
  using Props = DatagramProps<body_max_size>;
  enum class Status { ok = 0, invalid_parse, invalid_crc, invalid_length, invalid_sequence };

  explicit DatagramReceiver(HAL::CRC32C &crc32c) : crc32c_(crc32c) {}

  template <size_t input_size>
  Status transform(
      const Util::ByteVector<input_size> &input_buffer,
      ParsedDatagram<body_max_size> &output_datagram);

 private:
  uint8_t expected_seq_ = 0;
  HAL::CRC32C crc32c_;
};

// Generates datagrams from payloads
template <size_t body_max_size>
class DatagramSender {
 public:
  using Props = DatagramProps<body_max_size>;
  enum class Status { ok = 0, invalid_length };

  explicit DatagramSender(HAL::CRC32C &crc32c) : crc32c_(crc32c) {}

  template <size_t output_size>
  Status transform(
      const typename Props::PayloadBuffer &input_payload,
      Util::ByteVector<output_size> &output_buffer);

 private:
  uint8_t next_seq_ = 0;
  HAL::CRC32C crc32c_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Datagrams.tpp"
