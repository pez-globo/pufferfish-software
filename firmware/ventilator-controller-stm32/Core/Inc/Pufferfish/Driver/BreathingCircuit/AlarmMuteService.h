/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

static const uint32_t mute_max_duration = 120000;  // ms
static const uint32_t clock_scale = 1000;          // ms to s

class AlarmMuteService {
 public:
  void transform(
      uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute);

 private:
  int64_t remaining_min_ = 0;
  int64_t remaining_max_ = 120;
  uint32_t mute_duration_ = 0;    // ms
  uint32_t mute_start_time_ = 0;  // ms

  void input_clock(uint32_t current_time);
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
