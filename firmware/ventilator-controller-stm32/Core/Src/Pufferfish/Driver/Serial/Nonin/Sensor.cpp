/*
 * Sensor.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/Sensor.h"

#include <cmath>

namespace Pufferfish::Driver::Serial::Nonin {

// Sensor

InitializableState Sensor::setup() {
  switch (device_.output(measurements_)) {
    case PacketStatus::ok:
      waiting_timer_.reset(time_.millis());
      return InitializableState::ok;
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
      if (!waiting_timer_.within_timeout(time_.millis())) {
        return InitializableState::failed;
      }
      break;
  }

  return InitializableState::setup;
}

InitializableState Sensor::output(
    SensorConnections &sensor_connections, uint32_t current_time, float &spo2, float &hr) {
  switch (device_.output(measurements_)) {
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
      // handle error cases first
      spo2 = NAN;
      hr = NAN;
      return InitializableState::failed;
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
    case PacketStatus::ok:
      input_clock(time_.millis());
      break;
  }

  // measurements status
  sensor_connections.sensor_disconnected = find_any_true(measurements_.sensor_disconnect);
  sensor_connections.sensor_alarm = find_any_true(measurements_.sensor_alarm);
  sensor_connections.out_of_track = find_any_true(measurements_.out_of_track);

  if (measurements_.e_spo2_d == ErrorConstants::spo2_missing) {
    spo2 = NAN;
  } else {
    spo2 = measurements_.e_spo2_d;
  }

  if (measurements_.e_hr_d == ErrorConstants::hr_missing) {
    hr = NAN;
  } else {
    hr = measurements_.e_hr_d;
  }

  return InitializableState::ok;
}

static bool find_any_true(const Flags &measurement) {
  const auto *it = std::find(measurement.begin(), measurement.end(), true);
  return it != measurement.end();
}

}  // namespace Pufferfish::Driver::Serial::Nonin
