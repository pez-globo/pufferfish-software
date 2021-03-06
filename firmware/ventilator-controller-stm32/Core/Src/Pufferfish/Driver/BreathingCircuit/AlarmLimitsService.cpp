/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmLimitsService.h"

#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::BreathingCircuit {

using Application::LogEvent;
using Application::LogEventType;

// Update functions

Range transform_limits_range(int32_t floor, int32_t ceiling, Range request) {
  if (request.lower > request.upper) {
    std::swap(request.lower, request.upper);
  }
  request.lower = Util::clamp(request.lower, floor, ceiling);
  request.upper = Util::clamp(request.upper, floor, ceiling);
  return request;
}

void service_limits_range(
    Range request,
    Range allowed,
    LogEventCode code,
    bool log_changes,
    Range &response,
    Application::LogEventsManager &log_manager) {
  Range old_response;
  old_response = response;
  response = transform_limits_range(allowed.lower, allowed.upper, request);
  if (old_response.lower == response.lower && old_response.upper == response.upper) {
    return;
  }

  if (!log_changes) {
    return;
  }

  LogEvent event{};
  event.code = code;
  event.type = LogEventType::LogEventType_alarm_limits;
  event.old_range = old_response;
  event.has_old_range = true;
  event.new_range = response;
  event.has_new_range = true;
  log_manager.add_event(event);
}

// AlarmLimitsService

void AlarmLimitsService::transform(
    const Parameters &parameters,
    const AlarmLimitsRequest &alarm_limits_request,
    bool log_changes,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  service_fio2(parameters, log_changes, alarm_limits, log_manager);
  service_spo2(alarm_limits_request, log_changes, alarm_limits, log_manager);
  service_hr(alarm_limits_request, log_changes, alarm_limits, log_manager);
}

void AlarmLimitsService::service_fio2(
    const Parameters &parameters,
    bool log_changes,
    AlarmLimits &response,
    Application::LogEventsManager &log_manager) {
  Range fio2_range{};
  fio2_range.lower = parameters.fio2 - fio2_tolerance;
  fio2_range.upper = parameters.fio2 + fio2_tolerance;
  service_limits_range(
      fio2_range,
      allowed_fio2,
      LogEventCode::LogEventCode_fio2_alarm_limits_changed,
      log_changes,
      response.fio2,
      log_manager);
  response.has_fio2 = true;
}

void AlarmLimitsService::service_spo2(
    const AlarmLimitsRequest &request,
    bool log_changes,
    AlarmLimits &response,
    Application::LogEventsManager &log_manager) {
  service_limits_range(
      request.spo2,
      allowed_spo2,
      LogEventCode::LogEventCode_spo2_alarm_limits_changed,
      log_changes,
      response.spo2,
      log_manager);
  response.has_spo2 = true;
}

void AlarmLimitsService::service_hr(
    const AlarmLimitsRequest &request,
    bool log_changes,
    AlarmLimits &response,
    Application::LogEventsManager &log_manager) {
  service_limits_range(
      request.hr,
      allowed_hr,
      LogEventCode::LogEventCode_hr_alarm_limits_changed,
      log_changes,
      response.hr,
      log_manager);
  response.has_hr = true;
}

// HFNC AlarmLimits

void HFNCAlarmLimits::transform(
    const Parameters &parameters,
    const AlarmLimitsRequest &alarm_limits_request,
    bool log_changes,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  AlarmLimitsService::transform(
      parameters, alarm_limits_request, log_changes, alarm_limits, log_manager);
  service_flow(parameters, log_changes, alarm_limits, log_manager);
}

void HFNCAlarmLimits::service_flow(
    const Parameters &parameters,
    bool log_changes,
    AlarmLimits &response,
    Application::LogEventsManager &log_manager) {
  Range flow_range{};
  flow_range.lower = parameters.flow - flow_tolerance;
  flow_range.upper = parameters.flow + flow_tolerance;
  service_limits_range(
      flow_range,
      allowed_flow,
      LogEventCode::LogEventCode_flow_alarm_limits_changed,
      log_changes,
      response.flow,
      log_manager);
  response.has_flow = true;
}

// AlarmLimitsServices

void AlarmLimitsServices::transform(
    const Parameters &parameters,
    const AlarmLimitsRequest &alarm_limits_request,
    bool request_initialized,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  switch (parameters.mode) {
    case Application::VentilationMode_pc_ac:
      active_service_ = &pc_ac_;
      break;
    case Application::VentilationMode_hfnc:
      active_service_ = &hfnc_;
      break;
    default:
      active_service_ = nullptr;
      break;
  }
  if (active_service_ == nullptr) {
    return;
  }

  active_service_->transform(
      parameters, alarm_limits_request, request_initialized_, alarm_limits, log_manager);
  // Changes are logged only after the initialization request is processed
  request_initialized_ |= request_initialized;
}

// Initializers

void make_state_initializers(Application::StateSegment &request_segment, AlarmLimits &response) {
  response.has_fio2 = true;
  response.fio2.lower = allowed_fio2.lower;
  response.fio2.upper = response.fio2.lower + AlarmLimitsService::fio2_tolerance;
  response.has_flow = true;
  response.flow.lower = HFNCAlarmLimits::allowed_flow.lower;
  response.flow.upper = response.flow.lower + 2 * HFNCAlarmLimits::flow_tolerance;
  response.has_spo2 = true;
  response.spo2 = AlarmLimitsService::allowed_spo2;
  response.has_hr = true;
  response.hr = AlarmLimitsService::allowed_hr;

  AlarmLimitsRequest request{};
  request.has_fio2 = true;
  request.fio2 = response.fio2;
  request.has_flow = true;
  request.flow = response.flow;
  request.has_spo2 = true;
  request.spo2 = response.spo2;
  request.has_hr = true;
  request.hr = response.hr;
  request_segment.set(request);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
