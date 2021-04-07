/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Protocols/Lists.h"
#include "LogEvents.h"

namespace Pufferfish::Application {

template <typename ListSender>
void LogEventsManager<ListSender>::update_time(uint32_t current_time) {
  time_ = current_time;
}

template <typename ListSender>
void LogEventsManager<ListSender>::add_event(Element event, uint32_t &id) {
  event.id = next_event_id_;
  event.time = time_;
  Element overwritten;
  Protocols::ListInputStatus status = sender_.input(event, overwritten);
  // TODO(lietk12): if an event was overwritten, increment a counter somewhere to support investigations
  id = event.id;
  ++next_event_id_;
}

template <typename ListSender>
void LogEventsManager<ListSender>::add_event(const Element &event) {
  uint32_t id_discard;
  add_event(event, id_discard);
}

}  // namespace Pufferfish::Application
