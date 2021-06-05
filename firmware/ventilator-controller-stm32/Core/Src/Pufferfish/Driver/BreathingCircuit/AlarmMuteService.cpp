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
  if (initial_time_ == 0) {
    initial_time_ = current_time;
  }
  if (update_needed()) {
    previous_time_ = current_time_;
  }
  current_time_ = current_time - initial_time_;
}

void AlarmMuteService::transform(
    uint32_t current_time, const AlarmMuteRequest &alarm_mute_request, AlarmMute &alarm_mute) {
  alarm_mute.active = alarm_mute_request.active;
  input_clock(current_time);

  if (alarm_mute.active) {
    continue_countdown(alarm_mute);
  } else {
    initial_time_ = 0;
    alarm_mute.remaining = countdown_time;
  }
}

void AlarmMuteService::continue_countdown(AlarmMute &alarm_mute) {
  if (!update_needed()) {
    return;
  }
  alarm_mute.remaining = countdown_time - current_time_;
}

bool AlarmMuteService::update_needed() const {
  return !Util::within_timeout(previous_time_, sensor_update_interval, current_time_);
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = countdown_time;

  AlarmMuteRequest request{};
  request.active = false;
  request.remaining = countdown_time;
  request_segment.set(request);
}
}  // namespace Pufferfish::Driver::BreathingCircuit
