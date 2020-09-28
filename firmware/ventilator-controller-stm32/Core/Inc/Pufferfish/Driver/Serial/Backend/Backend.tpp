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
  BackendCRCReceiver::Props::PayloadBuffer temp_buffer2;
  BackendDatagramReceiver::Props::PayloadBuffer temp_buffer3;

  // Frame
  switch (frame_.output(temp_buffer1)) {
    case FrameProps::OutputStatus::waiting:
      return OutputStatus::waiting;
    case FrameProps::OutputStatus::invalid_length:
      return OutputStatus::invalid_frame_length;
    case FrameProps::OutputStatus::ok:
      break;
  }

  // CRCElement
  BackendParsedCRC receive_crc(temp_buffer2);
  switch (crc_.transform(temp_buffer1, receive_crc)) {
    case BackendCRCReceiver::Status::invalid_parse:
      return OutputStatus::invalid_crcelement_parse;
    case BackendCRCReceiver::Status::invalid_crc:
      return OutputStatus::invalid_crcelement_crc;
    case BackendCRCReceiver::Status::ok:
      break;
  }

  // Datagram
  BackendParsedDatagram receive_datagram(temp_buffer3);
  switch (datagram_.transform(temp_buffer2, receive_datagram)) {
    case BackendDatagramReceiver::Status::invalid_parse:
      return OutputStatus::invalid_datagram_parse;
    case BackendDatagramReceiver::Status::invalid_length:
      return OutputStatus::invalid_datagram_length;
    case BackendDatagramReceiver::Status::invalid_sequence:
      // TODO(lietk12): emit a warning about invalid sequence
    case BackendDatagramReceiver::Status::ok:
      break;
  }

  // Message
  switch (message_.transform(temp_buffer3, output_message)) {
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

BackendSender::Status BackendSender::transform(
    const Application::Message &input_message, FrameProps::ChunkBuffer &output_buffer) {
  BackendDatagramSender::Props::PayloadBuffer temp_buffer1;
  BackendCRCSender::Props::PayloadBuffer temp_buffer2;
  FrameProps::ChunkBuffer temp_buffer3;

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

  // CRCElement
  switch (crc_.transform(temp_buffer2, temp_buffer3)) {
    case BackendCRCSender::Status::invalid_length:
      return Status::invalid_crcelement_length;
    case BackendCRCSender::Status::ok:
      break;
  }

  // Frame
  switch (frame_.transform(temp_buffer3, output_buffer)) {
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
