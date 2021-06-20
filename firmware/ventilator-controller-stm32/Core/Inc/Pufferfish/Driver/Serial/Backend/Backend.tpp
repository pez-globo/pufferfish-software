/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Backend.h"
#include "Pufferfish/Protocols/Application/Lists.h"

namespace Pufferfish::Driver::Serial::Backend {

// Synchronizers

Synchronizers::Status Synchronizers::input(const Application::StateSegment &state_segment) {
  if (!ReceivableStates::includes(state_segment.tag)) {
    return Status::invalid;
  }

  connection_timer_.reset(current_time_);
  // Input into state synchronization
  switch (store_.input(state_segment)) {
    case Application::Store::Status::ok:
      break;
    default:
      // TODO(lietk12): handle error case
      return Status::invalid;
  }

  return Status::ok;
}

void Synchronizers::update_clock(uint32_t current_time) {
  current_time_ = current_time;
}

Synchronizers::Status Synchronizers::output(Application::StateSegment &state_segment) {
  if (state_send_timer_.within_timeout(current_time_)) {
    return Status::waiting;
  }

  state_send_timer_.reset(current_time_);
  update_list_senders();
  // Output from state synchronization
  switch (state_sender_root_.output(state_segment)) {
    case Protocols::Application::StateOutputStatus::ok:
      break;
    case Protocols::Application::StateOutputStatus::none:
      return Status::waiting;
    default:
      return Status::invalid;
  }

  return Status::ok;
}

bool Synchronizers::connected() const {
  return connection_timer_.within_timeout(current_time_);
}

void Synchronizers::update_list_senders() {
  const ExpectedLogEvent &event = store_.expected_log_event();
  if (log_events_sender_.input(event.id, event.session_id) !=
      Protocols::Application::ListInputStatus::ok) {
    // TODO(lietk12): handle warning case
  }
  log_events_sender_.output(store_.next_log_events());
}

// Backend

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

  // Input into synchronizers
  switch (synchronizers_.input(message.payload)) {
    case Synchronizers::Status::ok:
      break;
    default:
      // TODO(lietk12): handle error case
      return Status::invalid;
  }

  return Status::ok;
}

void Backend::update_clock(uint32_t current_time) {
  synchronizers_.update_clock(current_time);
}

Backend::Status Backend::output(FrameProps::ChunkBuffer &output_buffer) {
  // Output from synchronizers
  Application::StateSegment state_segment;
  switch (synchronizers_.output(state_segment)) {
    case Status::ok:
      break;
    case Status::waiting:
      return Status::waiting;
    default:
      return Status::invalid;
  }

  // Transform through sender
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

bool Backend::connected() const {
  return synchronizers_.connected();
}

}  // namespace Pufferfish::Driver::Serial::Backend
