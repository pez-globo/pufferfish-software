/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/States.h"

#include <algorithm>
#include <iterator>

// This macro is used to add a setter for a specified protobuf type with an associated
// union field and enum value. The use of a macro here complements the use of nanopb for
// generating types and code. We use a macro because it makes the code more maintainable here,
// while allowing us to ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATESEGMENT_TAGGED_SETTER(type, field) \
    template <>\
    template <>\
    void Util::TaggedUnion<Application::StateSegmentUnion, Application::MessageTypes>::set<Application::type>(\
        const Application::type &new_value) {\
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

// This macro is used to access a specified protobuf type from a union field and
// compare it. We use a macro because it makes the code more maintainable here, while allowing us to
// ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATESEGMENT_EQ_TAGGED(field, first_segment, second_segment) \
  (first_segment).value.field == (second_segment).value.field; // NOLINT(cppcoreguidelines-pro-type-union-access)
// clang-format on

namespace Pufferfish::Util {

// StateSegment
// Refer to States.h for justification of why we are using unions this way

// Measurements
STATESEGMENT_TAGGED_SETTER(SensorMeasurements, sensor_measurements)
STATESEGMENT_TAGGED_SETTER(CycleMeasurements, cycle_measurements)
// Parameters
STATESEGMENT_TAGGED_SETTER(Parameters, parameters)
STATESEGMENT_TAGGED_SETTER(ParametersRequest, parameters_request)
// Alarm Limits
STATESEGMENT_TAGGED_SETTER(AlarmLimits, alarm_limits)
STATESEGMENT_TAGGED_SETTER(AlarmLimitsRequest, alarm_limits_request)
// Log Events
STATESEGMENT_TAGGED_SETTER(ExpectedLogEvent, expected_log_event)
STATESEGMENT_TAGGED_SETTER(NextLogEvents, next_log_events)
STATESEGMENT_TAGGED_SETTER(ActiveLogEvents, active_log_events)
// Alarm Muting
STATESEGMENT_TAGGED_SETTER(AlarmMute, alarm_mute)
STATESEGMENT_TAGGED_SETTER(AlarmMuteRequest, alarm_mute_request)
// System Miscellaneous
STATESEGMENT_TAGGED_SETTER(MCUPowerStatus, mcu_power_status)
STATESEGMENT_TAGGED_SETTER(BackendConnections, backend_connections)

}  // namespace Pufferfish::Util

namespace Pufferfish::Application {

// Equality operators

template <>
bool operator==<NextLogEvents>(const NextLogEvents &first, const NextLogEvents &second) {
  if (first.next_expected != second.next_expected || first.total != second.total ||
      first.remaining != second.remaining || first.session_id != second.session_id) {
    return false;
  }

  // Check the elements array for equality
  if (first.elements_count != second.elements_count) {
    return false;
  }

  return std::equal(
      std::begin(first.elements),
      std::begin(first.elements) + first.elements_count,
      std::begin(second.elements));
}

template <>
bool operator==<ActiveLogEvents>(const ActiveLogEvents &first, const ActiveLogEvents &second) {
  if (first.id_count != second.id_count) {
    return false;
  }

  return std::equal(
      std::begin(first.id), std::begin(first.id) + first.id_count, std::begin(second.id));
}

bool operator==(const StateSegment &first, const StateSegment &second) {
  if (first.tag != second.tag) {
    return false;
  }

  switch (first.tag) {
    // Measurements
    case MessageTypes::sensor_measurements:
      return STATESEGMENT_EQ_TAGGED(sensor_measurements, first, second);
    case MessageTypes::cycle_measurements:
      return STATESEGMENT_EQ_TAGGED(cycle_measurements, first, second);
      // Parameters
    case MessageTypes::parameters:
      return STATESEGMENT_EQ_TAGGED(parameters, first, second);
    case MessageTypes::parameters_request:
      return STATESEGMENT_EQ_TAGGED(parameters_request, first, second);
    // Alarm Limits
    case MessageTypes::alarm_limits:
      return STATESEGMENT_EQ_TAGGED(alarm_limits, first, second);
    case MessageTypes::alarm_limits_request:
      return STATESEGMENT_EQ_TAGGED(alarm_limits_request, first, second);
    // Log Events
    case MessageTypes::expected_log_event:
      return STATESEGMENT_EQ_TAGGED(expected_log_event, first, second);
    case MessageTypes::next_log_events:
      return STATESEGMENT_EQ_TAGGED(next_log_events, first, second);
    case MessageTypes::active_log_events:
      return STATESEGMENT_EQ_TAGGED(active_log_events, first, second);
    // Alarm Muting
    case MessageTypes::alarm_mute:
      return STATESEGMENT_EQ_TAGGED(alarm_mute, first, second);
    case MessageTypes::alarm_mute_request:
      return STATESEGMENT_EQ_TAGGED(alarm_mute_request, first, second);
    // System Miscellaneous
    case MessageTypes::mcu_power_status:
      return STATESEGMENT_EQ_TAGGED(mcu_power_status, first, second);
    case MessageTypes::backend_connections:
      return STATESEGMENT_EQ_TAGGED(backend_connections, first, second);
    default:
      return false;
  }
}

// Store

// Backend States
// Measurements
SensorMeasurements &Store::sensor_measurements_filtered() {
  return state_segments_.sensor_measurements;
}
CycleMeasurements &Store::cycle_measurements() {
  return state_segments_.cycle_measurements;
}
// Parameters
Parameters &Store::parameters() {
  return state_segments_.parameters;
}
bool Store::has_parameters_request() const {
  return has_parameters_request_;
}
const ParametersRequest &Store::parameters_request() const {
  return state_segments_.parameters_request;
}
// Alarm Limits
AlarmLimits &Store::alarm_limits() {
  return state_segments_.alarm_limits;
}
bool Store::has_alarm_limits_request() const {
  return has_alarm_limits_request_;
}
const AlarmLimitsRequest &Store::alarm_limits_request() const {
  return state_segments_.alarm_limits_request;
}
// Log Events
const ExpectedLogEvent &Store::expected_log_event() const {
  return state_segments_.expected_log_event;
}
NextLogEvents &Store::next_log_events() {
  return state_segments_.next_log_events;
}
ActiveLogEvents &Store::active_log_events() {
  return state_segments_.active_log_events;
}
// Alarm Muting
AlarmMute &Store::alarm_mute() {
  return state_segments_.alarm_mute;
}
AlarmMuteRequest &Store::alarm_mute_request() {
  return state_segments_.alarm_mute_request;
}
// System Miscellaneous
MCUPowerStatus &Store::mcu_power_status() {
  return state_segments_.mcu_power_status;
}
const BackendConnections &Store::backend_connections() {
  return state_segments_.backend_connections;
}

// Internal States
SensorMeasurements &Store::sensor_measurements_raw() {
  return state_segments_.sensor_measurements_raw;
}
bool &Store::backend_connected() {
  return state_segments_.backend_connected;
}

Store::Status Store::input(const StateSegment &input, bool default_initialization) {
  switch (input.tag) {
    // Measurements
    case MessageTypes::sensor_measurements:
      STATESEGMENT_GET_TAGGED(sensor_measurements, input);
      return Status::ok;
    case MessageTypes::cycle_measurements:
      STATESEGMENT_GET_TAGGED(cycle_measurements, input);
      return Status::ok;
    // Parameters
    case MessageTypes::parameters:
      STATESEGMENT_GET_TAGGED(parameters, input);
      return Status::ok;
    case MessageTypes::parameters_request:
      STATESEGMENT_GET_TAGGED(parameters_request, input);
      if (!default_initialization) {
        has_parameters_request_ = true;
      }
      return Status::ok;
    // Alarm Limits
    case MessageTypes::alarm_limits:
      STATESEGMENT_GET_TAGGED(alarm_limits, input);
      return Status::ok;
    case MessageTypes::alarm_limits_request:
      STATESEGMENT_GET_TAGGED(alarm_limits_request, input);
      if (!default_initialization) {
        has_alarm_limits_request_ = true;
      }
      return Status::ok;
    // Log Events
    case MessageTypes::expected_log_event:
      STATESEGMENT_GET_TAGGED(expected_log_event, input);
      return Status::ok;
    case MessageTypes::next_log_events:
      STATESEGMENT_GET_TAGGED(next_log_events, input);
      return Status::ok;
    case MessageTypes::active_log_events:
      STATESEGMENT_GET_TAGGED(active_log_events, input);
      return Status::ok;
    // Alarm Muting
    case MessageTypes::alarm_mute:
      STATESEGMENT_GET_TAGGED(alarm_mute, input);
      return Status::ok;
    case MessageTypes::alarm_mute_request:
      STATESEGMENT_GET_TAGGED(alarm_mute_request, input);
      return Status::ok;
    // System Miscellaneous
    case MessageTypes::mcu_power_status:
      STATESEGMENT_GET_TAGGED(mcu_power_status, input);
      return Status::ok;
    case MessageTypes::backend_connections:
      STATESEGMENT_GET_TAGGED(backend_connections, input);
      return Status::ok;
    default:
      return Status::invalid_type;
  }
}

Store::Status Store::output(MessageTypes type, StateSegment &output) const {
  switch (type) {
    // Measurements
    case MessageTypes::sensor_measurements:
      output.set(state_segments_.sensor_measurements);
      return Status::ok;
    case MessageTypes::cycle_measurements:
      output.set(state_segments_.cycle_measurements);
      return Status::ok;
    // Parameters
    case MessageTypes::parameters:
      output.set(state_segments_.parameters);
      return Status::ok;
    case MessageTypes::parameters_request:
      output.set(state_segments_.parameters_request);
      return Status::ok;
    // Alarm Limits
    case MessageTypes::alarm_limits:
      output.set(state_segments_.alarm_limits);
      return Status::ok;
    case MessageTypes::alarm_limits_request:
      output.set(state_segments_.alarm_limits_request);
      return Status::ok;
    // Log Events
    case MessageTypes::expected_log_event:
      output.set(state_segments_.expected_log_event);
      return Status::ok;
    case MessageTypes::next_log_events:
      output.set(state_segments_.next_log_events);
      return Status::ok;
    case MessageTypes::active_log_events:
      output.set(state_segments_.active_log_events);
      return Status::ok;
    // Alarm Muting
    case MessageTypes::alarm_mute:
      output.set(state_segments_.alarm_mute);
      return Status::ok;
    case MessageTypes::alarm_mute_request:
      output.set(state_segments_.alarm_mute_request);
      return Status::ok;
    // System Miscellaneous
    case MessageTypes::mcu_power_status:
      output.set(state_segments_.mcu_power_status);
      return Status::ok;
    case MessageTypes::backend_connections:
      output.set(state_segments_.backend_connections);
      return Status::ok;
    default:
      return Status::invalid_type;
  }
}

}  // namespace Pufferfish::Application
