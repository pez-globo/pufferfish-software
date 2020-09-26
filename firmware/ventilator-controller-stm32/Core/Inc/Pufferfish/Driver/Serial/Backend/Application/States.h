/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_APPLICATION_H_
#define INC_PUFFERFISH_PROTOCOLS_APPLICATION_H_

#include <stdint.h>
#include <stddef.h>

#include "Messages.h"
#include "mcu_pb.pb.h"

namespace Pufferfish { namespace Protocols {

union ApplicationUnionMessage {
  Alarms alarms;
  SensorMeasurements sensorMeasurements;
  CycleMeasurements cycleMeasurements;
  Parameters parameters;
  ParametersRequest parametersRequest;
  Ping ping;
  Announcement announcement;
};

enum class ApplicationMessageTypes : uint8_t {
  alarms = 1,
  sensorMeasurements = 2,
  cycleMeasurements = 3,
  parameters = 4,
  parametersRequest = 5,
  ping = 6,
  announcement = 7
};

const pb_msgdesc_t *applicationMessageDescriptors[] = { // array index should match the type code value
    nullptr, // 0
    &Alarms_msg, // 1
    &SensorMeasurements_msg, // 2
    &CycleMeasurements_msg, // 3
    &Parameters_msg, // 4
    &ParametersRequest_msg, // 5
    &Ping_msg, // 6
    &Announcement_msg // 7
};
static const size_t numApplicationMessageDescriptors =
    sizeof(applicationMessageDescriptors) / sizeof(pb_msgdesc_t *);

using ApplicationMessage = Message<ApplicationUnionMessage>;
using ApplicationMessageReceiver = MessageReceiver<ApplicationUnionMessage>;
using ApplicationMessageSender = MessageSender<ApplicationUnionMessage>;

class ApplicationStates {
public:
  Alarms alarms;
  SensorMeasurements sensorMeasurements;
  CycleMeasurements cycleMeasurements;
  Parameters parameters;
  ParametersRequest parametersRequest;
  Ping ping;
  Announcement announcement;

  bool setState(const ApplicationMessage &inputMessage);
  void setMessage(ApplicationMessageTypes type, ApplicationMessage &outputMessage) const;
};

struct StateOutputScheduleEntry {
  uint32_t delay;
  ApplicationMessageTypes type;
};

class StateSynchronizer {
public:
  enum class InputStatus {ok = 0, invalidType};
  enum class OutputStatus {available = 0, waiting};

  StateSynchronizer(ApplicationStates &allStates);

  ApplicationStates &allStates;

  InputStatus input(uint32_t time);
  InputStatus input(const ApplicationMessage &inputMessage);
  OutputStatus output(ApplicationMessage &outputMessage);

protected:
  uint32_t currentTime;
  const StateOutputScheduleEntry outputSchedule[9] = {
      {10, ApplicationMessageTypes::sensorMeasurements},
      {10, ApplicationMessageTypes::parameters},
      {10, ApplicationMessageTypes::alarms},
      {10, ApplicationMessageTypes::sensorMeasurements},
      {10, ApplicationMessageTypes::ping},
      {10, ApplicationMessageTypes::alarms},
      {10, ApplicationMessageTypes::sensorMeasurements},
      {10, ApplicationMessageTypes::parametersRequest},
      {10, ApplicationMessageTypes::cycleMeasurements},
  };
  const size_t stateOutputScheduleLength = (
      sizeof(outputSchedule) / sizeof(StateOutputScheduleEntry)
  );
  size_t currentScheduleEntry = 0;
  uint32_t currentScheduleEntryStartTime = 0;

  bool shouldOutput() const;
};

} }

#include "Application.tpp"

#endif /* INC_PUFFERFISH_PROTOCOLS_APPLICATION_H_ */
