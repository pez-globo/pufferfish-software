/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/Sensor.h"

#include <cmath>

#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Serial::Nonin {

static const uint8_t value_unavailable = 127;

// Sensor

InitializableState Sensor::setup() {
  setup_ = true;
  float spo2 = NAN;
  float hr = NAN;
  return output(spo2, hr);
}

InitializableState Sensor::output(float &spo2, float &hr) {
  if (!setup_) {
    return InitializableState::failed;
  }

  if (device_.output(measurements_) == PacketStatus::available) {
    if (measurements_.e_spo2_d == value_unavailable) {
      spo2 = NAN;
    } else {
      spo2 = measurements_.e_spo2_d;
    }
    if (measurements_.e_hr_d == value_unavailable) {
      hr = NAN;
    } else {
      hr = measurements_.e_hr_d;
    }
  }
  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::Serial::Nonin
