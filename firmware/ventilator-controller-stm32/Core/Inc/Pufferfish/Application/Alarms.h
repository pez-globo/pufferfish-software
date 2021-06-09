/*
 * Alarm.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Application/Debouncing.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Application {

class AlarmsManager {
 public:
  explicit AlarmsManager(Application::LogEventsManager &log_manager) : log_manager_(log_manager) {}

  void update_time(uint32_t current_time);
  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type);
  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits);
  void deactivate_alarm(LogEventCode alarm_code);
  IndexStatus transform(ActiveLogEvents &active_log_events) const;

 private:
  unsigned long current_time_;
  Application::LogEventsManager &log_manager_;
  Util::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems> active_alarms_;
  // TODO: allow for elements in debouncers_ to be empty, i.e. no debouncer for an alarm code.
  // This will need a TaggedUnion or an Optional (maybe we should make a special IntMap class
  // with size_t keys backed by an array); we could also use it for pb_message_descriptors
  std::array<Application::Debouncer, 8> debouncers_ {
      // array index should match the LogEventCode value
      Application::Debouncer(),  // fio2_too_low
      Application::Debouncer(),  // fio2_too_high
      Application::Debouncer(),  // flow_too_low
      Application::Debouncer(),  // flow_too_high
      Application::Debouncer(),  // spo2_too_low
      Application::Debouncer(),  // spo2_too_high
      Application::Debouncer(),  // hr_too_low
      Application::Debouncer()   // hr_too_high
  };

  bool is_active(LogEventCode alarm_code) const;
  bool has_debouncer(LogEventCode alarm_code) const;
  // Returns the output value from the debouncer:
  bool debounce(LogEventCode alarm_code, bool input_value);
};

}  // namespace Pufferfish::Application
