/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * AuditoryAlarm.cpp
 *
 *  Created on: Jun 23, 2020
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/Driver/Indicators/AuditoryAlarm.h"

namespace Pufferfish {
namespace Driver {
namespace Indicators {

AlarmManagerStatus AuditoryAlarm::update(uint32_t currentTime) {
  mRegHigh.write(mOutHigh);
  mRegMed.write(mOutMed);
  mRegLow.write(mOutLow);

  // [0, mBuzzerPulseDuty) -> on, [mBuzzerPulseDuty, mBuzzerPulsePeriod) -> off
  uint32_t pulseDuration = currentTime - mLastCycle;

  // trimming the duration to be within [0, mBuzzerPulsePeriod)
  if (pulseDuration >= mBuzzerPulsePeriod || mReset) {
    // NOTE: this is only accurate in the case that update() is being called
    //  fast enough (> 1 kHz). However, it guarantees that the "on" time will
    //  always be >= mBuzzerPulseDuty
    pulseDuration = 0;
    mLastCycle = currentTime;
    mReset = false;
    mBuzzerSwitching = true;
  }

  if (pulseDuration < mBuzzerPulseDuty) {
    // on
    mBuzzer.write(mBuzzerSwitching);
    mBuzzerSwitching = !mBuzzerSwitching;
  } else {
    // off
    mBuzzer.write(false);
  }

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus AuditoryAlarm::setAlarm(AlarmStatus a) {
  mReset = true;

  switch (a) {
    case AlarmStatus::highPriority:
      mOutHigh = true;
      mOutMed = true;
      mOutLow = true;
      mBuzzerPulsePeriod = 0;
      mBuzzerPulseDuty = 0;
      break;
    case AlarmStatus::mediumPriority:
      mOutHigh = false;
      mOutMed = true;
      mOutLow = true;
      mBuzzerPulsePeriod = 0;
      mBuzzerPulseDuty = 0;
      break;
    case AlarmStatus::lowPriority:
      mOutHigh = false;
      mOutMed = false;
      mOutLow = true;
      mBuzzerPulsePeriod = 0;
      mBuzzerPulseDuty = 0;
      break;
    case AlarmStatus::technical1:
      // technical1 = buzzer at 0.5 Hz with 10% duty cycle
      mOutHigh = false;
      mOutMed = false;
      mOutLow = false;
      mBuzzerPulsePeriod = 2000;
      mBuzzerPulseDuty = 200;
      break;
    case AlarmStatus::technical2:
      // technical2 = buzzer at 0.25 Hz with 5% duty cycle
      mOutHigh = false;
      mOutMed = false;
      mOutLow = false;
      mBuzzerPulsePeriod = 4000;
      mBuzzerPulseDuty = 200;
      break;
    case AlarmStatus::noAlarm:
      mOutHigh = false;
      mOutMed = false;
      mOutLow = false;
      mBuzzerPulsePeriod = 0;
      mBuzzerPulseDuty = 0;
      break;
    default:return AlarmManagerStatus::invalidAlarm;
  }

  return AlarmManagerStatus::ok;
}

}  // namespace Indicators
}  // namespace HAL
}  // namespace Pufferfish
