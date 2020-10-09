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

namespace Pufferfish::Driver::Serial::Nonin {

/**
 * High-level (stateful) driver for Nonin OEM III SpO2 sensor
 */
class Sensor : public Initializable {
 public:
  explicit Sensor(Device &device) : device_(device) {}

  InitializableState setup() override;
  InitializableState output(float &spo2);

 private:
  Device &device_;

  PacketMeasurements measurements_{};
};

}  // namespace Pufferfish::Driver::Serial::Nonin
