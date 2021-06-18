/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Protocols/Application/Debouncing.h"
#include "Pufferfish/Util/Containers/Array.h"

namespace Pufferfish::Driver::Serial::Backend {

// All alarm codes for power management need to be registered in the following array:
static constexpr auto alarm_codes = Util::Containers::make_array<LogEventCode>(
    LogEventCode::LogEventCode_mcu_backend_connection_up,
    LogEventCode::LogEventCode_mcu_backend_connection_down);

class AlarmsService {
 public:
  void transform(
      bool connected,
      Application::AlarmsManager &alarms_manager,
      Application::LogEventsManager &log_manager);

 private:
  Protocols::Application::EdgeDetector edge_;
};

}  // namespace Pufferfish::Driver::Serial::Backend
