/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/States.h"

// This macro is used to add a setter for a specified protobuf type with an associated
// union field and enum value. The use of a macro here complements the use of nanopb for
// generating types and code. We use a macro because it makes the code more maintainable here,
// while allowing us to ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATESEGMENT_TAGGED_SETTER(type, field) \
    template <>\
    template <>\
    void Util::TaggedUnion<Application::StateSegmentUnion, Application::MessageTypes>::set<type>(\
        const type &new_value) {\
      tag = Application::MessageTypes::field;\
      value.field = new_value; } // NOLINT(cppcoreguidelines-pro-type-union-access)
// clang-format on

// This macro is used to access a specified protobuf type from a union field and
// save it in the associated states field. We use a macro because it makes the code
// more maintainable here, while allowing us to ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATESEGMENT_GET_TAGGED(field, segment) \
  state_segments_.field = (segment).value.field; // NOLINT(cppcoreguidelines-pro-type-union-access)
// clang-format on

namespace Pufferfish::Util {

// StateSegment
// Refer to States.h for justification of why we are using unions this way

STATESEGMENT_TAGGED_SETTER(SensorMeasurements, sensor_measurements)
STATESEGMENT_TAGGED_SETTER(CycleMeasurements, cycle_measurements)
STATESEGMENT_TAGGED_SETTER(Parameters, parameters)
STATESEGMENT_TAGGED_SETTER(ParametersRequest, parameters_request)
STATESEGMENT_TAGGED_SETTER(AlarmLimits, alarm_limits)
STATESEGMENT_TAGGED_SETTER(AlarmLimitsRequest, alarm_limits_request)
STATESEGMENT_TAGGED_SETTER(ExpectedLogEvent, expected_log_event)
STATESEGMENT_TAGGED_SETTER(NextLogEvents, next_log_events)
STATESEGMENT_TAGGED_SETTER(ActiveLogEvents, active_log_events)
STATESEGMENT_TAGGED_SETTER(Battery, battery)

}  // namespace Pufferfish::Util

namespace Pufferfish::Application {

// States

const ParametersRequest &States::parameters_request() const {
  return state_segments_.parameters_request;
}

Parameters &States::parameters() {
  return state_segments_.parameters;
}

const AlarmLimitsRequest &States::alarm_limits_request() const {
  return state_segments_.alarm_limits_request;
}

AlarmLimits &States::alarm_limits() {
  return state_segments_.alarm_limits;
}

SensorMeasurements &States::sensor_measurements() {
  return state_segments_.sensor_measurements;
}

CycleMeasurements &States::cycle_measurements() {
  return state_segments_.cycle_measurements;
}

const ExpectedLogEvent &States::expected_log_event() const {
  return state_segments_.expected_log_event;
}

NextLogEvents &States::next_log_events() {
  return state_segments_.next_log_events;
}

ActiveLogEvents &States::active_log_events() {
  return state_segments_.active_log_events;
}

Battery &States::battery() {
  return state_segments_.battery;
}

States::InputStatus States::input(const StateSegment &input) {
  switch (input.tag) {
    case MessageTypes::sensor_measurements:
      STATESEGMENT_GET_TAGGED(sensor_measurements, input);
      return InputStatus::ok;
    case MessageTypes::cycle_measurements:
      STATESEGMENT_GET_TAGGED(cycle_measurements, input);
      return InputStatus::ok;
    case MessageTypes::parameters:
      STATESEGMENT_GET_TAGGED(parameters, input);
      return InputStatus::ok;
    case MessageTypes::parameters_request:
      STATESEGMENT_GET_TAGGED(parameters_request, input);
      return InputStatus::ok;
    case MessageTypes::alarm_limits:
      STATESEGMENT_GET_TAGGED(alarm_limits, input);
      return InputStatus::ok;
    case MessageTypes::alarm_limits_request:
      STATESEGMENT_GET_TAGGED(alarm_limits_request, input);
      return InputStatus::ok;
    case MessageTypes::expected_log_event:
      STATESEGMENT_GET_TAGGED(expected_log_event, input);
      return InputStatus::ok;
    case MessageTypes::next_log_events:
      STATESEGMENT_GET_TAGGED(next_log_events, input);
      return InputStatus::ok;
    case MessageTypes::active_log_events:
      STATESEGMENT_GET_TAGGED(active_log_events, input);
      return InputStatus::ok;
    case MessageTypes::battery_power:
      STATESEGMENT_GET_TAGGED(battery_power, input);
      return InputStatus::ok;
    default:
      return InputStatus::invalid_type;
  }
}

States::OutputStatus States::output(MessageTypes type, StateSegment &output) const {
  switch (type) {
    case MessageTypes::sensor_measurements:
      output.set(state_segments_.sensor_measurements);
      return OutputStatus::ok;
    case MessageTypes::cycle_measurements:
      output.set(state_segments_.cycle_measurements);
      return OutputStatus::ok;
    case MessageTypes::parameters:
      output.set(state_segments_.parameters);
      return OutputStatus::ok;
    case MessageTypes::parameters_request:
      output.set(state_segments_.parameters_request);
      return OutputStatus::ok;
    case MessageTypes::alarm_limits:
      output.set(state_segments_.alarm_limits);
      return OutputStatus::ok;
    case MessageTypes::alarm_limits_request:
      output.set(state_segments_.alarm_limits_request);
      return OutputStatus::ok;
    case MessageTypes::expected_log_event:
      output.set(state_segments_.expected_log_event);
      return OutputStatus::ok;
    case MessageTypes::next_log_events:
      output.set(state_segments_.next_log_events);
      return OutputStatus::ok;
    case MessageTypes::active_log_events:
      output.set(state_segments_.active_log_events);
      return OutputStatus::ok;
    case MessageTypes::battery_power:
      output.set(state_segments_.battery_power);
      return OutputStatus::ok;
    default:
      return OutputStatus::invalid_type;
  }
}

}  // namespace Pufferfish::Application
