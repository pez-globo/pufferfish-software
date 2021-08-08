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
    case PacketStatus::invalid_checksum:
    case PacketStatus::invalid_header:
      return InitializableState::failed;
    case PacketStatus::waiting:
    case PacketStatus::frame_loss:
    case PacketStatus::ok:
      input_clock(time_.millis());
      break;
  }

  if (wait_time_exceeded()) {
    return InitializableState::failed;
  }

  return InitializableState::ok;
}

InitializableState Sensor::output(float &spo2, float &hr) {
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

  if (wait_time_exceeded()) {
    return InitializableState::failed;
  }

  // measurements status
  sensor_connections_.sensor_disconnected = find(measurements_.sensor_disconnect);
  sensor_connections_.sensor_alarm = find(measurements_.sensor_alarm);
  sensor_connections_.out_of_track = find(measurements_.out_of_track);

  if (measurements_.e_spo2_d == ErrorConstants::spo2_missing) {
    spo2 = NAN;
    return InitializableState::failed;
  }

  if (measurements_.e_hr_d == ErrorConstants::hr_missing) {
    hr = NAN;
    return InitializableState::failed;
  }

  spo2 = measurements_.e_spo2_d;
  hr = measurements_.e_hr_d;

  return InitializableState::ok;
}

void Sensor::input_clock(uint32_t current_time) {
  if (initial_time_ == 0) {
    initial_time_ = current_time;
  }
  if (wait_time_exceeded()) {
    waiting_timer_.reset(current_time_);
  }
  current_time_ = current_time - initial_time_;
}

bool Sensor::wait_time_exceeded() const {
  return !waiting_timer_.within_timeout(current_time_);
}

bool Sensor::find(const Flags &measurement) {
  const auto *it = std::find(measurement.begin(), measurement.end(), true);
  return it != measurement.end();
}

}  // namespace Pufferfish::Driver::Serial::Nonin
