/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Events.h"

namespace Pufferfish::Protocols::Application {

// Filtered Sending

template <typename Index, typename StateSegment>
StateOutputStatus FilteredStateSender<Index, StateSegment>::output(
    Index index, StateSegment &output) const {
  if (!allowed_indices_.has(index)) {
    return StateOutputStatus::none;
  }

  return sender_.output(index, output);
}

// Event Notification Sending

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
void EventNotificationSender<Index, StateSegment, sched_size, allowed_indices_capacity>::input(
    Index index) {
  sendable_indices_.input(index);
}

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
StateOutputStatus
EventNotificationSender<Index, StateSegment, sched_size, allowed_indices_capacity>::output(
    StateSegment &output) {
  if (sendable_indices_.empty()) {
    if (!output_idle) {
      return StateOutputStatus::none;
    }
    return get_next_idle_output(output);
  }

  StateOutputStatus status = get_next_sendable_output(output);
  if (status == StateOutputStatus::none && output_idle) {
    status = get_next_idle_output(output);
  }
  return status;
}

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
StateOutputStatus
EventNotificationSender<Index, StateSegment, sched_size, allowed_indices_capacity>::
    get_next_sendable_output(StateSegment &output) {
  StateOutputStatus status = sendable_sender_.output(output);
  Index index = sendable_sender_.last_index();
  if (status == StateOutputStatus::ok) {
    sendable_indices_.erase(index);
  }
  return status;
}

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
StateOutputStatus
EventNotificationSender<Index, StateSegment, sched_size, allowed_indices_capacity>::
    get_next_idle_output(StateSegment &output) {
  StateOutputStatus status = idle_sender_.output(output);
  Index index = idle_sender_.last_index();
  if (status == StateOutputStatus::ok) {
    sendable_indices_.erase(index);
  }
  return status;
}

// State Change Event Sending

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
StateChangeEventSender<Index, StateSegment, sched_size, allowed_indices_capacity>::
    StateChangeEventSender(
        const IndexSequence &index_sequence, IndexedSender &all_states, bool output_idle)
    : all_states_(all_states), notification_sender_(index_sequence, all_states_, output_idle) {
  // Make trackable_states_ by removing duplicates from index_sequence
  Util::Containers::EnumSet<Index, allowed_indices_capacity> trackable_set;
  for (Index index : index_sequence) {
    if (!trackable_set.has(index)) {
      trackable_set.input(index);
      trackable_states_.push_back(index);
    }
  }
}

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
void StateChangeEventSender<Index, StateSegment, sched_size, allowed_indices_capacity>::input() {
  for (Index index : trackable_states_) {
    notification_sender_.input(index);
  }
}
template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
StateOutputStatus
StateChangeEventSender<Index, StateSegment, sched_size, allowed_indices_capacity>::output(
    StateSegment &output) {
  for (Index index : trackable_states_) {
    StateSegment new_state{};
    StateOutputStatus status = all_states_.output(index, new_state);
    if (status == StateOutputStatus::invalid_type) {
      return status;
    }

    if (status != StateOutputStatus::none) {
      if (!prev_states_.has(index)) {
        notification_sender_.input(index);
        prev_states_.input(index, new_state);
      } else {
        StateSegment prev_state{};
        // prev_states_ has index, so output will return IndexStatus::ok
        prev_states_.output(index, prev_state);
        if (new_state != prev_state) {
          notification_sender_.input(index);
          prev_states_.input(index, new_state);
        }
      }
    }
  }
  return notification_sender_.output(output);
}

}  // namespace Pufferfish::Protocols::Application
