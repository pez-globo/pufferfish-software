/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/FDO2/Device.h"

#include <cstdio>

namespace FDO2 = Pufferfish::Driver::Serial::FDO2;

// This macro is used to add a setter for a specified request type with an associated
// union field and enum value. We use a macro because it makes the code more maintainable here,
// while allowing us to ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define REQUEST_TAGGED_SETTER(type, field) \
    template <>\
    template <>\
    void Util::TaggedUnion<FDO2::Requests::Union, FDO2::CommandTypes>::set<type>(\
        const type &new_value) {\
      tag = FDO2::CommandTypes::field;\
      value.field = new_value; } // NOLINT(cppcoreguidelines-pro-type-union-access)
// clang-format on

namespace Pufferfish::Util {

REQUEST_TAGGED_SETTER(FDO2::Requests::Vers, vers)
REQUEST_TAGGED_SETTER(FDO2::Requests::Logo, logo)
REQUEST_TAGGED_SETTER(FDO2::Requests::Bcst, bcst)

}  // namespace Pufferfish::Util

namespace Pufferfish::Driver::Serial::FDO2 {

// ResponseReceiver

ResponseReceiver::InputStatus ResponseReceiver::input(uint8_t new_byte) {
  switch (chunks_.input(new_byte)) {
    case Protocols::ChunkInputStatus::output_ready:
      return InputStatus::output_ready;
    case Protocols::ChunkInputStatus::invalid_length:
      return InputStatus::invalid_frame_length;
    case Protocols::ChunkInputStatus::ok:
      break;
  }
  return InputStatus::ok;
}

ResponseReceiver::OutputStatus ResponseReceiver::output(Response &output_response) {
  Responses::ChunkBuffer temp_buffer;

  // Chunk
  switch (chunks_.output(temp_buffer)) {
    case Protocols::ChunkOutputStatus::waiting:
      return OutputStatus::waiting;
    case Protocols::ChunkOutputStatus::invalid_length:
      return OutputStatus::invalid_frame_length;
    case Protocols::ChunkOutputStatus::ok:
      break;
  }

  // Command
  switch (commands_.transform(temp_buffer, output_response)) {
    case CommandReceiver::Status::invalid_header:
      return OutputStatus::invalid_header;
    case CommandReceiver::Status::invalid_args:
      return OutputStatus::invalid_args;
    case CommandReceiver::Status::ok:
      break;
  }

  return OutputStatus::available;
}

// RequestSender

RequestSender::Status RequestSender::transform(
    const Request &input_request, Requests::ChunkBuffer &output_buffer) const {
  // Command
  if (commands_.transform(input_request, output_buffer) != CommandSender::Status::ok) {
    return Status::invalid_command;
  }

  // Chunk
  if (chunks_.transform(output_buffer) != Protocols::ChunkOutputStatus::ok) {
    return Status::invalid_length;
  }
  /*if (output_buffer.push_back(0x0a) != IndexStatus::ok) {
    return Status::invalid_length;
  }*/

  return Status::ok;
}

// Device

Device::Status Device::start_broadcast() {
  Requests::ChunkBuffer request_buffer;
  Requests::Bcst bcst{broadcast_interval};
  Request request{};
  request.set(bcst);
  requests_.transform(request, request_buffer);
  HAL::AtomicSize written_size = 0;
  if (uart_.write(
          reinterpret_cast<uint8_t *>(request_buffer.buffer()),
          request_buffer.size(),
          written_size) != BufferStatus::ok) {
    return Status::timed_out;
  }

  return Status::ok;
}

Device::Status Device::receive(Response &response) {
  while (true) {  // repeat until UART read buffer is empty or output is available
    uint8_t receive = 0;

    // UART
    switch (uart_.read(receive)) {
      case BufferStatus::ok:
        break;
      case BufferStatus::empty:
        return Status::waiting;
      default:
        break;
    }

    // Responses
    switch (responses_.input(receive)) {
      case ResponseReceiver::InputStatus::invalid_frame_length:
        // TODO(lietk12): handle error case first
      case ResponseReceiver::InputStatus::ok:
        break;
      case ResponseReceiver::InputStatus::output_ready:
        switch (responses_.output(response)) {
          case ResponseReceiver::OutputStatus::available:
            return Status::ok;
          case ResponseReceiver::OutputStatus::waiting:
            return Status::waiting;
          default:
            break;
        }
    }
  }
}

Device::Status Device::flash_led() {
  Requests::ChunkBuffer request_buffer;
  Requests::Logo logo{};
  Request request{};
  request.set(logo);
  requests_.transform(request, request_buffer);
  HAL::AtomicSize written_size = 0;
  if (uart_.write(
          reinterpret_cast<uint8_t *>(request_buffer.buffer()),
          request_buffer.size(),
          written_size) != BufferStatus::ok) {
    return Status::timed_out;
  }

  return Status::ok;
}

Device::Status Device::request_version() {
  Requests::ChunkBuffer request_buffer;
  Requests::Vers vers{};
  Request request{};
  request.set(vers);
  requests_.transform(request, request_buffer);
  HAL::AtomicSize written_size = 0;
  if (uart_.write(
          reinterpret_cast<uint8_t *>(request_buffer.buffer()),
          request_buffer.size(),
          written_size) != BufferStatus::ok) {
    return Status::timed_out;
  }

  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::FDO2
