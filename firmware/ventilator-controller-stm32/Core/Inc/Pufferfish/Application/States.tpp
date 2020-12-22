/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "States.h"

namespace Pufferfish::Application {

union StateSegmentUnion {
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  AlarmLimits alarm_limits;
  AlarmLimitsRequest alarm_limits_request;
};

constexpr bool States::should_input(MessageTypes type) {
  return type == MessageTypes::parameters_request || type == MessageTypes::alarm_limits_request;
}

}  // namespace Pufferfish::Application
