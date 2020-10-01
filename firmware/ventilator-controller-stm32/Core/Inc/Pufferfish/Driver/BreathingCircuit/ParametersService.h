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

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

class ParametersService {
 public:
  ParametersService(const ParametersRequest &parameters_request, Parameters &parameters)
      : parameters_request_(parameters_request), parameters_(parameters) {}

  virtual void update() = 0;
  [[nodiscard]] virtual bool mode_active() const = 0;

 protected:
  [[nodiscard]] const ParametersRequest &parameters_request() const;
  [[nodiscard]] const Parameters &parameters() const;
  Parameters &parameters();

  void update_fio2();

 private:
  static constexpr float fio2_min = 21;   // % FiO2
  static constexpr float fio2_max = 100;  // % FiO2

  const ParametersRequest &parameters_request_;
  Parameters &parameters_;
};

class PCACParameters : public ParametersService {
 public:
  PCACParameters(const ParametersRequest &parameters_request, Parameters &parameters)
      : ParametersService(parameters_request, parameters) {}

  void update() override;
  [[nodiscard]] bool mode_active() const override;
};

class HFNCParameters : public ParametersService {
 public:
  HFNCParameters(const ParametersRequest &parameters_request, Parameters &parameters)
      : ParametersService(parameters_request, parameters) {}

  void update() override;
  [[nodiscard]] bool mode_active() const override;
};

class ParametersServices {
 public:
  ParametersServices(const ParametersRequest &parameters_request, Parameters &parameters)
      : parameters_request_(parameters_request),
        pc_ac_(parameters_request, parameters),
        hfnc_(parameters_request, parameters) {}

  void update();

 private:
  const ParametersRequest &parameters_request_;
  ParametersService *active_service_ = nullptr;
  PCACParameters pc_ac_;
  HFNCParameters hfnc_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
