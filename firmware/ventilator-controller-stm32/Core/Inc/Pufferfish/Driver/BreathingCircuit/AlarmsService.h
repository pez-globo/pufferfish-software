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
#include "Pufferfish/Util/Containers/Array.h"
#include "Pufferfish/Util/Containers/OrderedMap.h"

namespace Pufferfish::Driver::BreathingCircuit {

using Application::AlarmLimits;
using Application::LogEventCode;
using Application::Parameters;
using Application::Range;
using Application::SensorMeasurements;

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
      Application::AlarmsManager &alarms_manager);
};

class PCACAlarms : public AlarmsService {};

class HFNCAlarms : public AlarmsService {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      Application::AlarmsManager &alarms_manager) override;
};

class AlarmsServices {
 public:
  void transform(
      const Parameters &parameters,
      const AlarmLimits &alarm_limits,
      const SensorMeasurements &sensor_measurements,
      Application::AlarmsManager &alarms_manager);

 private:
  AlarmsService *active_service_ = nullptr;
  PCACAlarms pc_ac_;
  HFNCAlarms hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
