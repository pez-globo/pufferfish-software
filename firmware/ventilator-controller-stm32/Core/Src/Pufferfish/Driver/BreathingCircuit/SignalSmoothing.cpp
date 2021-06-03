/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/SignalSmoothing.h"

namespace Pufferfish::Driver::BreathingCircuit {

// DisplaySmoother

void SensorMeasurementsSmoothers::transform(
    uint32_t current_time, const SensorMeasurements &raw, SensorMeasurements &filtered) {
  filtered.time = current_time;
  fio2_.transform(current_time, raw.fio2, filtered.fio2);
  flow_.transform(current_time, raw.flow, filtered.flow);
  spo2_.transform(current_time, raw.spo2, filtered.spo2);
  hr_.transform(current_time, raw.hr, filtered.hr);
}

}  // namespace Pufferfish::Driver::BreathingCircuit
