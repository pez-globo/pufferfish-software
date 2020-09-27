/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Simulator.h"

namespace Pufferfish::BreathingCircuit {

Simulator::Simulator(
    const ParametersRequest &parametersRequest,
    Parameters &parameters,
    SensorMeasurements &sensorMeasurements,
    CycleMeasurements &cycleMeasurements
) :
    parametersRequest(parametersRequest),
    parameters(parameters),
    sensorMeasurements(sensorMeasurements),
    cycleMeasurements(cycleMeasurements) {}

void Simulator::update_clock(uint32_t currentTime) {
  this->currentTime = currentTime;
}

void Simulator::update_sensors() {
  if (currentTime - previousTime > sensorUpdateInterval) {
    // Parameters
    if (parametersRequest.rr > 0) {
      parameters.rr = parametersRequest.rr;
    }
    if (parametersRequest.ie > 0) {
      parameters.ie = parametersRequest.ie;
    }
    if (parametersRequest.pip > 0) {
      parameters.pip = parametersRequest.pip;
    }
    parameters.peep = parametersRequest.peep;
    parameters.fio2 = parametersRequest.fio2;
    // Timing
    previousTime = currentTime;
    sensorMeasurements.time = currentTime;
    cyclePeriod = 60000.0 / parameters.rr;
    if (currentTime - cycleStartTime > cyclePeriod) {
      cycleStartTime = currentTime;
      sensorMeasurements.flow = inspInitFlowRate;
    }
    inspPeriod = cyclePeriod / (1 + 1.0 / parameters.ie);
    // Airway
    if (currentTime - cycleStartTime < inspPeriod) {
      sensorMeasurements.paw += (
          parameters.pip - sensorMeasurements.paw
      ) * inspResponsiveness / sensorUpdateInterval;
      sensorMeasurements.flow *= (
          1 - inspFlowResponsiveness / sensorUpdateInterval
      );
    } else {
      sensorMeasurements.paw += (
          parameters.peep - sensorMeasurements.paw
      ) * expResponsiveness / sensorUpdateInterval;
      if (sensorMeasurements.flow >= 0) {
        sensorMeasurements.flow = expInitFlowRate;
      } else {
        sensorMeasurements.flow *= (
            1 - expFlowResponsiveness / sensorUpdateInterval
        );
      }
    }
    // FiO2
    sensorMeasurements.fio2 += (
        parameters.fio2 - sensorMeasurements.fio2
    ) * fio2Responsiveness / sensorUpdateInterval;
    // Cycle Measurements
    cycleMeasurements.time = currentTime;
    cycleMeasurements.rr = parameters.rr;
    cycleMeasurements.peep = parameters.peep;
    cycleMeasurements.pip = parameters.pip;
    cycleMeasurements.ip = sensorMeasurements.paw - parameters.peep;
  }
}

void Simulator::update_actuators() {}

}
