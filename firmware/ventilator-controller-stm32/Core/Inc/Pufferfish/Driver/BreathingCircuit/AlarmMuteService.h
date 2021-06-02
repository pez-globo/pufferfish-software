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

static const float countdown_time = 120;

class AlarmMuteService {
 public:
  void transform(
      uint32_t current_time, AlarmMute &alarm_mute, const AlarmMuteRequest &alarm_mute_request);
  void start_countdown(AlarmMute &alarm_mute);

 private:
  static const uint32_t sensor_update_interval = 1000;  // ms

  uint32_t current_time_ = 0;   // ms
  uint32_t previous_time_ = 0;  // ms
  uint32_t initial_time_ = 0;   // ms

  void input_clock(uint32_t current_time);
  [[nodiscard]] bool update_needed() const;
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
