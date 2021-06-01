/*
 * SignalSmoothing.h
 *
 *  Signal processing and denoising
 */

#pragma once

#include "Pufferfish/Application/SignalSmoothing.h"
#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::BreathingCircuit {

class SensorMeasurementsSmoothers {
 public:
  static const uint32_t sampling_interval = 5;  // ms
  static constexpr Application::DisplaySmoother::SmoothingParameters fio2_params{1, 0.5, 1000, 100};
  static constexpr Application::DisplaySmoother::SmoothingParameters flow_params{1, 1, 200, 1000};
  static constexpr Application::DisplaySmoother::SmoothingParameters spo2_params{1, 1, 200, 1000};
  static constexpr Application::DisplaySmoother::SmoothingParameters hr_params{1, 1, 200, 1000};

  SensorMeasurementsSmoothers()
      : fio2_(sampling_interval, fio2_params),
        flow_(sampling_interval, flow_params),
        spo2_(sampling_interval, spo2_params),
        hr_(sampling_interval, hr_params) {}

  void transform(
      uint32_t current_time, const SensorMeasurements &raw, SensorMeasurements &filtered);

 private:
  Application::DisplaySmoother fio2_;
  Application::DisplaySmoother flow_;
  Application::DisplaySmoother spo2_;
  Application::DisplaySmoother hr_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
