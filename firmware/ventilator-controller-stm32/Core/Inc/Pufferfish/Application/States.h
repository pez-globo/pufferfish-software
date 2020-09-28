/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Driver/Serial/Backend/Datagrams.h"
#include "Pufferfish/Driver/Serial/Backend/Frames.h"
#include "Pufferfish/Protocols/Messages.h"
#include "mcu_pb.h"

namespace Pufferfish::Application {

// Since nanopb is running dynamically, we cannot have extensive compile-time type-checking.
// It's not clear how we might use variants to replace this union, since the nanopb functions
// would need access to the underlying memory in the variant, which is not publicly accessible.
// For now the Message class is acting as an ad hoc tagged union here, though we don't have
// any type-checking with the tags.
union UnionMessage {
  Alarms alarms;
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  Ping ping;
  Announcement announcement;
};

using Message = Protocols::Message<
    UnionMessage,
    Driver::Serial::Backend::DatagramProps<
        Driver::Serial::Backend::FrameProps::frame_payload_max_size>::payload_max_size>;

enum class MessageTypes : uint8_t {
  alarms = 1,
  sensor_measurements = 2,
  cycle_measurements = 3,
  parameters = 4,
  parameters_request = 5,
  ping = 6,
  announcement = 7
};

class States {
 public:
  [[nodiscard]] constexpr const Alarms &alarms() const { return alarms_; }
  [[nodiscard]] constexpr const SensorMeasurements &sensor_measurements() const {
    return sensor_measurements_;
  }
  constexpr SensorMeasurements &sensor_measurements() { return sensor_measurements_; }
  [[nodiscard]] constexpr const CycleMeasurements &cycle_measurements() const {
    return cycle_measurements_;
  }
  constexpr CycleMeasurements &cycle_measurements() { return cycle_measurements_; }
  [[nodiscard]] constexpr const Parameters &parameters() const { return parameters_; }
  constexpr Parameters &parameters() { return parameters_; }
  [[nodiscard]] constexpr const ParametersRequest &parameters_request() const {
    return parameters_request_;
  }
  [[nodiscard]] constexpr const Ping &ping() const { return ping_; }
  [[nodiscard]] constexpr const Announcement &announcement() const { return announcement_; }

  bool set_state(const Message &input_message);
  void set_message(MessageTypes type, Message &output_message) const;

 private:
  // Backend States
  Alarms alarms_;
  SensorMeasurements sensor_measurements_;
  CycleMeasurements cycle_measurements_;
  Parameters parameters_;
  ParametersRequest parameters_request_;
  Ping ping_;
  Announcement announcement_;
};

}  // namespace Pufferfish::Application
