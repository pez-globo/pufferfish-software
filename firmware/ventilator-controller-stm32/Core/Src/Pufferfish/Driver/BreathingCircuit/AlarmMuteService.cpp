/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/AlarmMuteService.h"

// AlarmMuteService

namespace Pufferfish::Driver::BreathingCircuit {

void AlarmMuteService::transform(
    AlarmMute &alarm_mute, const AlarmMuteRequest &alarm_mute_request) {
  alarm_mute.active = alarm_mute_request.active;
}

}  // namespace Pufferfish::Driver::BreathingCircuit
