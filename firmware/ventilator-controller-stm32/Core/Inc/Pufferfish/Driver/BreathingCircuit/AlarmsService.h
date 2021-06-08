/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Driver::BreathingCircuit {

// All alarm codes for BreathingCircuit need to be registered in the following array:
static constexpr auto alarm_codes = Util::make_array<LogEventCode>(
    LogEventCode::LogEventCode_spo2_too_low,
    LogEventCode::LogEventCode_spo2_too_high,
    LogEventCode::LogEventCode_hr_too_low,
    LogEventCode::LogEventCode_hr_too_high,
    LogEventCode::LogEventCode_fio2_too_low,
    LogEventCode::LogEventCode_fio2_too_high,
    LogEventCode::LogEventCode_flow_too_low,
    LogEventCode::LogEventCode_flow_too_high);

class AlarmsService {
 public:
  static void check_parameter(
      const Range &alarm_limits,
      float measured_value,
      LogEventCode too_low_code,
      LogEventCode too_high_code,
      Application::AlarmsManager &alarms_manager);

  virtual void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      ActiveLogEvents &active_log_events,
      Application::AlarmsManager &alarms_manager);

  static void deactivate_alarms(
      ActiveLogEvents &active_log_events, Application::AlarmsManager &alarms_manager);
};

class PCACAlarms : public AlarmsService {};

class HFNCAlarms : public AlarmsService {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      ActiveLogEvents &active_log_events,
      Application::AlarmsManager &alarms_manager) override;
};

class AlarmsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      ActiveLogEvents &active_log_events,
      Application::AlarmsManager &alarms_manager);

 private:
  AlarmsService *active_service_ = nullptr;
  PCACAlarms pc_ac_;
  HFNCAlarms hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
