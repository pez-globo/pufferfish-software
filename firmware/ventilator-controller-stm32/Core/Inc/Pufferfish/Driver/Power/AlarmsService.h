/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"

namespace Pufferfish::Driver::Power {
class AlarmsService {
 public:
  void transform(
      Battery &battery,
      ActiveLogEvents &active_log_events,
      Application::AlarmsManager &alarms_manager);
};

}  // namespace Pufferfish::Driver::Power
