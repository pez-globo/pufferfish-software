/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Util/Enums.h"
#include "Pufferfish/Util/TaggedUnion.h"
#include "mcu_pb.h"

namespace Pufferfish::Application {

// Message constants
static const size_t next_log_events_max_elems = 2;
static const size_t active_log_events_max_elems = 32;

// Type tags

// To add a new message type, add it to MessageTypes, MessageTypeValues,
// and Driver::Serial::Backend::message_descriptors in this file.
// Then update StateSegmentUnion in States.tpp. Then add a setter to
// States.cpp using the STATESEGMENT_TAGGED_SETTER macro, add setters and
// getters to the States class as needed, and add switch cases to the
// States::input and States::output methods.
// To make the Backend recognize it as an input, add it to
// Driver::Serial::Backend::InputStates. To make Backend send it as an
// output, add it to Driver::Serial::Backend::state_sync_schedule.
enum class MessageTypes : uint8_t {
  unknown = 0,
  sensor_measurements = 2,
  cycle_measurements = 3,
  parameters = 4,
  parameters_request = 5,
  alarm_limits = 6,
  alarm_limits_request = 7,
  expected_log_event = 8,
  next_log_events = 9,
  active_log_events = 10,
  alarm_mute = 11,
  alarm_mute_request = 12,
  power_management = 20,
};

// MessageTypeValues should include all defined values of MessageTypes
using MessageTypeValues = Util::EnumValues<
    MessageTypes,
    MessageTypes::unknown,
    MessageTypes::sensor_measurements,
    MessageTypes::cycle_measurements,
    MessageTypes::parameters,
    MessageTypes::parameters_request,
    MessageTypes::alarm_limits,
    MessageTypes::alarm_limits_request,
    MessageTypes::expected_log_event,
    MessageTypes::next_log_events,
    MessageTypes::active_log_events,
    MessageTypes::alarm_mute,
    MessageTypes::alarm_mute_request,
    MessageTypes::power_management>;

// Since nanopb is running dynamically, we cannot have extensive compile-time type-checking.
// It's not clear how we might use variants to replace this union, since the nanopb functions
// would need access to the underlying memory in the variant, which is not publicly accessible.
// For now the State Segment class is a simple tagged union, though we don't have any compiler
// type-checking with the tags - it's enforced with the templated functions.
union StateSegmentUnion;

using StateSegment = Util::TaggedUnion<StateSegmentUnion, MessageTypes>;

struct StateSegments {
  // Backend States
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  AlarmLimits alarm_limits;
  AlarmLimitsRequest alarm_limits_request;
  ExpectedLogEvent expected_log_event;
  NextLogEvents next_log_events;
  ActiveLogEvents active_log_events;
  AlarmMute alarm_mute;
  AlarmMuteRequest alarm_mute_request;
  PowerManagement power_management;
};

class Store {
 public:
  Store() = default;
  enum class InputStatus { ok = 0, invalid_type };
  enum class OutputStatus { ok = 0, invalid_type };

  SensorMeasurements &sensor_measurements();
  CycleMeasurements &cycle_measurements();
  Parameters &parameters();
  [[nodiscard]] bool has_parameters_request() const;
  [[nodiscard]] const ParametersRequest &parameters_request() const;
  AlarmLimits &alarm_limits();
  [[nodiscard]] bool has_alarm_limits_request() const;
  [[nodiscard]] const AlarmLimitsRequest &alarm_limits_request() const;
  [[nodiscard]] const ExpectedLogEvent &expected_log_event() const;
  NextLogEvents &next_log_events();
  ActiveLogEvents &active_log_events();
  AlarmMute &alarm_mute();
  AlarmMuteRequest &alarm_mute_request();
  PowerManagement &power_management();

  InputStatus input(const StateSegment &input, bool default_initialization = false);
  OutputStatus output(MessageTypes type, StateSegment &output) const;

 private:
  StateSegments state_segments_{};
  bool has_parameters_request_ = false;
  bool has_alarm_limits_request_ = false;
};

}  // namespace Pufferfish::Application

#include "States.tpp"
