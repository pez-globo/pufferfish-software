/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Backend.h"

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

// Backend

constexpr bool Backend::accept_message(Application::MessageTypes type) noexcept {
  return InputStates::includes(type);
}

Backend::Status Backend::input(uint8_t new_byte) {
  // Input into receiver
  switch (receiver_.input(new_byte)) {
    case Receiver::InputStatus::output_ready:
      break;
    case Receiver::InputStatus::invalid_frame_length:
    case Receiver::InputStatus::input_overwritten:
      // TODO(lietk12): handle error case first
    case Receiver::InputStatus::ok:
      return Status::waiting;
  }

  // Output from receiver
  Message message;
  switch (receiver_.output(message)) {
    case Receiver::OutputStatus::invalid_datagram_sequence:
      // TODO(lietk12): handle warning case first
    case Receiver::OutputStatus::available:
      break;
    case Receiver::OutputStatus::invalid_frame_length:
    case Receiver::OutputStatus::invalid_frame_encoding:
    case Receiver::OutputStatus::invalid_crcelement_parse:
    case Receiver::OutputStatus::invalid_crcelement_crc:
    case Receiver::OutputStatus::invalid_datagram_parse:
    case Receiver::OutputStatus::invalid_datagram_length:
    case Receiver::OutputStatus::invalid_message_length:
    case Receiver::OutputStatus::invalid_message_type:
    case Receiver::OutputStatus::invalid_message_encoding:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
    case Receiver::OutputStatus::waiting:
      return Status::waiting;
  }

  if (!accept_message(message.payload.tag)) {
    return Status::invalid;
  }

  // Input into state synchronization
  switch (store_.input(message.payload)) {
    case Application::Store::InputStatus::ok:
      break;
    case Application::Store::InputStatus::invalid_type:
      // TODO(lietk12): handle error case
      return Status::invalid;
  }

  return Status::ok;
}

void Backend::update_clock(uint32_t current_time) {
  synchronizer_.input(current_time);
}

void Backend::update_list_senders() {
  const ExpectedLogEvent &event = store_.expected_log_event();
  if (log_events_sender_.input(event.id, event.session_id) !=
      Protocols::Application::ListInputStatus::ok) {
    // TODO(lietk12): handle warning case
  }
  log_events_sender_.output(store_.next_log_events());
}

Backend::Status Backend::output(FrameProps::ChunkBuffer &output_buffer) {
  // Output from state synchronization
  Application::StateSegment state_segment;
  switch (synchronizer_.output(state_segment)) {
    case StateSynchronizer::OutputStatus::ok:
      break;
    case StateSynchronizer::OutputStatus::invalid_type:
      return Status::invalid;
    case StateSynchronizer::OutputStatus::waiting:
      return Status::waiting;
  }

  switch (sender_.transform(state_segment, output_buffer)) {
    case Sender::Status::ok:
      break;
    case Sender::Status::invalid_message_length:
    case Sender::Status::invalid_message_type:
    case Sender::Status::invalid_message_encoding:
    case Sender::Status::invalid_datagram_length:
    case Sender::Status::invalid_crcelement_length:
    case Sender::Status::invalid_frame_length:
    case Sender::Status::invalid_frame_encoding:
    case Sender::Status::invalid_return_code:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
  }

  return Status::ok;
}

}  // namespace Pufferfish::Driver::Serial::Backend
