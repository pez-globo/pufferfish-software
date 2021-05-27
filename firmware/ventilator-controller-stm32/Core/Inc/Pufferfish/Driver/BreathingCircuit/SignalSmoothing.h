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

  SensorMeasurementsSmoothers()
      : fio2_(sampling_interval, 1, 0.5, 1000, 100),
        flow_(sampling_interval, 1, 0.5, 1000, 100),
        spo2_(sampling_interval, 1, 1, 200, 1000),
        hr_(sampling_interval, 1, 1, 200, 1000) {}

  void transform(
      uint32_t current_time, const SensorMeasurements &raw, SensorMeasurements &filtered);

 private:
  Application::DisplaySmoother fio2_;
  Application::DisplaySmoother flow_;
  Application::DisplaySmoother spo2_;
  Application::DisplaySmoother hr_;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
