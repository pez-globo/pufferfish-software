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
#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Util/Containers/Array.h"

namespace Pufferfish::Driver::BreathingCircuit {

using Application::LogEventCode;
using Application::ParametersRequest;
using Application::VentilationMode;

float transform_parameter(float floor, float ceiling, float request, float current);
// Return value indicates whether the request caused a change in the response:
bool service_parameter(
    float request,
    bool log_changes,
    float &response,
    const Range &allowed,
    LogEventCode code,
    Application::LogEventsManager &log_manager);

class ParametersService {
 public:
  static void service_ventilating(
      bool request,
      bool &response,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager);
  static void service_mode(
      VentilationMode request,
      bool log_changes,
      VentilationMode &response,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager);
  static void service_fio2(
      float request,
      bool log_changes,
      float &response,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager);

  virtual void transform(
      const ParametersRequest &parameters_request,
      bool log_changes,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager) = 0;
  [[nodiscard]] virtual bool mode_active(const Parameters &parameters) const = 0;
};

class PCACParameters : public ParametersService {
 public:
  void transform(
      const ParametersRequest &parameters_request,
      bool log_changes,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager) override;
  [[nodiscard]] bool mode_active(const Parameters &parameters) const override;
};

class HFNCParameters : public ParametersService {
 public:
  static constexpr Range allowed_flow{0, 80};  // L/min

  static void service_flow(
      float request,
      bool log_changes,
      float &response,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager);

  void transform(
      const ParametersRequest &parameters_request,
      bool log_changes,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager) override;
  [[nodiscard]] bool mode_active(const Parameters &parameters) const override;
};

class ParametersServices {
 public:
  void transform(
      const ParametersRequest &parameters_request,
      bool request_initialized,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      Application::AlarmsManager &alarms_manager);

 private:
  ParametersService *active_service_ = nullptr;
  PCACParameters pc_ac_;
  HFNCParameters hfnc_;
  bool request_initialized_ = false;
};

void make_state_initializers(Application::StateSegment &request_segment, Parameters &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
