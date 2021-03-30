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

namespace Pufferfish::Driver::BreathingCircuit {

class AlarmLimitsService {
 public:
  virtual void transform(const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) = 0;

  static void transform_limits_range(
      uint32_t floor,
      uint32_t ceiling,
      uint32_t min_request,
      uint32_t max_request,
      uint32_t &min_response,
      uint32_t &max_response);

  static void service_limits_range(
      const Range &allowed_range, const Range &request, Range &response);

 protected:
  static constexpr Range allowed_fio2{21, 100};
  static constexpr Range allowed_spo2{0, 100};
  static constexpr Range allowed_hr{0, 200};
};

class PCACAlarmLimits : public AlarmLimitsService {
 public:
  void transform(const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) override;
};

class HFNCAlarmLimits : public AlarmLimitsService {
 public:
  void transform(const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits) override;

 private:
  static constexpr float max_flow = 80;  // L/min
};

class AlarmLimitsServices {
 public:
  void transform(const Parameters &parameters, const AlarmLimitsRequest &alarm_limits_request, AlarmLimits &alarm_limits);

 private:
  AlarmLimitsService *active_service_ = nullptr;
  PCACAlarmLimits pc_ac_;
  HFNCAlarmLimits hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
