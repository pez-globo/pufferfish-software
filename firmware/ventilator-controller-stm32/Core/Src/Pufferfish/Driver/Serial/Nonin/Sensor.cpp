/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/Sensor.h"

#include <cmath>

namespace Pufferfish::Driver::Serial::Nonin {

// Sensor

InitializableState Sensor::setup() {
  // sensor is connected when we have measurements available
  switch (measure(measurements_)) {
    case InitializableState::failed:
      return InitializableState::failed;
    case InitializableState::ok:
      sensor_connections_.nonin_connected = true;
      return InitializableState::ok;
    case InitializableState::setup:
      break;
  }

  return InitializableState::setup;
}

InitializableState Sensor::output(float &spo2, float &hr) {
  if (!sensor_connections_.nonin_connected) {
    return InitializableState::failed;
  }

  if (measure(measurements_) != InitializableState::ok) {
    spo2 = NAN;
    hr = NAN;
    return InitializableState::failed;
  }

  // measurements status
  sensor_connections_.sensor_disconnected = find(measurements_.sensor_disconnect, false);
  sensor_connections_.sensor_alarm = find(measurements_.sensor_alarm, true);
  sensor_connections_.spo2_out_of_track = (measurements_.e_spo2 == ErrorConstants::spo2_missing ||
                                      measurements_.e_spo2_d == ErrorConstants::spo2_missing) &&
                                     find(measurements_.out_of_track, true);
  sensor_connections_.hr_out_of_track = (measurements_.e_hr == ErrorConstants::hr_missing ||
                                      measurements_.e_spo2_d == ErrorConstants::hr_missing) &&
                                     find(measurements_.out_of_track, true);

  spo2 = measurements_.e_spo2_d;
  hr = measurements_.e_hr_d;

  return InitializableState::ok;
}

InitializableState Sensor::measure(Sample measurements) {
  switch (device_.output(measurements)) {
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
      return InitializableState::failed;
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
      return InitializableState::setup;
    case PacketStatus::ok:
      break;
  }

  return InitializableState::ok;
}

bool Sensor::find(const Flags &measurement, const bool &expected) {
  const bool *it = std::find(measurement.begin(), measurement.end(), expected);
  return it != measurement.end();
}

}  // namespace Pufferfish::Driver::Serial::Nonin
