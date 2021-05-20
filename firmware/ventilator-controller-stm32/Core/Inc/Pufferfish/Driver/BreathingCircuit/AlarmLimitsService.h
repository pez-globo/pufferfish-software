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

Range transform_limits_range(int32_t floor, int32_t ceiling, Range request);
void service_limits_range(
    Range request,
    Range &response,
    const Range &allowed,
    LogEventCode code,
    Application::LogEventsManager &log_manager,
    bool log_changes);

class AlarmLimitsService {
 public:
  static const int8_t fio2_tolerance = 2;        // % FiO2
  static constexpr Range allowed_fio2{21, 100};  // % SpO2
  static constexpr Range allowed_spo2{21, 100};  // % SpO2
  static constexpr Range allowed_hr{0, 200};     // bpm

  virtual void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager,
      bool log_changes);

 private:
  static void service_fio2(
      const Parameters &parameters,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager,
      bool log_changes);
  static void service_spo2(
      const AlarmLimitsRequest &request,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager,
      bool log_changes);
  static void service_hr(
      const AlarmLimitsRequest &request,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager,
      bool log_changes);
};

class PCACAlarmLimits : public AlarmLimitsService {};

class HFNCAlarmLimits : public AlarmLimitsService {
 public:
  static const int8_t flow_tolerance = 2;                    // L/min
  static constexpr Range allowed_flow{-flow_tolerance, 80};  // L/min

  void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager,
      bool log_changes) override;

 private:
  static void service_flow(
      const Parameters &parameters,
      AlarmLimits &response,
      Application::LogEventsManager &log_manager,
      bool log_changes);
};

class AlarmLimitsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager,
      bool request_initialized);

 private:
  AlarmLimitsService *active_service_ = nullptr;
  PCACAlarmLimits pc_ac_;
  HFNCAlarmLimits hfnc_;
  bool request_initialized_ = false;
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmLimits &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
