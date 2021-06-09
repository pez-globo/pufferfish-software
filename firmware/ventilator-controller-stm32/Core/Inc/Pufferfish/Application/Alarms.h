/*
 * Alarm.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <functional>

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Protocols/Application/Debouncing.h"
#include "Pufferfish/Util/Containers/EnumMap.h"
#include "Pufferfish/Util/Containers/OrderedMap.h"

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
  Util::Containers::EnumMap<
    LogEventCode,
    Protocols::Application::Debouncer,
    LogEventCode::LogEventCode_hr_too_high> debouncers_{
      {LogEventCode::LogEventCode_fio2_too_low, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_fio2_too_high, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_flow_too_low, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_flow_too_high, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_spo2_too_low, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_spo2_too_high, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_hr_too_low, Protocols::Application::Debouncer()},
      {LogEventCode::LogEventCode_hr_too_high, Protocols::Application::Debouncer()}
  };
  uint32_t current_time_ = 0;
  Application::LogEventsManager &log_manager_;
  Util::Containers::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems>
      active_alarms_;

  [[nodiscard]] bool is_active(LogEventCode alarm_code) const;
  [[nodiscard]] bool has_debouncer(LogEventCode alarm_code) const;
  // Returns the output value from the debouncer:
  bool debounce(LogEventCode alarm_code, bool input_value);
};

}  // namespace Pufferfish::Application
