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

#include "Controller.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

using Application::AlarmLimits;
using Application::AlarmLimitsRequest;
using Application::LogEventCode;
using Application::Parameters;
using Application::Range;

Range transform_limits_range(int32_t floor, int32_t ceiling, Range request);
void service_limits_range(
    Range request,
    Range allowed,
    LogEventCode code,
    bool log_changes,
    Range &response,
    Application::LogEventsManager &log_manager);

class AlarmLimitsService {
 public:
  static const int8_t fio2_tolerance = 2;        // % FiO2
  static constexpr Range allowed_fio2{21, 100};  // % SpO2
  static constexpr Range allowed_spo2{21, 100};  // % SpO2
  static constexpr Range allowed_hr{0, 200};     // bpm

  virtual void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      bool log_changes,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager);

 private:
  static void service_fio2(
      const Parameters &parameters,
      bool log_changes,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager);
  static void service_spo2(
      const AlarmLimitsRequest &request,
      bool log_changes,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager);
  static void service_hr(
      const AlarmLimitsRequest &request,
      bool log_changes,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager);
};

class PCACAlarmLimits : public AlarmLimitsService {};

class HFNCAlarmLimits : public AlarmLimitsService {
 public:
  static const int8_t flow_tolerance = 2;                    // L/min
  static constexpr Range allowed_flow{-flow_tolerance, 80};  // L/min

  void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      bool log_changes,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager) override;

 private:
  static void service_flow(
      const Parameters &parameters,
      bool log_changes,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager);
};

class AlarmLimitsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      bool request_initialized,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager);

 private:
  AlarmLimitsService *active_service_ = nullptr;
  PCACAlarmLimits pc_ac_;
  HFNCAlarmLimits hfnc_;
  bool request_initialized_ = false;
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmLimits &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
