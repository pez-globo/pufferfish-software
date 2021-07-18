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

void AlarmMuteService::setup(uint32_t seq_num) {
  seq_num_ = seq_num;
}

void AlarmMuteService::transform(uint32_t current_time, bool mute, AlarmMute &alarm_mute) {
  update_internal_state(mute, current_time);
  update_response(current_time, alarm_mute);
}

void AlarmMuteService::transform(
    uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute) {
  if (alarm_mute_request.seq_num == seq_num_ + 1) {
    update_internal_state(alarm_mute_request.active, current_time);
  }
  update_response(current_time, alarm_mute);
}

void AlarmMuteService::update_internal_state(bool active, uint32_t current_time) {
  if (active == active_) {
    return;
  }

  ++seq_num_;
  active_ = active;
  if (active) {
    mute_start_time_ = current_time;
    // TODO: make log event for starting mute, including reason for starting (hardware mute, software mute)
  } else {
    mute_start_time_ = 0;
    // TODO: make log event for ending mute, including reason for ending (automatic timeout, hardware unmute, software unmute)
  }
}

void AlarmMuteService::update_response(uint32_t current_time, AlarmMute &alarm_mute) {
  alarm_mute.active = active_;
  alarm_mute.seq_num = seq_num_;
  update_remaining(current_time, alarm_mute);
  if (alarm_mute.remaining > 0) {
    return;
  }

  // Active alarm mute timed out
  update_internal_state(false, current_time);
  alarm_mute.active = active_;
  alarm_mute.seq_num = seq_num_;
  update_remaining(current_time, alarm_mute);
}

void AlarmMuteService::update_remaining(uint32_t current_time, AlarmMute &alarm_mute) {
  if (!active_) {
    alarm_mute.remaining = mute_max_duration / clock_scale;
    return;
  }

  uint32_t mute_duration = current_time - mute_start_time_;
  int64_t remaining =
      (static_cast<int64_t>(mute_max_duration) - static_cast<int64_t>(mute_duration)) /
      clock_scale;
  alarm_mute.remaining = Util::clamp<int64_t>(remaining, 0, mute_max_duration / clock_scale);
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = mute_max_duration / clock_scale;

  AlarmMuteRequest request{};
  request.active = false;
  request_segment.set(request);
}
}  // namespace Pufferfish::Application
