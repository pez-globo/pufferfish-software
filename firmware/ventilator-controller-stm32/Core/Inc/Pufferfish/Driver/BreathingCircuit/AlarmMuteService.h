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

static const float countdown_time = 120;

class AlarmMuteService {
 public:
  static void transform(AlarmMute &alarm_mute, const AlarmMuteRequest &alarm_mute_request);
};

void make_state_initializers(Application::StateSegment &request_segment, AlarmMute &response);

}  // namespace Pufferfish::Driver::BreathingCircuit
