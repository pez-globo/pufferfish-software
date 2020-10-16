/*
 * Sensor.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  High-level measurement driver for the FDO2.
 */

#pragma once

#include <cstddef>

#include "Device.h"
#include "Pufferfish/Driver/Initializable.h"
#include "Pufferfish/HAL/Interfaces/Time.h"
#include "Pufferfish/Types.h"

namespace Pufferfish::Driver::Serial::FDO2 {

/**
 * High-level (stateful) driver for FDO2 sensor
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device, HAL::Time &time) : device_(device), time_(time) {}

  InitializableState setup() override;
  InitializableState output(uint32_t &po2);

 private:
  static const uint32_t setup_timeout = 1000;        // ms
  static const uint32_t setup_request_timeout = 50;  // ms

  Device &device_;
  HAL::Time &time_;

  uint32_t setup_start_time_ = 0;  // ms
  uint32_t request_time_ = 0;      // ms
  bool setup_completed_ = false;

  bool check_version();
  bool start_broadcast();
};

}  // namespace Pufferfish::Driver::Serial::FDO2
