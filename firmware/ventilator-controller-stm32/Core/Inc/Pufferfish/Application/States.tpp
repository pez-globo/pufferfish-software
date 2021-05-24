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
  ExpectedLogEvent expected_log_event;
  NextLogEvents next_log_events;
  ActiveLogEvents active_log_events;
  PowerManagement power_management;
};

}  // namespace Pufferfish::Application
