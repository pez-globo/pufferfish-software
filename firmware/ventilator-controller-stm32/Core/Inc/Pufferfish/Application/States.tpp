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
  Alarms alarms;
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  Ping ping;
  Announcement announcement;
};

}  // namespace Pufferfish::Application
