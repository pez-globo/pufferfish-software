/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/mcu_pb.h"
#include "Pufferfish/Driver/BreathingCircuit/ParametersService.h"
#include "Pufferfish/Util/Ranges.h"

namespace Pufferfish::Driver::BreathingCircuit {

// Update functions

float transform_parameter(float floor, float ceiling, float request, float current) {
  if (!Util::within(current, floor, ceiling)) {
    // Ensure that the request is within range so that it's guaranteed
    // to bring current back into range.
    request = Util::clamp(request, floor, ceiling);
  }
  if (Util::within(request, floor, ceiling)) {
    return request;
  }

  return current;
}

void service_parameter(
    float request,
    float &response,
    const Range &allowed,
    LogEventCode code,
    Application::LogEventsManager &log_manager) {
  float old_response = response;
  response = transform_parameter(allowed.lower, allowed.upper, request, response);
  if (old_response == response) {
    return;
  }

  LogEvent event;
  event.code = code;
  event.type = LogEventType::LogEventType_control;
  event.old_float = old_response;
  event.new_float = response;
  log_manager.add_event(event);
}

// ParametersService

void ParametersService::service_ventilating(
    bool request, bool &response, Application::LogEventsManager &log_manager) {
  if (response == request) {
    return;
  }

  LogEvent event;
  event.code = LogEventCode::LogEventCode_ventilation_operation_changed;
  event.type = LogEventType::LogEventType_control;
  event.old_bool = response;
  event.new_bool = request;
  log_manager.add_event(event);
  response = request;
}

void ParametersService::service_mode(
    VentilationMode request,
    VentilationMode &response,
    Application::LogEventsManager &log_manager) {
  if (response == request) {
    return;
  }

  LogEvent event;
  event.code = LogEventCode::LogEventCode_ventilation_mode_changed;
  event.type = LogEventType::LogEventType_control;
  event.old_mode = response;
  event.new_mode = request;
  log_manager.add_event(event);
  response = request;
}

void ParametersService::service_fio2(
    float request, float &response, Application::LogEventsManager &log_manager) {
  service_parameter(
      request,
      response,
      allowed_fio2,
      LogEventCode::LogEventCode_fio2_setting_changed,
      log_manager);
}

// PCAC Parameters

void PCACParameters::transform(
    const ParametersRequest &parameters_request,
    Parameters &parameters,
    Application::LogEventsManager &log_manager) {
  service_mode(parameters_request.mode, parameters.mode, log_manager);
  if (!mode_active(parameters)) {
    return;
  }

  service_ventilating(parameters_request.ventilating, parameters.ventilating, log_manager);
  if (parameters_request.rr > 0) {
    parameters.rr = parameters_request.rr;
  }
  if (parameters_request.ie > 0) {
    parameters.ie = parameters_request.ie;
  }
  if (parameters_request.pip > 0) {
    parameters.pip = parameters_request.pip;
  }
  parameters.peep = parameters_request.peep;
  service_fio2(parameters_request.fio2, parameters.fio2, log_manager);
}

bool PCACParameters::mode_active(const Parameters &parameters) const {
  return parameters.mode == VentilationMode_pc_ac;
}

// HFNC Parameters

void HFNCParameters::service_flow(
    float request, float &response, Application::LogEventsManager &log_manager) {
  service_parameter(
      request,
      response,
      allowed_flow,
      LogEventCode::LogEventCode_flow_setting_changed,
      log_manager);
}

void HFNCParameters::transform(
    const ParametersRequest &parameters_request,
    Parameters &parameters,
    Application::LogEventsManager &log_manager) {
  service_mode(parameters_request.mode, parameters.mode, log_manager);
  if (!mode_active(parameters)) {
    return;
  }

  service_ventilating(parameters_request.ventilating, parameters.ventilating, log_manager);
  service_flow(parameters_request.flow, parameters.flow, log_manager);
  service_fio2(parameters_request.fio2, parameters.fio2, log_manager);
}

bool HFNCParameters::mode_active(const Parameters &parameters) const {
  return parameters.mode == VentilationMode_hfnc;
}

// ParametersServices

void ParametersServices::transform(
    const ParametersRequest &parameters_request,
    Parameters &parameters,
    Application::LogEventsManager &log_manager) {
  switch (parameters_request.mode) {
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

  active_service_->transform(parameters_request, parameters, log_manager);
}

// Initializers

void make_state_initializers(Application::StateSegment &request_segment, Parameters &response) {
  response.fio2 = 21;
  response.flow = 0;

  ParametersRequest request;
  request.fio2 = response.fio2;
  request.flow = response.flow;
  request_segment.set(request);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
