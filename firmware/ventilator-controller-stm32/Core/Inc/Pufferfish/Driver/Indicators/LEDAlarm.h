/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * LEDAlarm.h
 *
 *  Created on: Jun 23, 2020
 *      Author: March Boonyapaluk
 */

#pragma once

#include "AlarmDevice.h"
#include "Pufferfish/HAL/HAL.h"

namespace Pufferfish {
namespace Driver {
namespace Indicators {

/**
 * Drives a regulatory RGB LED indicator
 */
class LEDAlarm : public AlarmDevice {
 public:
  /**
   * Constructs a new LEDAlarm object
   * @param red     output for the red LED
   * @param green   output for the green LED
   * @param blue    output for the blue LED
   */
  LEDAlarm(HAL::DigitalOutput &red, HAL::DigitalOutput &green,
           HAL::DigitalOutput &blue)
      : red_(red), green_(green), blue_(blue) {}

  AlarmManagerStatus update(uint32_t current_time) override;
  AlarmManagerStatus set_alarm(AlarmStatus a) override;

 private:
  HAL::DigitalOutput &red_;
  HAL::DigitalOutput &green_;
  HAL::DigitalOutput &blue_;

  bool reset_ = false;
  bool out_red_ = false, out_green_ = false, out_blue_ = false;
  uint32_t period_ = 0;
  uint32_t last_cycle_ = 0;
};

}  // namespace Indicators
}  // namespace Driver
}  // namespace Pufferfish
