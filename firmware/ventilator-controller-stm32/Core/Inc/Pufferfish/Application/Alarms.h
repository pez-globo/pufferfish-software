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
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Application {

using DebouncersInit =
    std::initializer_list<std::pair<LogEventCode, Protocols::Application::Debouncer>>;
using InitWaitersInit = std::initializer_list<std::pair<LogEventCode, Util::MsTimer>>;

class AlarmsManager {
 public:
  // TODO(lietk12): provide a way to provide multiple init lists for debouncers and init_waiters?
  explicit AlarmsManager(
      Application::LogEventsManager &log_manager,
      const DebouncersInit &debouncers,
      const InitWaitersInit &init_waiters)
      : log_manager_(log_manager), debouncers_{debouncers}, init_waiters_{init_waiters} {}

  void update_time(uint32_t current_time);

  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type);
  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits);
  void deactivate_alarm(LogEventCode alarm_code);

  void reset_debouncer(LogEventCode alarm_code);
  void reset_debouncers(const DebouncersInit &debouncers);
  void reset_init_waiter(LogEventCode alarm_code);
  void reset_init_waiters(const InitWaitersInit &init_waiters);

  IndexStatus transform(ActiveLogEvents &active_log_events) const;

 private:
  using Debouncers = Util::Containers::
      EnumMap<LogEventCode, Protocols::Application::Debouncer, _LogEventCode_MAX + 1>;
  using InitWaiters = Util::Containers::EnumMap<LogEventCode, Util::MsTimer, _LogEventCode_MAX + 1>;

  uint32_t current_time_ = 0;
  Application::LogEventsManager &log_manager_;
  Debouncers debouncers_;
  InitWaiters init_waiters_;
  Util::Containers::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems>
      active_alarms_;

  [[nodiscard]] bool is_active(LogEventCode alarm_code) const;
  // Returns the output value from the debouncer:
  bool debounced_output(LogEventCode alarm_code, bool input_value);
  // Returns whether the init waiter has completed:
  bool waiting_init(LogEventCode alarm_code);
};

}  // namespace Pufferfish::Application
