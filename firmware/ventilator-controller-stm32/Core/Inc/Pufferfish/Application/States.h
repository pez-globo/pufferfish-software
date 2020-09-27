/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Driver/Serial/Backend/Datagrams.h"
#include "Pufferfish/Protocols/Messages.h"
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

using Message = Protocols::Message<
    UnionMessage,
    Driver::Serial::Backend::Datagram::payloadMaxSize>;

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
  // Backend States
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

}
