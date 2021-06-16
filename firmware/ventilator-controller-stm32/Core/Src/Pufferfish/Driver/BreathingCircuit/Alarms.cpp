/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Alarms.h"

namespace Pufferfish::Driver::BreathingCircuit {

void reset_alarms(Application::AlarmsManager &alarms_manager) {
  alarms_manager.reset_debouncers(debouncers);
  alarms_manager.reset_init_waiters(init_waiters);
  for (const auto &pair : debouncers) {
    alarms_manager.deactivate_alarm(pair.first);
  }
}
}  // namespace Pufferfish::Driver::BreathingCircuit
