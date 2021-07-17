/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/AlarmMuteService.h"

#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Application {

// AlarmMuteService

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
    int64_t remaining =
        (static_cast<int64_t>(mute_max_duration) - static_cast<int64_t>(mute_duration_)) /
        clock_scale;
    alarm_mute.remaining = Util::clamp<int64_t>(remaining, remaining_min_, remaining_max_);
  } else {
    mute_start_time_ = current_time;
    alarm_mute.remaining = mute_max_duration / clock_scale;
  }
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = mute_max_duration / clock_scale;

  AlarmMuteRequest request{};
  request.active = false;
  request_segment.set(request);
}
}  // namespace Pufferfish::Application
