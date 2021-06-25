/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Backend.h"

namespace Pufferfish::Driver::Serial::Backend {

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
