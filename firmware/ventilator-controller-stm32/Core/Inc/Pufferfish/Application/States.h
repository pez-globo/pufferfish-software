/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <array>
#include <tuple>
#include <cstdint>
#include <cstddef>

#include "Pufferfish/Driver/Serial/Backend/Messages.h"
#include "Pufferfish/Util/Protobuf.h"
#include "mcu_pb.h"

namespace Pufferfish::Application {

union UnionMessage {
  Alarms alarms;
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  Ping ping;
  Announcement announcement;
};

enum class MessageTypes : uint8_t {
  alarms = 1,
  sensor_measurements = 2,
  cycle_measurements = 3,
  parameters = 4,
  parameters_request = 5,
  ping = 6,
  announcement = 7
};

using ProtobufDescriptors = Util::ProtobufDescriptors<8>;

static const ProtobufDescriptors message_descriptors = {
    // array index should match the type code value
    Util::getProtobufDescriptor<Util::UnrecognizedMessage>(), // 0
    Util::getProtobufDescriptor<Alarms>(), // 1
    Util::getProtobufDescriptor<SensorMeasurements>(), // 2
    Util::getProtobufDescriptor<CycleMeasurements>(), // 3
    Util::getProtobufDescriptor<Parameters>(), // 4
    Util::getProtobufDescriptor<ParametersRequest>(), // 5
    Util::getProtobufDescriptor<Ping>(), // 6
    Util::getProtobufDescriptor<Announcement>() // 7
};

using Message = Driver::Serial::Backend::Message<UnionMessage>;
using MessageReceiver = Driver::Serial::Backend::MessageReceiver<
    UnionMessage, std::tuple_size<ProtobufDescriptors>::value>;
using MessageSender = Driver::Serial::Backend::MessageSender<
    UnionMessage, std::tuple_size<ProtobufDescriptors>::value>;

class States {
public:
  Alarms alarms;
  SensorMeasurements sensor_measurements;
  CycleMeasurements cycle_measurements;
  Parameters parameters;
  ParametersRequest parameters_request;
  Ping ping;
  Announcement announcement;

  bool setState(const Message &inputMessage);
  void setMessage(MessageTypes type, Message &outputMessage) const;
};

struct StateOutputScheduleEntry {
  uint32_t delay;
  MessageTypes type;
};

class StateSynchronizer {
public:
  enum class InputStatus {ok = 0, invalidType};
  enum class OutputStatus {available = 0, waiting};

  StateSynchronizer(States &allStates);

  States &allStates;

  InputStatus input(uint32_t time);
  InputStatus input(const Message &inputMessage);
  OutputStatus output(Message &outputMessage);

protected:
  uint32_t currentTime = 0;
  const StateOutputScheduleEntry outputSchedule[9] = {
      {10, MessageTypes::sensor_measurements},
      {10, MessageTypes::parameters},
      {10, MessageTypes::alarms},
      {10, MessageTypes::sensor_measurements},
      {10, MessageTypes::ping},
      {10, MessageTypes::alarms},
      {10, MessageTypes::sensor_measurements},
      {10, MessageTypes::parameters_request},
      {10, MessageTypes::cycle_measurements},
  };
  const size_t stateOutputScheduleLength = (
      sizeof(outputSchedule) / sizeof(StateOutputScheduleEntry)
  );
  size_t currentScheduleEntry = 0;
  uint32_t currentScheduleEntryStartTime = 0;

  bool shouldOutput() const;
};

}
