/*
 * Alarm.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Application {

class AlarmsManager {
 public:
  explicit AlarmsManager(Application::LogEventsManager &log_manager) : log_manager_(log_manager) {}

  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits);
  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type);
  void deactivate_alarm(LogEventCode alarm_code);
  IndexStatus transform(ActiveLogEvents &active_log_events) const;

 private:
  Application::LogEventsManager &log_manager_;
  Util::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems> active_alarms_;
};

}  // namespace Pufferfish::Application