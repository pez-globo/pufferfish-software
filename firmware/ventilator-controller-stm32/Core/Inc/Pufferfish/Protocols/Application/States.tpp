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
  for (size_t i = 0; i < index_sequence_.size(); ++i) {
    StateOutputStatus status = get_next_output(output);
    switch (status) {
      case StateOutputStatus::invalid_type:
      case StateOutputStatus::ok:
        return status;
      case StateOutputStatus::none: {
        if (!skip_unavailable) {
          return status;
        }
      }
    }
  }
  return StateOutputStatus::none;
}

template <typename Index, typename StateSegment, size_t sched_size>
Index SequentialStateSender<Index, StateSegment, sched_size>::last_index() const {
  return last_index_;
}

template <typename Index, typename StateSegment, size_t sched_size>
StateOutputStatus SequentialStateSender<Index, StateSegment, sched_size>::get_next_output(
    StateSegment &output) {
  if (index_sequence_.empty()) {
    return StateOutputStatus::none;
  }

  Index index = index_sequence_[sequence_cursor_];
  StateOutputStatus status = indexed_sender_.output(index, output);
  switch (status) {
    case StateOutputStatus::ok:
    case StateOutputStatus::none:
      break;
    case StateOutputStatus::invalid_type:
      return status;
  }

  sequence_cursor_ = (sequence_cursor_ + 1) % index_sequence_.size();
  if (status != StateOutputStatus::ok) {
    return status;
  }

  last_index_ = index;
  return StateOutputStatus::ok;
}

}  // namespace Pufferfish::Protocols::Application
