/*
 * SignalSmoothing.h
 *
 *  Signal processing and denoising
 */

#pragma once

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Protocols/Application/SignalSmoothing.h"

namespace Pufferfish::Driver::BreathingCircuit {

class SensorMeasurementsSmoothers {
 public:
  using SmoothingParameters = Protocols::Application::DisplaySmoother::SmoothingParameters;

  static constexpr SmoothingParameters fio2_params{1, 0.5, 100, 100};
  static constexpr SmoothingParameters flow_params{1, 0.5, 100, 100};
  static constexpr SmoothingParameters spo2_params{1, 0.5, 100, 1000};
  static constexpr SmoothingParameters hr_params{1, 0.5, 100, 1000};

  static const uint32_t sampling_interval = 5;  // ms

  SensorMeasurementsSmoothers()
      : fio2_(sampling_interval, fio2_params),
        flow_(sampling_interval, flow_params),
        spo2_(sampling_interval, spo2_params),
        hr_(sampling_interval, hr_params) {}

  void transform(
      uint32_t current_time, const SensorMeasurements &raw, SensorMeasurements &filtered);

 private:
  Protocols::Application::DisplaySmoother fio2_;
  Protocols::Application::DisplaySmoother flow_;
  Protocols::Application::DisplaySmoother spo2_;
  Protocols::Application::DisplaySmoother hr_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
