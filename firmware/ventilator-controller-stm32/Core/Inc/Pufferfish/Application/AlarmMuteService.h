/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "States.h"

namespace Pufferfish::Application {

using Application::AlarmMute;
using Application::AlarmMuteRequest;

static const uint32_t mute_max_duration = 120000;  // ms
static const uint32_t clock_scale = 1000;          // ms to s

class AlarmMuteService {
 public:
  void setup(uint32_t seq_num);

  // Used for internal toggling of mute, bypassing the store
  void transform(uint32_t current_time, bool mute, AlarmMute &alarm_mute);
  // Used for external toggling of mute, via the store
  void transform(
      uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute);

 private:
  bool active_ = false;
  uint32_t mute_start_time_ = 0;  // ms; zero when not active, non-zero otherwise
  uint32_t seq_num_ = 0;

  void update_internal_state(bool active, uint32_t current_time);
  void update_response(uint32_t current_time, AlarmMute &alarm_mute);
  void update_remaining(uint32_t current_time, AlarmMute &alarm_mute);
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response);

}  // namespace Pufferfish::Application
