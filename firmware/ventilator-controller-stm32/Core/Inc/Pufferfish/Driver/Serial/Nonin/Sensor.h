/*
 * Sensor.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  High-level measurement driver for the Nonin OEM III.
 */

#pragma once

#include <cstddef>

#include "Device.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Types.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Serial::Nonin {

struct SensorConnections {
  bool sensor_disconnected;
  bool sensor_alarm;
  bool spo2_out_of_track;
  bool hr_out_of_track;
};

/**
 * High-level (stateful) driver for Nonin OEM III SpO2 sensor
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device, HAL::Interfaces::Time &time) : device_(device), time_(time) {}

  InitializableState setup() override;
  InitializableState output(float &spo2, float &hr);

 private:
  static const uint32_t waiting_duration = 2;  // ms
  static bool find(const Flags &measurement, const bool &expected);

  Device &device_;

  Sample measurements_{};
  SensorConnections sensor_connections_{};

  [[nodiscard]] bool wait_time_exceeded() const;

  HAL::Interfaces::Time &time_;

  void input_clock(uint32_t current_time);
  Util::MsTimer waiting_timer_{waiting_duration, 0};
  uint32_t current_time_ = 0;  // ms
  uint32_t initial_time_ = 0;  // ms
};

}  // namespace Pufferfish::Driver::Serial::Nonin
