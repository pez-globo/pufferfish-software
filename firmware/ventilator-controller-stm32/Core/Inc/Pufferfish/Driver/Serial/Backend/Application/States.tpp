/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_APPLICATION_TPP_
#define INC_PUFFERFISH_PROTOCOLS_APPLICATION_TPP_

#include "Application.h"

namespace Pufferfish { namespace Protocols {

// ApplicationStates

bool ApplicationStates::setState(const ApplicationMessage &inputMessage) {
  switch (inputMessage.type) {
  case static_cast<uint8_t>(ApplicationMessageTypes::alarms):
    alarms = inputMessage.payload.alarms;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::sensorMeasurements):
    sensorMeasurements = inputMessage.payload.sensorMeasurements;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::cycleMeasurements):
    cycleMeasurements = inputMessage.payload.cycleMeasurements;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::parameters):
    parameters = inputMessage.payload.parameters;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::parametersRequest):
    parametersRequest = inputMessage.payload.parametersRequest;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::ping):
    ping = inputMessage.payload.ping;
    return true;
  case static_cast<uint8_t>(ApplicationMessageTypes::announcement):
    announcement = inputMessage.payload.announcement;
    return true;
  }

  return false;
}

void ApplicationStates::setMessage(
    ApplicationMessageTypes type, ApplicationMessage &outputMessage
) const {
  outputMessage.type = static_cast<uint8_t>(type);
  switch (type) {
  case ApplicationMessageTypes::alarms:
    outputMessage.payload.alarms = alarms;
    return;
  case ApplicationMessageTypes::sensorMeasurements:
    outputMessage.payload.sensorMeasurements = sensorMeasurements;
    return;
  case ApplicationMessageTypes::cycleMeasurements:
    outputMessage.payload.cycleMeasurements = cycleMeasurements;
    return;
  case ApplicationMessageTypes::parameters:
    outputMessage.payload.parameters = parameters;
    return;
  case ApplicationMessageTypes::parametersRequest:
    outputMessage.payload.parametersRequest = parametersRequest;
    return;
  case ApplicationMessageTypes::ping:
    outputMessage.payload.ping = ping;
    return;
  case ApplicationMessageTypes::announcement:
    outputMessage.payload.announcement = announcement;
    return;
  }
}

// StateSynchronizer

StateSynchronizer::StateSynchronizer(ApplicationStates &allStates) :
    allStates(allStates) {}

StateSynchronizer::InputStatus
StateSynchronizer::input(uint32_t time) {
  currentTime = time;
  return InputStatus::ok;
}

StateSynchronizer::InputStatus
StateSynchronizer::input(const ApplicationMessage &inputMessage) {
  switch (inputMessage.type) {
  case static_cast<uint8_t>(ApplicationMessageTypes::parametersRequest):
  case static_cast<uint8_t>(ApplicationMessageTypes::ping):
  case static_cast<uint8_t>(ApplicationMessageTypes::announcement):
    if (!allStates.setState(inputMessage)) {
      return InputStatus::invalidType;
    }
  default:
    break;
  }

  return InputStatus::ok;
}

StateSynchronizer::OutputStatus
StateSynchronizer::output(ApplicationMessage &outputMessage) {
  if (!shouldOutput()) {
    return OutputStatus::waiting;
  }

  allStates.setMessage(outputSchedule[currentScheduleEntry].type, outputMessage);
  currentScheduleEntry = (currentScheduleEntry + 1) % stateOutputScheduleLength;
  currentScheduleEntryStartTime = currentTime;
  return OutputStatus::available;
}

bool StateSynchronizer::shouldOutput() const {
  return (
      currentTime - currentScheduleEntryStartTime
  ) >= outputSchedule[currentScheduleEntry].delay;
}


} }

#endif /* INC_PUFFERFISH_PROTOCOLS_APPLICATION_TPP_ */
