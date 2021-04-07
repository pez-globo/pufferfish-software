/*
 * LogEvents.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

namespace Pufferfish::Application {

template <typename LogEventsSender>
class LogEventsManager {
 public:
  using Element = typename LogEventsSender::Element;

  LogEventsManager(LogEventsSender &sender) :
    sender_(sender) {}

  void update_time(uint32_t current_time);
  // Create a new event with the provided data. The id and time fields
  // are automatically generated, and the id is an output parameter;
  // the provided event is not modified.
  void add_event(Element event, uint32_t &id);
  void add_event(const Element &event);

 private:
  LogEventsSender &sender_;
  uint32_t next_event_id_ = 0;
  uint32_t time_ = 0;
};

}  // namespace Pufferfish::Application

#include "LogEvents.tpp"
