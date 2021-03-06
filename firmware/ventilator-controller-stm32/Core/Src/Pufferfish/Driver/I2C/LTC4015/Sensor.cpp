/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Rohan Purohit
 *
 */

#include "Pufferfish/Driver/I2C/LTC4015/Sensor.h"

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::I2C::LTC4015 {

// StateMachine

StateMachine::Action StateMachine::update() {
  switch (next_action_) {
    case Action::initialize:
      next_action_ = Action::measure;
      break;
    case Action::measure:
      break;
  }
  return next_action_;
}

// Sensor

InitializableState Sensor::setup() {
  switch (next_action_) {
    case Action::initialize:
      return initialize();
    case Action::measure:
      return InitializableState::ok;
  }
  return InitializableState::failed;
}

InitializableState Sensor::output(MCUPowerStatus &mcu_power_status) {
  switch (next_action_) {
    case Action::measure:
      return measure(mcu_power_status);
    default:
      break;
  }
  return InitializableState::failed;
}

InitializableState Sensor::initialize() {
  if (retry_count_ > max_retries_setup) {
    return InitializableState::failed;
  }

  retry_count_ = 0;
  // measure
  bool charging_status = false;
  while (device_.read_charging_status(charging_status) != I2CDeviceStatus::ok) {
    ++retry_count_;
    if (retry_count_ > max_retries_setup) {
      return InitializableState::failed;
    }
  }

  next_action_ = fsm_.update();
  retry_count_ = 0;  // reset retries to 0 for measuring
  return InitializableState::setup;
}

InitializableState Sensor::measure(MCUPowerStatus &mcu_power_status) {
  // check if charger is connected
  bool charging_status = false;
  if (device_.read_charging_status(charging_status) == I2CDeviceStatus::ok) {
    retry_count_ = 0;  // reset retries to 0 for next measurement
    mcu_power_status.charging = charging_status;
    next_action_ = fsm_.update();
    return InitializableState::ok;
  }

  ++retry_count_;
  if (retry_count_ > max_retries_measure) {
    return InitializableState::failed;
  }

  return InitializableState::ok;
}

}  // namespace Pufferfish::Driver::I2C::LTC4015
