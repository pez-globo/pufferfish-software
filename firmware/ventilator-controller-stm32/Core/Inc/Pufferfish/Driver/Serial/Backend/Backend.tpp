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
    case FrameReceiver::InputStatus::output_ready:
      return InputStatus::output_ready;
    case FrameReceiver::InputStatus::invalid_chunk_length:
      return InputStatus::invalid_frame_chunk_length;
    case FrameReceiver::InputStatus::input_ready:
      break;
  }
  return InputStatus::input_ready;
}

BackendReceiver::OutputStatus BackendReceiver::output(Application::Message &output_message) {
  ChunkBuffer temp_buffer1;
  Datagram::PayloadBuffer temp_buffer2;

  // Frame
  switch (frame_.output(temp_buffer1)) {
    case FrameReceiver::OutputStatus::waiting:
      return OutputStatus::waiting;
    case FrameReceiver::OutputStatus::invalid_chunk_length:
      return OutputStatus::invalid_frame_chunk_length;
    case FrameReceiver::OutputStatus::invalid_cobs_length:
      return OutputStatus::invalid_frame_cobs_length;
    case FrameReceiver::OutputStatus::available:
      break;
  }

  // Datagram
  Datagram receive_datagram(temp_buffer2);
  switch (datagram_.transform(temp_buffer1, receive_datagram)) {
    case DatagramReceiver::Status::invalid_parse:
      return OutputStatus::invalid_datagram_parse;
    case DatagramReceiver::Status::invalid_crc:
      return OutputStatus::invalid_datagram_crc;
    case DatagramReceiver::Status::invalid_length:
      return OutputStatus::invalid_datagram_length;
    case DatagramReceiver::Status::invalid_sequence:
      // TODO(lietk12): emit a warning about invalid sequence
    case DatagramReceiver::Status::ok:
      break;
  }

  // Message
  switch (message_.transform(temp_buffer2, output_message)) {
    case BackendMessageReceiver::Status::invalid_length:
      return OutputStatus::invalid_message_length;
    case BackendMessageReceiver::Status::invalid_type:
      return OutputStatus::invalid_message_type;
    case BackendMessageReceiver::Status::invalid_encoding:
      return OutputStatus::invalid_message_encoding;
    case BackendMessageReceiver::Status::ok:
      break;
  }
  return OutputStatus::available;
}

// BackendSender

BackendSender::BackendSender(HAL::CRC32C &crc32c)
    : message_(message_descriptors), datagram_(crc32c) {}

BackendSender::Status BackendSender::transform(
    const Application::Message &input_message, ChunkBuffer &output_buffer) {
  Datagram::PayloadBuffer temp_buffer1;
  ChunkBuffer temp_buffer2;

  // Message
  switch (message_.transform(input_message, temp_buffer1)) {
    case BackendMessageSender::Status::invalid_length:
      return Status::invalid_message_length;
    case BackendMessageSender::Status::invalid_type:
      return Status::invalid_message_type;
    case BackendMessageSender::Status::invalid_encoding:
      return Status::invalid_message_encoding;
    case BackendMessageSender::Status::ok:
      break;
  }

  // Datagram
  switch (datagram_.transform(temp_buffer1, temp_buffer2)) {
    case DatagramSender::Status::invalid_length:
      return Status::invalid_datagram_length;
    case DatagramSender::Status::ok:
      break;
  }

  // Frame
  switch (frame_.transform(temp_buffer2, output_buffer)) {
    case FrameSender::Status::invalid_cobs_length:
      return Status::invalid_frame_cobs_length;
    case FrameSender::Status::invalid_chunk_length:
      return Status::invalid_frame_chunk_length;
    case FrameSender::Status::ok:
      break;
  }
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
