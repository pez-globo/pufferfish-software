/*
 * Sensor.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/Nonin/Sensor.h"

#include <cmath>

namespace Pufferfish::Driver::Serial::Nonin {

bool find_any_true(const Flags &measurement) {
  const auto *it = std::find(measurement.begin(), measurement.end(), true);
  return it != measurement.end();
}

// Sensor

InitializableState Sensor::setup() {
  if (prev_state_ == InitializableState::failed) {
    return InitializableState::failed;
  }

  switch (device_.output(measurements_)) {
    case PacketStatus::ok:
      waiting_timer_.reset(time_.millis());
      prev_state_ = InitializableState::ok;
      return prev_state_;
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
      if (!waiting_timer_.within_timeout(time_.millis())) {
        prev_state_ = InitializableState::failed;
        return prev_state_;
      }
      break;
  }

  prev_state_ = InitializableState::setup;
  return InitializableState::setup;
}

InitializableState Sensor::output(SensorConnections &sensor_connections, float &spo2, float &hr) {
  if (prev_state_ == InitializableState::failed) {
    return InitializableState::failed;
  }

  if (prev_state_ == InitializableState::setup) {
    return InitializableState::setup;
  }

  switch (device_.output(measurements_)) {
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
      if (!waiting_timer_.within_timeout(time_.millis())) {
        prev_state_ = InitializableState::failed;
        return prev_state_;
      }
      prev_state_ = InitializableState::ok;
      return prev_state_;
    case PacketStatus::ok:
      waiting_timer_.reset(time_.millis());
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

}  // namespace Pufferfish::Driver::Serial::Nonin
