/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Backend.h"

namespace Pufferfish::Driver::Serial::Backend {

// BackendReceiver

BackendReceiver::BackendReceiver(HAL::CRC32C &crc32c)
    : datagram_(crc32c), message_(message_descriptors) {}

BackendReceiver::InputStatus BackendReceiver::input(uint8_t new_byte) {
  switch (frame_.input(new_byte)) {
    case FrameProps::InputStatus::output_ready:
      return InputStatus::output_ready;
    case FrameProps::InputStatus::invalid_length:
      return InputStatus::invalid_frame_length;
    case FrameProps::InputStatus::ok:
      break;
  }
  return InputStatus::ok;
}

BackendReceiver::OutputStatus BackendReceiver::output(Application::Message &output_message) {
  FrameProps::ChunkBuffer temp_buffer1;
  BackendDatagramReceiver::Props::PayloadBuffer temp_buffer2;

  // Frame
  switch (frame_.output(temp_buffer1)) {
    case FrameProps::OutputStatus::waiting:
      return OutputStatus::waiting;
    case FrameProps::OutputStatus::invalid_length:
      return OutputStatus::invalid_frame_length;
    case FrameProps::OutputStatus::ok:
      break;
  }

  // Datagram
  BackendParsedDatagram receive_datagram(temp_buffer2);
  switch (datagram_.transform(temp_buffer1, receive_datagram)) {
    case BackendDatagramReceiver::Status::invalid_parse:
      return OutputStatus::invalid_datagram_parse;
    case BackendDatagramReceiver::Status::invalid_crc:
      return OutputStatus::invalid_datagram_crc;
    case BackendDatagramReceiver::Status::invalid_length:
      return OutputStatus::invalid_datagram_length;
    case BackendDatagramReceiver::Status::invalid_sequence:
      // TODO(lietk12): emit a warning about invalid sequence
    case BackendDatagramReceiver::Status::ok:
      break;
  }

  // Message
  switch (message_.transform(temp_buffer2, output_message)) {
    case Protocols::MessageStatus::invalid_length:
      return OutputStatus::invalid_message_length;
    case Protocols::MessageStatus::invalid_type:
      return OutputStatus::invalid_message_type;
    case Protocols::MessageStatus::invalid_encoding:
      return OutputStatus::invalid_message_encoding;
    case Protocols::MessageStatus::ok:
      break;
  }
  return OutputStatus::available;
}

// BackendSender

BackendSender::BackendSender(HAL::CRC32C &crc32c)
    : message_(message_descriptors), datagram_(crc32c) {}

BackendSender::Status BackendSender::transform(
    const Application::Message &input_message, FrameProps::ChunkBuffer &output_buffer) {
  BackendDatagramSender::Props::PayloadBuffer temp_buffer1;
  FrameProps::ChunkBuffer temp_buffer2;

  // Message
  switch (message_.transform(input_message, temp_buffer1)) {
    case Protocols::MessageStatus::invalid_length:
      return Status::invalid_message_length;
    case Protocols::MessageStatus::invalid_type:
      return Status::invalid_message_type;
    case Protocols::MessageStatus::invalid_encoding:
      return Status::invalid_message_encoding;
    case Protocols::MessageStatus::ok:
      break;
  }

  // Datagram
  switch (datagram_.transform(temp_buffer1, temp_buffer2)) {
    case BackendDatagramSender::Status::invalid_length:
      return Status::invalid_datagram_length;
    case BackendDatagramSender::Status::ok:
      break;
  }

  // Frame
  switch (frame_.transform(temp_buffer2, output_buffer)) {
    case FrameProps::OutputStatus::invalid_length:
      return Status::invalid_frame_length;
    case FrameProps::OutputStatus::ok:
      break;
    default:
      return Status::invalid_return_code;
  }
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
