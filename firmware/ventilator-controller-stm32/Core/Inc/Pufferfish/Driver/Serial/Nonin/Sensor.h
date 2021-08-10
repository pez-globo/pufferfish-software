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
#include <cstdint>

#include "Device.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Serial::Nonin {

struct SensorConnections {
  bool sensor_disconnected;
  bool sensor_alarm;
  bool out_of_track;
};

/**
 * High-level (stateful) driver for Nonin OEM III SpO2 sensor
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device, HAL::Interfaces::Time &time) : device_(device), time_(time) {}

  InitializableState setup() override;
  InitializableState output(uint32_t current_time, float &spo2, float &hr);

 private:
  static const uint32_t measurement_timeout = 2000;  // ms

  Device &device_;
  HAL::Interfaces::Time &time_;

  Sample measurements_{};
  SensorConnections sensor_connections_{};

  Util::MsTimer waiting_timer_{measurement_timeout, 0};
};

}  // namespace Pufferfish::Driver::Serial::Nonin
