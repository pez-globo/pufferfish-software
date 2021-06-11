/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmsService.h"

#include <cmath>

#include "Pufferfish/Driver/BreathingCircuit/Alarms.h"
#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::BreathingCircuit {

// AlarmsService

void AlarmsService::check_parameter(
    const Range &alarm_limits,
    float measured_value,
    LogEventCode too_low_code,
    LogEventCode too_high_code,
    Application::AlarmsManager &alarms_manager) {
  if (std::lround(measured_value) < alarm_limits.lower) {
    alarms_manager.activate_alarm(too_low_code, LogEventType::LogEventType_patient, alarm_limits);
  } else {
    alarms_manager.deactivate_alarm(too_low_code);
  }
  if (std::lround(measured_value) > alarm_limits.upper) {
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
    Application::AlarmsManager &alarms_manager) {
  if (!parameters.ventilating) {
    // ParametersService already reset the alarms in AlarmsManager, so AlarmsService just needs to
    // update active_log_events
    alarms_manager.transform(active_log_events);
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

// HFNCAlarms

void HFNCAlarms::transform(
    const Parameters &parameters,
    const AlarmLimits &alarm_limits,
    const SensorMeasurements &sensor_measurements,
    ActiveLogEvents &active_log_events,
    Application::AlarmsManager &alarms_manager) {
  AlarmsService::transform(
      parameters, alarm_limits, sensor_measurements, active_log_events, alarms_manager);
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
    Application::AlarmsManager &alarms_manager) {
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
      parameters, alarm_limits, sensor_measurements, active_log_events, alarms_manager);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
