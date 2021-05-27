/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Util/OrderedMap.h"

namespace Pufferfish::Driver::BreathingCircuit {

class AlarmMuteService {
 public:
  static void transform(AlarmMute &alarm_mute, const AlarmMuteRequest &alarm_mute_request);
};

}  // namespace Pufferfish::Driver::BreathingCircuit
