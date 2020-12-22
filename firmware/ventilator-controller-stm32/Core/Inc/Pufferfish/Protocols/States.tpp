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

template <typename States, typename StateSegment, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::InputStatus
StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::input(uint32_t time) {
  current_time_ = time;
  return InputStatus::ok;
}

template <typename States, typename StateSegment, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::InputStatus
StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::input(
    const StateSegment &input) {
  if (all_states_.should_input(input.tag)) {
    all_states_.input(input);
  }

  return InputStatus::ok;
}

template <typename States, typename StateSegment, typename MessageTypes, size_t schedule_size>
typename StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::OutputStatus
StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::output(StateSegment &output) {
  if (!should_output()) {
    return OutputStatus::waiting;
  }

  all_states_.output(output_schedule_[current_schedule_entry_].type, output);
  current_schedule_entry_ = (current_schedule_entry_ + 1) % output_schedule_.size();
  current_schedule_entry_start_time_ = current_time_;
  return OutputStatus::available;
}

template <typename States, typename StateSegment, typename MessageTypes, size_t schedule_size>
bool StateSynchronizer<States, StateSegment, MessageTypes, schedule_size>::should_output() const {
  return (current_time_ - current_schedule_entry_start_time_) >=
         output_schedule_[current_schedule_entry_].delay;
}

}  // namespace Pufferfish::Protocols
