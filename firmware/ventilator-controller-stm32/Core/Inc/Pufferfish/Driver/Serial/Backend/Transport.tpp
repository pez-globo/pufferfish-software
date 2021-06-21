/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Transport.h"

namespace Pufferfish::Driver::Serial::Backend {

// Receiver

Receiver::InputStatus Receiver::input(uint8_t new_byte) {
  switch (frame_.input(new_byte)) {
    case FrameProps::InputStatus::output_ready:
      return InputStatus::output_ready;
    case FrameProps::InputStatus::invalid_length:
      return InputStatus::invalid_frame_length;
    case FrameProps::InputStatus::input_overwritten:
      return InputStatus::input_overwritten;
    case FrameProps::InputStatus::ok:
      break;
  }
  return InputStatus::ok;
}

Receiver::OutputStatus Receiver::output(Message &output_message) {
  FrameProps::PayloadBuffer temp_buffer1;
  CRCReceiver::Props::PayloadBuffer temp_buffer2;
  DatagramReceiver::Props::PayloadBuffer temp_buffer3;

  // Frame
  switch (frame_.output(temp_buffer1)) {
    case FrameProps::OutputStatus::waiting:
      return OutputStatus::waiting;
    case FrameProps::OutputStatus::invalid_length:
      return OutputStatus::invalid_frame_length;
    case FrameProps::OutputStatus::invalid_cobs:
      return OutputStatus::invalid_frame_encoding;
    case FrameProps::OutputStatus::ok:
      break;
  }

  // CRCElement
  ParsedCRC receive_crc(temp_buffer2);
  switch (crc_.transform(temp_buffer1, receive_crc)) {
    case CRCReceiver::Status::invalid_parse:
      return OutputStatus::invalid_crcelement_parse;
    case CRCReceiver::Status::invalid_crc:
      return OutputStatus::invalid_crcelement_crc;
    case CRCReceiver::Status::ok:
      break;
  }

  // Datagram
  ParsedDatagram receive_datagram(temp_buffer3);
  switch (datagram_.transform(temp_buffer2, receive_datagram)) {
    case DatagramReceiver::Status::invalid_parse:
      return OutputStatus::invalid_datagram_parse;
    case DatagramReceiver::Status::invalid_length:
      return OutputStatus::invalid_datagram_length;
    case DatagramReceiver::Status::invalid_sequence:
      // TODO(lietk12): emit a warning about invalid sequence
    case DatagramReceiver::Status::ok:
      break;
  }

  // Message
  using MessageStatus = Protocols::Transport::MessageStatus;
  switch (message_.transform(temp_buffer3, output_message)) {
    case MessageStatus::invalid_length:
      return OutputStatus::invalid_message_length;
    case MessageStatus::invalid_type:
      return OutputStatus::invalid_message_type;
    case MessageStatus::invalid_encoding:
      return OutputStatus::invalid_message_encoding;
    case MessageStatus::ok:
      break;
  }
  return OutputStatus::available;
}

// Sender

Sender::Status Sender::transform(
    const Application::StateSegment &state_segment, FrameProps::ChunkBuffer &output_buffer) {
  DatagramSender::Props::PayloadBuffer temp_buffer1;
  CRCSender::Props::PayloadBuffer temp_buffer2;
  FrameProps::PayloadBuffer temp_buffer3;

  // Message
  using MessageStatus = Protocols::Transport::MessageStatus;
  switch (message_.transform(state_segment, temp_buffer1)) {
    case MessageStatus::invalid_length:
      return Status::invalid_message_length;
    case MessageStatus::invalid_type:
      return Status::invalid_message_type;
    case MessageStatus::invalid_encoding:
      return Status::invalid_message_encoding;
    case MessageStatus::ok:
      break;
  }

  // Datagram
  switch (datagram_.transform(temp_buffer1, temp_buffer2)) {
    case DatagramSender::Status::invalid_length:
      return Status::invalid_datagram_length;
    case DatagramSender::Status::ok:
      break;
  }

  // CRCElement
  switch (crc_.transform(temp_buffer2, temp_buffer3)) {
    case CRCSender::Status::invalid_length:
      return Status::invalid_crcelement_length;
    case CRCSender::Status::ok:
      break;
  }

  // Frame
  switch (frame_.transform(temp_buffer3, output_buffer)) {
    case FrameProps::OutputStatus::invalid_length:
      return Status::invalid_frame_length;
    case FrameProps::OutputStatus::invalid_cobs:
      return Status::invalid_frame_encoding;
    case FrameProps::OutputStatus::ok:
      break;
    default:
      return Status::invalid_return_code;
  }
  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
