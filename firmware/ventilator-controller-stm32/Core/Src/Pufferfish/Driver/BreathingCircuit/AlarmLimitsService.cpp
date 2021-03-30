/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmLimitsService.h"

namespace Pufferfish::Driver::BreathingCircuit {

// AlarmLimitsService

void AlarmLimitsService::transform_limits_range(
    uint32_t floor,
    uint32_t ceiling,
    uint32_t min_request,
    uint32_t max_request,
    uint32_t &min_response,
    uint32_t &max_response) {
  if (floor <= min_request && min_request <= max_request && max_request <= ceiling) {
    min_response = min_request;
    max_response = max_request;
    return;
  }

  if (min_response < floor) {
    min_response = floor;
  }
  if (max_response > ceiling) {
    max_response = ceiling;
  }
}

void AlarmLimitsService::service_limits_range(
    const Range &allowed_range, const Range &request, Range &response) {
  transform_limits_range(
      allowed_range.lower,
      allowed_range.upper,
      request.lower,
      request.upper,
      response.lower,
      response.upper);
}

// PCAC AlarmLimits

void PCACAlarmLimits::transform(
    const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) {
  service_limits_range(allowed_fio2, alarm_limits_request.fio2, alarm_limits.fio2);
  alarm_limits.has_fio2 = true;
  service_limits_range(allowed_spo2, alarm_limits_request.spo2, alarm_limits.spo2);
  alarm_limits.has_spo2 = true;
  service_limits_range(allowed_hr, alarm_limits_request.hr, alarm_limits.hr);
  alarm_limits.has_hr = true;
}

// HFNC AlarmLimits

void HFNCAlarmLimits::transform(
    const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) {
  service_limits_range(allowed_fio2, alarm_limits_request.fio2, alarm_limits.fio2);
  alarm_limits.has_fio2 = true;
  service_limits_range(allowed_spo2, alarm_limits_request.spo2, alarm_limits.spo2);
  alarm_limits.has_spo2 = true;
  service_limits_range(allowed_hr, alarm_limits_request.hr, alarm_limits.hr);
  alarm_limits.has_hr = true;
}

// AlarmLimitsServices

void AlarmLimitsServices::transform(
    const Parameters &parameters, const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) {
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

  active_service_->transform(alarm_limits_request, alarm_limits);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
