/*
 * Alarms.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/Alarms.h"

namespace Pufferfish::Application {
// AlarmsManager

void AlarmsManager::activate_alarm(
    LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits) {
  size_t index_discard = 0;
  if (active_alarms_.find(alarm_code, index_discard) == IndexStatus::ok) {
    // Alarm is already active
    return;
  }

  uint32_t event_id = 0;
  LogEvent event{};
  event.code = alarm_code;
  event.type = alarm_type;
  event.alarm_limits = alarm_limits;
  event.has_alarm_limits = true;
  log_manager_.add_event(event, event_id);
  active_alarms_.insert(alarm_code, event_id);
}

void AlarmsManager::activate_alarm(LogEventCode alarm_code, LogEventType alarm_type) {
  size_t index_discard = 0;
  if (active_alarms_.find(alarm_code, index_discard) == IndexStatus::ok) {
    // Alarm is already active
    return;
  }

  uint32_t event_id = 0;
  LogEvent event{};
  event.code = alarm_code;
  event.type = alarm_type;
  log_manager_.add_event(event, event_id);
  active_alarms_.insert(alarm_code, event_id);
}

void AlarmsManager::deactivate_alarm(LogEventCode alarm_code) {
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

}  // namespace Pufferfish::Application
