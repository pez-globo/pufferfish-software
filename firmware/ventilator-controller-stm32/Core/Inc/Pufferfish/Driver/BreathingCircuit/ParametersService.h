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

float transform_parameter(float floor, float ceiling, float request, float current);
void service_parameter(
    float request,
    float &response,
    const Range &allowed,
    LogEventCode code,
    Application::LogEventsManager &log_manager,
    bool log_changes);

class ParametersService {
 public:
  static void service_ventilating(
      bool request, bool &response, Application::LogEventsManager &log_manager);
  static void service_mode(
      VentilationMode request,
      VentilationMode &response,
      Application::LogEventsManager &log_manager,
      bool log_changes);
  static void service_fio2(
      float request, float &response, Application::LogEventsManager &log_manager, bool log_changes);

  virtual void transform(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      bool log_changes) = 0;
  [[nodiscard]] virtual bool mode_active(const Parameters &parameters) const = 0;
};

class PCACParameters : public ParametersService {
 public:
  void transform(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      bool log_changes) override;
  [[nodiscard]] bool mode_active(const Parameters &parameters) const override;
};

class HFNCParameters : public ParametersService {
 public:
  static constexpr Range allowed_flow{0, 80};  // L/min

  static void service_flow(
      float request, float &response, Application::LogEventsManager &log_manager, bool log_changes);

  void transform(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      bool log_changes) override;
  [[nodiscard]] bool mode_active(const Parameters &parameters) const override;
};

class ParametersServices {
 public:
  void transform(
      const ParametersRequest &parameters_request,
      Parameters &parameters,
      Application::LogEventsManager &log_manager,
      bool request_initialized);

 private:
  ParametersService *active_service_ = nullptr;
  PCACParameters pc_ac_;
  HFNCParameters hfnc_;
  bool request_initialized_ = false;
};

void make_state_initializers(Application::StateSegment &request_segment, Parameters &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
