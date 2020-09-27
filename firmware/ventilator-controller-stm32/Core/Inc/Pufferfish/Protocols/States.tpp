/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "States.h"

namespace Pufferfish::Protocols {

// StateSynchronizer

template <typename States, typename Message, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, Message, MessageTypes, schedule_size>::InputStatus
StateSynchronizer<States, Message, MessageTypes, schedule_size>::input(uint32_t time) {
  current_time_ = time;
  return InputStatus::ok;
}

template <typename States, typename Message, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, Message, MessageTypes, schedule_size>::InputStatus
StateSynchronizer<States, Message, MessageTypes, schedule_size>::input(
    const Message &inputMessage) {
  switch (inputMessage.type) {
    case static_cast<uint8_t>(MessageTypes::parameters_request):
    case static_cast<uint8_t>(MessageTypes::ping):
    case static_cast<uint8_t>(MessageTypes::announcement):
      if (!all_states.set_state(inputMessage)) {
        return InputStatus::invalid_type;
      }
      break;
    default:
      break;
  }

  return InputStatus::ok;
}

template <typename States, typename Message, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, Message, MessageTypes, schedule_size>::OutputStatus
StateSynchronizer<States, Message, MessageTypes, schedule_size>::output(Message &outputMessage) {
  if (!should_output()) {
    return OutputStatus::waiting;
  }

  all_states.set_message(output_schedule_[current_schedule_entry_].type, outputMessage);
  current_schedule_entry_ = (current_schedule_entry_ + 1) % output_schedule_.size();
  current_schedule_entry_start_time_ = current_time_;
  return OutputStatus::available;
}

template <typename States, typename Message, typename MessageTypes, size_t schedule_size>
bool StateSynchronizer<States, Message, MessageTypes, schedule_size>::should_output() const {
  return (current_time_ - current_schedule_entry_start_time_) >=
         output_schedule_[current_schedule_entry_].delay;
}

}  // namespace Pufferfish::Protocols
