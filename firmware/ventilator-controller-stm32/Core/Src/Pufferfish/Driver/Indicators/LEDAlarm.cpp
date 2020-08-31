/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * LEDAlarm.cpp
 *
 *  Created on: Jun 23, 2020
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/Driver/Indicators/LEDAlarm.h"

namespace Pufferfish {
namespace Driver {
namespace Indicators {

AlarmManagerStatus LEDAlarm::update(uint32_t current_time) {
  // [0, mPeriod / 2) -> on, [mPeriod / 2, mPeriod) -> off
  uint32_t blink_duration = current_time - last_cycle_;

  // trimming the duration to be within [0, mPeriod)
  if (blink_duration > period_ || reset_) {
    // NOTE: this is only accurate in the case that update() is being called
    //  fast enough (> 1 kHz). However, it guarantees that the "on" time will
    //  always be >= mPeriod / 2
    blink_duration = 0;
    last_cycle_ = current_time;
    reset_ = false;
  }

  if (blink_duration < (period_ >> 1) || period_ == 0) {
    // on
    red_.write(out_red_);
    green_.write(out_green_);
    blue_.write(out_blue_);
  } else {
    // off
    red_.write(false);
    green_.write(false);
    blue_.write(false);
  }

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus LEDAlarm::set_alarm(AlarmStatus a) {
  reset_ = true;

  switch (a) {
    case AlarmStatus::high_priority:
      // red
      out_red_ = true;
      out_green_ = false;
      out_blue_ = false;
      // 476 ms == 2.1 Hz
      period_ = 476;
      break;
    case AlarmStatus::medium_priority:
      // red + green = yellow
      out_red_ = true;
      out_green_ = true;
      out_blue_ = false;
      // 1667 ms == 0.6 Hz
      period_ = 1667;
      break;
    case AlarmStatus::low_priority:
      // green + blue = cyan
      out_red_ = false;
      out_green_ = true;
      out_blue_ = true;
      period_ = 0;
      break;
    case AlarmStatus::technical1:
      // blue + red = purple
      out_red_ = true;
      out_green_ = false;
      out_blue_ = true;
      // 2000 ms == 0.5 Hz
      period_ = 2000;
      break;
    case AlarmStatus::technical2:
      // green
      out_red_ = false;
      out_green_ = true;
      out_blue_ = false;
      // 4000 ms == 0.25 Hz
      period_ = 4000;
      break;
    case AlarmStatus::no_alarm:
      out_red_ = false;
      out_green_ = false;
      out_blue_ = false;
      period_ = 0;
      break;
    default:
      return AlarmManagerStatus::invalid_alarm;
  }

  return AlarmManagerStatus::ok;
}

}  // namespace Indicators
}  // namespace Driver
}  // namespace Pufferfish
