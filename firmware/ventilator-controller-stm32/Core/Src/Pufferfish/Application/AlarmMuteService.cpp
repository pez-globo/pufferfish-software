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

void AlarmMuteService::transform(
    uint32_t current_time,
    bool mute,
    AlarmMuteSource source,
    AlarmMute &alarm_mute,
    Application::LogEventsManager &log_manager) {
  update_internal_state(mute, current_time, source, log_manager);
  update_response(current_time, alarm_mute, log_manager);
}

void AlarmMuteService::transform(
    uint32_t current_time,
    const AlarmMuteRequest &alarm_mute_request,
    AlarmMute &alarm_mute,
    Application::LogEventsManager &log_manager) {
  if (alarm_mute_request.seq_num == seq_num_ + 1) {
    update_internal_state(
        alarm_mute_request.active, current_time, alarm_mute_request.source, log_manager);
  }
  update_response(current_time, alarm_mute, log_manager);
}

void AlarmMuteService::update_internal_state(
    bool active,
    uint32_t current_time,
    AlarmMuteSource source,
    Application::LogEventsManager &log_manager) {
  if (active == active_) {
    return;
  }

  ++seq_num_;
  active_ = active;
  source_ = source;
  LogEventCode log_event_code{};
  if (active) {
    mute_start_time_ = current_time;
    switch (source_) {
      case Application::AlarmMuteSource_user_software:
        log_event_code = Application::LogEventCode_alarms_muted_user_software;
        break;
      case Application::AlarmMuteSource_user_hardware:
        log_event_code = Application::LogEventCode_alarms_muted_user_hardware;
        break;
      default:
        log_event_code = Application::LogEventCode_alarms_muted_unknown;
        break;
    }
  } else {
    mute_start_time_ = 0;
    switch (source_) {
      case Application::AlarmMuteSource_initialization:
        log_event_code = Application::LogEventCode_alarms_unmuted_initialization;
        break;
      case Application::AlarmMuteSource_user_software:
        log_event_code = Application::LogEventCode_alarms_unmuted_user_software;
        break;
      case Application::AlarmMuteSource_user_hardware:
        log_event_code = Application::LogEventCode_alarms_unmuted_user_hardware;
        break;
      case Application::AlarmMuteSource_timeout:
        log_event_code = Application::LogEventCode_alarms_unmuted_timeout;
        break;
      case Application::AlarmMuteSource_mcu_backend_loss:
        log_event_code = Application::LogEventCode_alarms_unmuted_mcu_backend_loss;
        break;
      case Application::AlarmMuteSource_backend_mcu_loss:
        log_event_code = Application::LogEventCode_alarms_unmuted_backend_mcu_loss;
        break;
      case Application::AlarmMuteSource_backend_frontend_loss:
        log_event_code = Application::LogEventCode_alarms_unmuted_backend_frontend_loss;
        break;
      case Application::AlarmMuteSource_frontend_backend_loss:
        log_event_code = Application::LogEventCode_alarms_unmuted_frontend_backend_loss;
        break;
      default:
        log_event_code = Application::LogEventCode_alarms_unmuted_unknown;
        break;
    }
  }
  log_manager.add_event(log_event_code, Application::LogEventType_system);
}

void AlarmMuteService::update_response(
    uint32_t current_time, AlarmMute &alarm_mute, Application::LogEventsManager &log_manager) {
  alarm_mute.active = active_;
  alarm_mute.seq_num = seq_num_;
  alarm_mute.source = source_;
  update_remaining(current_time, alarm_mute);
  if (alarm_mute.remaining > 0) {
    return;
  }

  // Active alarm mute timed out
  update_internal_state(false, current_time, Application::AlarmMuteSource_timeout, log_manager);
  alarm_mute.active = active_;
  alarm_mute.seq_num = seq_num_;
  alarm_mute.source = source_;
  update_remaining(current_time, alarm_mute);
}

void AlarmMuteService::update_remaining(uint32_t current_time, AlarmMute &alarm_mute) const {
  if (!active_) {
    alarm_mute.remaining = mute_max_duration;
    return;
  }

  uint32_t mute_duration = current_time - mute_start_time_;
  int64_t remaining =
      (static_cast<int64_t>(mute_max_duration) - static_cast<int64_t>(mute_duration));
  alarm_mute.remaining = Util::clamp<int64_t>(remaining, 0, mute_max_duration);
}

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response) {
  response.remaining = mute_max_duration;
  response.source = Application::AlarmMuteSource_initialization;

  AlarmMuteRequest request{};
  request.active = false;
  request.source = Application::AlarmMuteSource_initialization;
  request_segment.set(request);
}
}  // namespace Pufferfish::Application
