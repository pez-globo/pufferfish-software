/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Protocols/Application/Lists.h"
#include "States.h"

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
  handle_new_connections(connected());
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

void Synchronizers::handle_new_connections(bool backend_connected) {
  if (backend_connected && !prev_backend_connected_) {
    event_sender_.input();
  }
  prev_backend_connected_ = backend_connected;
}

}  // namespace Pufferfish::Driver::Serial::Backend
