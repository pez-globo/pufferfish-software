/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Driver::BreathingCircuit {

class AlarmMuteService {
 public:
  void transform(AlarmMute &alarm_mute, const AlarmMuteRequest &alarm_mute_request);

 private:
  Util::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems> active_alarms_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
