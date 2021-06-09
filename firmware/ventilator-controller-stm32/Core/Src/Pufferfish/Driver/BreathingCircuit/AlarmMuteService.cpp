/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmMuteService.h"

#include "Pufferfish/Util/Timeouts.h"

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
    alarm_mute.remaining = mute_max_duration;
  }
}

void AlarmMuteService::continue_countdown(AlarmMute &alarm_mute) {
  alarm_mute.remaining = (mute_max_duration - mute_duration_) / 1000;
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = mute_max_duration;

  AlarmMuteRequest request{};
  request.active = false;
  request.remaining = mute_max_duration;
  request_segment.set(request);
}
}  // namespace Pufferfish::Driver::BreathingCircuit
