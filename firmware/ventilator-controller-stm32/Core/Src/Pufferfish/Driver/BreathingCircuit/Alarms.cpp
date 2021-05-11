/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Alarms.h"

#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::BreathingCircuit {

// AlarmsManager

void AlarmsManager::activate_alarm(
    LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits) {
  size_t index_discard = 0;
  if (active_alarms_.find(alarm_code, index_discard) == IndexStatus::ok) {
    // Alarm is already active
    return;
  }

  uint32_t event_id = 0;
  LogEvent event{};
  event.code = alarm_code;
  event.type = alarm_type;
  event.alarm_limits = alarm_limits;
  event.has_alarm_limits = true;
  log_manager_.add_event(event, event_id);
  active_alarms_.insert(alarm_code, event_id);
}

void AlarmsManager::deactivate_alarm(LogEventCode alarm_code) {
  active_alarms_.erase(alarm_code);
}

IndexStatus AlarmsManager::transform(ActiveLogEvents &active_log_events) const {
  IndexStatus status = IndexStatus::ok;
  size_t num_elems = active_alarms_.size();
  if (Application::active_log_events_max_elems < active_alarms_.size()) {
    status = IndexStatus::out_of_bounds;
    num_elems = Application::active_log_events_max_elems;
  }
  active_log_events.id_count = num_elems;
  for (size_t i = 0; i < num_elems; ++i) {
    active_log_events.id[i] = active_alarms_.items()[i].second;
  }
  return status;
}

// AlarmsService

void AlarmsService::check_parameter(
    const Range &alarm_limits,
    float measured_value,
    LogEventCode too_low_code,
    LogEventCode too_high_code,
    AlarmsManager &alarms_manager) {
  if (measured_value < alarm_limits.lower) {
    alarms_manager.activate_alarm(too_low_code, LogEventType::LogEventType_patient, alarm_limits);
  } else {
    alarms_manager.deactivate_alarm(too_low_code);
  }
  if (measured_value > alarm_limits.upper) {
    alarms_manager.activate_alarm(too_high_code, LogEventType::LogEventType_patient, alarm_limits);
  } else {
    alarms_manager.deactivate_alarm(too_high_code);
  }
}

void AlarmsService::transform(
    const Parameters &parameters,
    const AlarmLimits &alarm_limits,
    const SensorMeasurements &sensor_measurements,
    ActiveLogEvents &active_log_events,
    AlarmsManager &alarms_manager) {
  if (!parameters.ventilating) {
    deactivate_alarms(active_log_events, alarms_manager);
    return;
  }

  check_parameter(
      alarm_limits.spo2,
      sensor_measurements.spo2,
      LogEventCode::LogEventCode_spo2_too_low,
      LogEventCode::LogEventCode_spo2_too_high,
      alarms_manager);
  check_parameter(
      alarm_limits.hr,
      sensor_measurements.hr,
      LogEventCode::LogEventCode_hr_too_low,
      LogEventCode::LogEventCode_hr_too_high,
      alarms_manager);
  check_parameter(
      alarm_limits.fio2,
      sensor_measurements.fio2,
      LogEventCode::LogEventCode_fio2_too_low,
      LogEventCode::LogEventCode_fio2_too_high,
      alarms_manager);

  alarms_manager.transform(active_log_events);
}

void AlarmsService::deactivate_alarms(
    ActiveLogEvents &active_log_events, AlarmsManager &alarms_manager) {
  for (size_t i = 0; i < alarm_codes.max_size(); ++i) {
    alarms_manager.deactivate_alarm(alarm_codes[i]);
  }
  alarms_manager.transform(active_log_events);
}

// HFNCAlarms

void HFNCAlarms::transform(
    const Parameters &parameters,
    const AlarmLimits &alarm_limits,
    const SensorMeasurements &sensor_measurements,
    ActiveLogEvents &active_log_events,
    AlarmsManager &alarms_manager) {
  AlarmsService::transform(
      parameters,
      alarm_limits,
      sensor_measurements,
      active_log_events,
      alarms_manager);
  if (!parameters.ventilating) {
    return;
  }

  check_parameter(
      alarm_limits.flow,
      sensor_measurements.flow,
      LogEventCode::LogEventCode_flow_too_low,
      LogEventCode::LogEventCode_flow_too_high,
      alarms_manager);

  alarms_manager.transform(active_log_events);
}

// AlarmsServices

void AlarmsServices::transform(
    const Parameters &parameters,
    const AlarmLimits &alarm_limits,
    const SensorMeasurements &sensor_measurements,
    ActiveLogEvents &active_log_events,
    AlarmsManager &alarms_manager) {
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

  active_service_->transform(
      parameters,
      alarm_limits,
      sensor_measurements,
      active_log_events,
      alarms_manager);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
