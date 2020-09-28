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
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(type, field) \
    template <>\
    template <>\
    void Util::TaggedUnion<Application::StateSegmentUnion, Application::MessageTypes>::set<type>(\
        const type &new_value) {\
      tag = Application::MessageTypes::field;\
      value.field = new_value; } // NOLINT(cppcoreguidelines-pro-type-union-access)

// This macro is used to access a specified protobuf type from a union field and
// save it in the associated states field. We use a macro because it makes the code
// more maintainable here, while allowing us to ensure union tagging.
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define PF_APPLICATION_STATESEGMENT_GET_TAGGED(field, segment) \
  if ((segment).tag == MessageTypes::field) {                  \
    state_segments_.field = (segment).value.field; return; } // NOLINT(cppcoreguidelines-pro-type-union-access)

namespace Pufferfish::Util {

// StateSegment
// Refer to States.h for justification of why we are using unions this way

PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(Alarms, alarms);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(SensorMeasurements, sensor_measurements);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(CycleMeasurements, cycle_measurements);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(Parameters, parameters);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(ParametersRequest, parameters_request);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(Ping, ping);
PF_APPLICATION_STATESEGMENT_TAGGED_SETTER(Announcement, announcement);

}  // namespace Pufferfish::Util

namespace Pufferfish::Application {

// States

const ParametersRequest &States::parameters_request() const {
  return state_segments_.parameters_request;
}

Parameters &States::parameters() {
  return state_segments_.parameters;
}

SensorMeasurements &States::sensor_measurements() {
  return state_segments_.sensor_measurements;
}

CycleMeasurements &States::cycle_measurements() {
  return state_segments_.cycle_measurements;
}

void States::input(const StateSegment &input) {
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(alarms, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(sensor_measurements, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(cycle_measurements, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(parameters, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(parameters_request, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(ping, input);
  PF_APPLICATION_STATESEGMENT_GET_TAGGED(announcement, input);
}

void States::output(MessageTypes type, StateSegment &output) const {
  switch (type) {
    case MessageTypes::alarms:
      output.set(state_segments_.alarms);
      return;
    case MessageTypes::sensor_measurements:
      output.set(state_segments_.sensor_measurements);
      return;
    case MessageTypes::cycle_measurements:
      output.set(state_segments_.cycle_measurements);
      return;
    case MessageTypes::parameters:
      output.set(state_segments_.parameters);
      return;
    case MessageTypes::parameters_request:
      output.set(state_segments_.parameters_request);
      return;
    case MessageTypes::ping:
      output.set(state_segments_.ping);
      return;
    case MessageTypes::announcement:
      output.set(state_segments_.announcement);
      return;
    default:
      return;
  }
}

}  // namespace Pufferfish::Application
