/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/ParametersService.h"

namespace Pufferfish::Driver::BreathingCircuit {

// ParametersService

const ParametersRequest &ParametersService::parameters_request() const {
  return parameters_request_;
}

const Parameters &ParametersService::parameters() const {
  return parameters_;
}

Parameters &ParametersService::parameters() {
  return parameters_;
}

void ParametersService::update_fio2() {
  if (parameters_request_.fio2 >= fio2_min && parameters_request_.fio2 <= fio2_max) {
    parameters_.fio2 = parameters_request_.fio2;
  }
}

// PCAC Parameters

void PCACParameters::update() {
  parameters().mode = parameters_request().mode;
  if (!mode_active()) {
    return;
  }

  if (parameters_request().rr > 0) {
    parameters().rr = parameters_request().rr;
  }
  if (parameters_request().ie > 0) {
    parameters().ie = parameters_request().ie;
  }
  if (parameters_request().pip > 0) {
    parameters().pip = parameters_request().pip;
  }
  parameters().peep = parameters_request().peep;
  update_fio2();
}

bool PCACParameters::mode_active() const {
  return parameters().mode == VentilationMode_pc_ac;
}

// HFNC Parameters

void HFNCParameters::update() {
  parameters().mode = parameters_request().mode;
  if (!mode_active()) {
    return;
  }

  if (parameters_request().flow >= 0) {
    parameters().flow = parameters_request().flow;
  }
  update_fio2();
}

bool HFNCParameters::mode_active() const {
  return parameters().mode == VentilationMode_hfnc;
}

// ParametersServices

void ParametersServices::update() {
  switch (parameters_request_.mode) {
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

  active_service_->update();
}

}  // namespace Pufferfish::Driver::BreathingCircuit
