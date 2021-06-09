/*
 * Alarms.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/Alarms.h"

namespace Pufferfish::Application {

// AlarmsManager

void AlarmsManager::update_time(uint32_t current_time) {
  current_time_ = current_time;
}

void AlarmsManager::activate_alarm(LogEventCode alarm_code, LogEventType alarm_type) {
  if (debouncers_.has(alarm_code) && !debounce(alarm_code, true)) {
    return;
  }

  if (is_active(alarm_code)) {
    return;
  }

  uint32_t event_id = 0;
  LogEvent event{};
  event.code = alarm_code;
  event.type = alarm_type;
  // Time is written by log_manager_, so we don't write it here.
  log_manager_.add_event(event, event_id);
  active_alarms_.insert(alarm_code, event_id);
}

void AlarmsManager::activate_alarm(
    LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits) {
  if (debouncers_.has(alarm_code) && !debounce(alarm_code, true)) {
    return;
  }

  if (is_active(alarm_code)) {
    return;
  }

  uint32_t event_id = 0;
  LogEvent event{};
  event.code = alarm_code;
  event.type = alarm_type;
  // Time is written by log_manager_, so we don't write it here.
  event.alarm_limits = alarm_limits;
  event.has_alarm_limits = true;
  log_manager_.add_event(event, event_id);
  active_alarms_.insert(alarm_code, event_id);
}

void AlarmsManager::deactivate_alarm(LogEventCode alarm_code) {
  if (debouncers_.has(alarm_code) && debounce(alarm_code, false)) {
    return;
  }

  active_alarms_.erase(alarm_code);
}

IndexStatus AlarmsManager::transform(ActiveLogEvents &active_log_events) const {
  IndexStatus status = IndexStatus::ok;
  size_t num_elems = active_alarms_.size();
  if (Application::active_log_events_max_elems < active_alarms_.size()) {
    status = IndexStatus::out_of_bounds;
    num_elems = Application::active_log_events_max_elems;
  }
  active_log_events.id_count = num_elems;
  for (size_t i = 0; i < num_elems; ++i) {
    active_log_events.id[i] = active_alarms_.items()[i].second;
  }
  return status;
}

bool AlarmsManager::is_active(LogEventCode alarm_code) const {
  size_t index_discard = 0;
  return active_alarms_.find(alarm_code, index_discard) == IndexStatus::ok;
}

bool AlarmsManager::debounce(LogEventCode alarm_code, bool input_value) {
  if (!debouncers_.has(alarm_code)) {
    // Always pass the input through if no debouncer exists for this alarm code
    return input_value;
  }

  bool output = false;
  // This ignores the return code of the debouncer
  debouncers_[alarm_code].transform(input_value, current_time_, output);
  return output;
}

}  // namespace Pufferfish::Application
