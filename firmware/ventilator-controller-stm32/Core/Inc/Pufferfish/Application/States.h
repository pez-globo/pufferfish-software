/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "nanopb/pb_common.h"
#include "Pufferfish/Driver/Serial/Backend/Messages.h"
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

static const pb_msgdesc_t *message_descriptors[] = { // array index should match the type code value
    nullptr, // 0
    &Alarms_msg, // 1
    &SensorMeasurements_msg, // 2
    &CycleMeasurements_msg, // 3
    &Parameters_msg, // 4
    &ParametersRequest_msg, // 5
    &Ping_msg, // 6
    &Announcement_msg // 7
};
static const size_t num_message_descriptors =
    sizeof(message_descriptors) / sizeof(pb_msgdesc_t *);

using Message = Driver::Serial::Backend::Message<UnionMessage>;
using MessageReceiver = Driver::Serial::Backend::MessageReceiver<UnionMessage>;
using MessageSender = Driver::Serial::Backend::MessageSender<UnionMessage>;

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
