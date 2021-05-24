/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/I2C/LTC4015/Sensor.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

// Sensor

void Sensor::transform(PowerManagement &power_management) {
  // check if charger is connected
  bool charging_status = false;
  device_.read_charging_status(charging_status);

  power_management.charging = charging_status;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
