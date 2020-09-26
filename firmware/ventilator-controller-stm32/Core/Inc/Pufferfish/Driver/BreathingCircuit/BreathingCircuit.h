/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#ifndef INC_PUFFERFISH_BREATHINGCIRCUIT_H_
#define INC_PUFFERFISH_BREATHINGCIRCUIT_H_

#include <stdint.h>
#include "Pufferfish/Protocols/Application.h"

namespace Pufferfish {

class BreathingCircuitSimulator{
public:
  BreathingCircuitSimulator(
      const ParametersRequest &parametersRequest,
      Parameters &parameters,
      SensorMeasurements &sensorMeasurements,
      CycleMeasurements &cycleMeasurements
  );

  void updateClock(uint32_t currentTime);
  void updateSensors();
  void updateActuators();

protected:
  const ParametersRequest &parametersRequest;
  Parameters &parameters;
  SensorMeasurements &sensorMeasurements;
  CycleMeasurements &cycleMeasurements;

  uint32_t currentTime;
  uint32_t previousTime;
  uint32_t cycleStartTime = 0;
  const uint32_t sensorUpdateInterval = 2; // ms
  float cyclePeriod = 2000; // ms
  float inspPeriod = 1000; // ms
  const float inspResponsiveness = 0.05; // ms
  const float expResponsiveness = 0.05; // ms
  const float inspInitFlowRate = 120; // L / min
  const float expInitFlowRate = -120; // L / min
  const float inspFlowResponsiveness = 0.02;
  const float expFlowResponsiveness = 0.02;
  const float fio2Responsiveness = 0.01; // ms
};

}

#include "BreathingCircuit.tpp"

#endif /* INC_PUFFERFISH_BREATHINGCIRCUIT_H_ */
