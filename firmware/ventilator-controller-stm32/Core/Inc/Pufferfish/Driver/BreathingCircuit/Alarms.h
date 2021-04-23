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

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Driver::BreathingCircuit {

// All alarm codes need to be registered in the following array:
static constexpr auto alarm_codes = Util::make_array<LogEventCode>(
    LogEventCode::LogEventCode_fio2_too_low,
    LogEventCode::LogEventCode_fio2_too_high,
    LogEventCode::LogEventCode_spo2_too_low,
    LogEventCode::LogEventCode_spo2_too_high,
    LogEventCode::LogEventCode_rr_too_low,
    LogEventCode::LogEventCode_rr_too_high,
    LogEventCode::LogEventCode_hr_too_low,
    LogEventCode::LogEventCode_hr_too_high);

class AlarmsManager {
 public:
  explicit AlarmsManager(Application::LogEventsManager &log_manager) : log_manager_(log_manager) {}

  void activate_alarm(LogEventCode alarm_code, LogEventType alarm_type, const Range &alarm_limits);
  void deactivate_alarm(LogEventCode alarm_code);
  IndexStatus transform(ActiveLogEvents &active_log_events) const;

 private:
  Application::LogEventsManager &log_manager_;
  Util::OrderedMap<LogEventCode, uint32_t, Application::active_log_events_max_elems> active_alarms_;
};

class AlarmsService {
 public:
  static void check_parameter(
      const Range &alarm_limits,
      float measured_value,
      LogEventCode too_low_code,
      LogEventCode too_high_code,
      AlarmsManager &alarms_manager);

  static void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      ActiveLogEvents &active_log_events,
      AlarmsManager &alarms_manager);

  static void deactivate_alarms(ActiveLogEvents &active_log_events, AlarmsManager &alarms_manager);
};

class PCACAlarms : public AlarmsService {};

class HFNCAlarms : public AlarmsService {};

class AlarmsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      ActiveLogEvents &active_log_events,
      AlarmsManager &alarms_manager);

 private:
  AlarmsService *active_service_ = nullptr;
  PCACAlarms pc_ac_;
  HFNCAlarms hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
