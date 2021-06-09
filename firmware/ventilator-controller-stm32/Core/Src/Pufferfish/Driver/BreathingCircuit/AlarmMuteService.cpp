/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmMuteService.h"

// AlarmMuteService

namespace Pufferfish::Driver::BreathingCircuit {

void AlarmMuteService::input_clock(uint32_t current_time) {
  if (mute_start_time_ == 0) {
    mute_start_time_ = current_time;
  }
  mute_duration_ = current_time - mute_start_time_;
}

void AlarmMuteService::transform(
    uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute) {
  alarm_mute.active = alarm_mute_request.active;
  input_clock(current_time);

  if (alarm_mute.active) {
    continue_countdown(alarm_mute);
  } else {
    mute_start_time_ = current_time;
    alarm_mute.remaining = mute_max_duration / clock_scale;
  }
}

void AlarmMuteService::continue_countdown(AlarmMute &alarm_mute) const {
  alarm_mute.remaining = (mute_max_duration - mute_duration_) / clock_scale;
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = countdown_time;

  AlarmMuteRequest request{};
  request.active = false;
  request.remaining = countdown_time;
  request_segment.set(request);
}
}  // namespace Pufferfish::Driver::BreathingCircuit
