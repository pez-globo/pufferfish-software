/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmLimitsService.h"
#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::BreathingCircuit {

// Update functions

Range transform_limits_range(
    uint32_t floor, uint32_t ceiling, Range request, Range current) {
  if (current.lower > current.upper) {
    std::swap(current.lower, current.upper);
  }
  if (request.lower > request.upper) {
    std::swap(request.lower, request.upper);
  }
  if (!Util::within(current.lower, floor, current.upper) || !Util::within(current.upper, current.lower, ceiling)) {
    request.lower = Util::clamp(request.lower, floor, ceiling);
    request.upper = Util::clamp(request.upper, floor, ceiling);
  }
  if (Util::within(request.lower, floor, request.upper) && Util::within(request.upper, request.lower, ceiling)) {
    return request;
  }

  return current;
}

void service_limits_range(
    Range request, Range &response, const Range &allowed, LogEventCode code,
    Application::LogEventsManager &log_manager) {
  Range old_response;
  old_response = response;
  response = transform_limits_range(allowed.lower, allowed.upper, request, response);
  if (old_response.lower == response.lower && old_response.upper == response.upper) {
    return;
  }

  LogEvent event;
  event.code = code;
  event.type = LogEventType::LogEventType_alarm_limits;
  event.old_range = old_response;
  event.has_old_range = true;
  event.new_range = response;
  event.has_new_range = true;
  log_manager.add_event(event);
}

// AlarmLimitsService

void AlarmLimitsService::service_fio2(
    const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager) {
  service_limits_range(
    request.fio2, response.fio2, allowed_fio2,
    LogEventCode::LogEventCode_fio2_alarm_limits_changed, log_manager);
  response.has_fio2 = true;
}

void AlarmLimitsService::service_spo2(
    const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager) {
  service_limits_range(
    request.spo2, response.spo2, allowed_spo2,
    LogEventCode::LogEventCode_spo2_alarm_limits_changed, log_manager);
  response.has_spo2 = true;
}

void AlarmLimitsService::service_hr(
    const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager) {
  if (request.hr.upper == 0) {
    response.hr.upper = 200;
  }
  service_limits_range(
    request.hr, response.hr, allowed_hr,
    LogEventCode::LogEventCode_hr_alarm_limits_changed, log_manager);
  response.has_hr = true;
}

// PCAC AlarmLimits

void PCACAlarmLimits::transform(
    const AlarmLimitsRequest &alarm_limits_request,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  service_fio2(alarm_limits_request, alarm_limits, log_manager);
  service_spo2(alarm_limits_request, alarm_limits, log_manager);
  service_hr(alarm_limits_request, alarm_limits, log_manager);
}

// HFNC AlarmLimits

void HFNCAlarmLimits::transform(
    const AlarmLimitsRequest &alarm_limits_request,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  service_fio2(alarm_limits_request, alarm_limits, log_manager);
  service_spo2(alarm_limits_request, alarm_limits, log_manager);
  service_hr(alarm_limits_request, alarm_limits, log_manager);
}

// AlarmLimitsServices

void AlarmLimitsServices::transform(
    const Parameters &parameters,
    const AlarmLimitsRequest &alarm_limits_request,
    AlarmLimits &alarm_limits,
    Application::LogEventsManager &log_manager) {
  switch (parameters.mode) {
    case VentilationMode_pc_ac:
      active_service_ = &pc_ac_;
      break;
    case VentilationMode_hfnc:
      active_service_ = &hfnc_;
      break;
    default:
      active_service_ = nullptr;
      break;
  }
  if (active_service_ == nullptr) {
    return;
  }

  active_service_->transform(alarm_limits_request, alarm_limits, log_manager);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
