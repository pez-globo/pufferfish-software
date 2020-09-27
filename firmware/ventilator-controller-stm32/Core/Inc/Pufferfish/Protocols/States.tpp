/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "States.h"

namespace Pufferfish::Protocols {

// StateSynchronizer

template<
  typename States,
  typename Message,
  typename MessageTypes,
  typename StateOutputSchedule
>
typename StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::InputStatus
StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::input(uint32_t time) {
  currentTime = time;
  return InputStatus::ok;
}

template<
  typename States,
  typename Message,
  typename MessageTypes,
  typename StateOutputSchedule
>
typename StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::InputStatus
StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::input(const Message &inputMessage) {
  switch (inputMessage.type) {
  case static_cast<uint8_t>(MessageTypes::parameters_request):
  case static_cast<uint8_t>(MessageTypes::ping):
  case static_cast<uint8_t>(MessageTypes::announcement):
    if (!allStates.setState(inputMessage)) {
      return InputStatus::invalidType;
    }
    break;
  default:
    break;
  }

  return InputStatus::ok;
}

template<
  typename States,
  typename Message,
  typename MessageTypes,
  typename StateOutputSchedule
>
typename StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::OutputStatus
StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::output(Message &outputMessage) {
  if (!shouldOutput()) {
    return OutputStatus::waiting;
  }

  allStates.setMessage(outputSchedule[currentScheduleEntry].type, outputMessage);
  currentScheduleEntry = (currentScheduleEntry + 1) % outputSchedule.size();
  currentScheduleEntryStartTime = currentTime;
  return OutputStatus::available;
}

template<
  typename States,
  typename Message,
  typename MessageTypes,
  typename StateOutputSchedule
>
bool StateSynchronizer<States, Message, MessageTypes, StateOutputSchedule>::shouldOutput() const {
  return (
      currentTime - currentScheduleEntryStartTime
  ) >= outputSchedule[currentScheduleEntry].delay;
}


}
