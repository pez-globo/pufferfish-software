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

static const uint32_t mute_max_duration = 120000;  // ms

class AlarmMuteService {
 public:
  void transform(
      uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute);

 private:
  void continue_countdown(AlarmMute &alarm_mute);

  uint32_t mute_duration_ = 0;    // ms
  uint32_t mute_start_time_ = 0;  // ms

  void input_clock(uint32_t current_time);
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
