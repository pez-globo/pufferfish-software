/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Util/Timeouts.h"
#include "States.h"

namespace Pufferfish::Protocols::Application {

// State Sending

template <typename Index, typename StateSegment, size_t senders_capacity>
StateOutputStatus MappedStateSenders<Index, StateSegment, senders_capacity>::output(
    Index index, StateSegment &output) const {
  if (!senders_.has(index) || senders_[index] == nullptr) {
    return StateOutputStatus::invalid_type;
  }

  return senders_[index]->output(output);
}

// Scheduled State Sending

template <typename Index, typename StateSegment, size_t sched_size>
StateOutputStatus SequentialStateSender<Index, StateSegment, sched_size>::output(
    StateSegment &output) {
  StateOutputStatus status = indexed_sender_.output(index_sequence_[sequence_cursor_], output);
  if (status != StateOutputStatus::ok) {
    return status;
  }

  sequence_cursor_ = (sequence_cursor_ + 1) % index_sequence_.size();
  return StateOutputStatus::ok;
}

}  // namespace Pufferfish::Protocols::Application
