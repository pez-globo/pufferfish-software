/*
 * Application.h
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

namespace Pufferfish::Protocols {

// State Synchronization

template<typename MessageTypes>
struct StateOutputScheduleEntry {
  uint32_t delay;
  MessageTypes type;
};

template<typename MessageTypes, size_t size>
using StateOutputSchedule = std::array<StateOutputScheduleEntry<const MessageTypes>, size>;

template<
  typename States,
  typename Message,
  typename MessageTypes,
  typename StateOutputSchedule
>
class StateSynchronizer {
public:
  enum class InputStatus {ok = 0, invalidType};
  enum class OutputStatus {available = 0, waiting};

  StateSynchronizer(States &allStates, const StateOutputSchedule &schedule) :
    allStates(allStates), outputSchedule(schedule) {}

  States &allStates;

  InputStatus input(uint32_t time);
  InputStatus input(const Message &inputMessage);
  OutputStatus output(Message &outputMessage);

protected:
  const StateOutputSchedule &outputSchedule;
  uint32_t currentTime = 0;
  size_t currentScheduleEntry = 0;
  uint32_t currentScheduleEntryStartTime = 0;

  bool shouldOutput() const;
};

}

#include "States.tpp"
