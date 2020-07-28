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

AlarmManagerStatus LEDAlarm::update(uint32_t currentTime) {
  // [0, mPeriod / 2) -> on, [mPeriod / 2, mPeriod) -> off
  uint32_t blinkDuration = currentTime - mLastCycle;

  // trimming the duration to be within [0, mPeriod)
  if (blinkDuration > mPeriod || mReset) {
    // NOTE: this is only accurate in the case that update() is being called
    //  fast enough (> 1 kHz). However, it guarantees that the "on" time will
    //  always be >= mPeriod / 2
    blinkDuration = 0;
    mLastCycle = currentTime;
    mReset = false;
  }

  if (blinkDuration < (mPeriod >> 1) || mPeriod == 0) {
    // on
    mRed.write(mOutRed);
    mGreen.write(mOutGreen);
    mBlue.write(mOutBlue);
  } else {
    // off
    mRed.write(false);
    mGreen.write(false);
    mBlue.write(false);
  }

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus LEDAlarm::setAlarm(AlarmStatus a) {
  mReset = true;

  switch (a) {
    case AlarmStatus::highPriority:
      // red
      mOutRed = true;
      mOutGreen = false;
      mOutBlue = false;
      // 476 ms == 2.1 Hz
      mPeriod = 476;
      break;
    case AlarmStatus::mediumPriority:
      // red + green = yellow
      mOutRed = true;
      mOutGreen = true;
      mOutBlue = false;
      // 1667 ms == 0.6 Hz
      mPeriod = 1667;
      break;
    case AlarmStatus::lowPriority:
      // green + blue = cyan
      mOutRed = false;
      mOutGreen = true;
      mOutBlue = true;
      mPeriod = 0;
      break;
    case AlarmStatus::technical1:
      // blue + red = purple
      mOutRed = true;
      mOutGreen = false;
      mOutBlue = true;
      // 2000 ms == 0.5 Hz
      mPeriod = 2000;
      break;
    case AlarmStatus::technical2:
      // green
      mOutRed = false;
      mOutGreen = true;
      mOutBlue = false;
      // 4000 ms == 0.25 Hz
      mPeriod = 4000;
      break;
    case AlarmStatus::noAlarm:
      mOutRed = false;
      mOutGreen = false;
      mOutBlue = false;
      mPeriod = 0;
      break;
    default:return AlarmManagerStatus::invalidAlarm;
  }

  return AlarmManagerStatus::ok;
}

}  // namespace Indicators
}  // namespace HAL
}  // namespace Pufferfish
