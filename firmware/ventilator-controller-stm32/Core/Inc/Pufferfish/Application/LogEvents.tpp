/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "LogEvents.h"

namespace Pufferfish::Application {

template <typename ListSender>
void IncrementalEventsSender<ListSender>::update_time(uint32_t current_time) {
  time_ = current_time;
}

template <typename ListSender>
void IncrementalEventsSender<ListSender>::add_event(Element event, uint32_t &id) {
  event.id = next_event_id_;
  event.time = time_;
  Element overwritten;
  sender_.input(event, overwritten);
  // TODO(lietk12): if an event was overwritten, increment a counter somewhere to support
  // investigations
  id = event.id;
  ++next_event_id_;
}

template <typename ListSender>
void IncrementalEventsSender<ListSender>::add_event(const Element &event) {
  uint32_t id_discard = 0;
  add_event(event, id_discard);
}

template <typename ListSender>
void IncrementalEventsSender<ListSender>::add_event(LogEventCode code, LogEventType type) {
  Element event;
  event.code = code;
  event.type = type;
  add_event(event);
}

}  // namespace Pufferfish::Application
