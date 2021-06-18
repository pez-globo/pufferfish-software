/*
 * LogEvents.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Protocols/Application/Lists.h"
#include "States.h"

namespace Pufferfish::Application {

template <typename LogEventsSender>
class IncrementalEventsSender {
 public:
  using Element = typename LogEventsSender::Element;

  explicit IncrementalEventsSender(LogEventsSender &sender) : sender_(sender) {}

  void update_time(uint32_t current_time);
  // Create a new event with the provided data. The id and time fields
  // are automatically generated, and the id is an output parameter;
  // the provided event is not modified.
  void add_event(Element event, uint32_t &id);
  void add_event(const Element &event);
  void add_event(LogEventCode code, LogEventType type);

 private:
  LogEventsSender &sender_;
  uint32_t next_event_id_ = 0;
  uint32_t time_ = 0;
};

static const size_t log_events_list_buffer_len = 128;
using LogEventsSender = Protocols::Application::
    ListSender<NextLogEvents, LogEvent, log_events_list_buffer_len, next_log_events_max_elems>;
using LogEventsManager = IncrementalEventsSender<LogEventsSender>;

}  // namespace Pufferfish::Application

#include "LogEvents.tpp"
