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
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Application/LogEvents.h"

namespace Pufferfish::Driver::BreathingCircuit {

Range transform_limits_range(uint32_t floor, uint32_t ceiling, Range request, Range current);
void service_limits_range(
    Range request, Range &response, const Range &allowed, LogEventCode code,
    Application::LogEventsManager &log_manager);

class AlarmLimitsService {
 public:
  static void service_fio2(
      const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager);
  static void service_spo2(
      const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager);
  static void service_hr(
      const AlarmLimitsRequest &request, AlarmLimits &response, Application::LogEventsManager &log_manager);

  virtual void transform(
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager) = 0;

 protected:
  static constexpr Range allowed_fio2{21, 100};
  static constexpr Range allowed_spo2{21, 100};
  static constexpr Range allowed_hr{0, 200};
};

class PCACAlarmLimits : public AlarmLimitsService {
 public:
  void transform(
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager) override;
};

class HFNCAlarmLimits : public AlarmLimitsService {
 public:
  void transform(
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager) override;

 private:
  static constexpr float max_flow = 80;  // L/min
};

class AlarmLimitsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimitsRequest &alarm_limits_request,
      AlarmLimits &alarm_limits,
      Application::LogEventsManager &log_manager);

 private:
  AlarmLimitsService *active_service_ = nullptr;
  PCACAlarmLimits pc_ac_;
  HFNCAlarmLimits hfnc_;
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmLimits &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
