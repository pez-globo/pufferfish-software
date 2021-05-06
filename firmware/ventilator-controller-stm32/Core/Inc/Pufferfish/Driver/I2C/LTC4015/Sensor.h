/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 */

#pragma once

#include "Device.h"
#include "Pufferfish/Application/Alarms.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

/**
 * High-level driver for LTC4015
 */
class Sensor {
 public:
  explicit Sensor(Device &device) : device_(device) {}

  // updates the battery power charging field
  void transform(Battery &battery);

 private:
  Device device_;
};

}  // namespace Pufferfish::Driver::I2C::LTC4015
