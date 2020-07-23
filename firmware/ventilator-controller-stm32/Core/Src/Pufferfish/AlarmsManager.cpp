/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * AlarmsManager.cpp
 *
 *  Created on: Jun 23, 2020
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/AlarmsManager.h"

namespace Pufferfish {

AlarmManagerStatus Pufferfish::AlarmsManager::add(AlarmStatus a) {
  int ind = static_cast<int>(a);

  if (ind < 0 || ind >= static_cast<int>(AlarmStatus::noAlarm)) {
    return AlarmManagerStatus::invalidAlarm;
  }

  mAlarmsCnt[ind]++;
  this->updateActive();

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus Pufferfish::AlarmsManager::remove(AlarmStatus a) {
  int ind = static_cast<int>(a);

  if (ind < 0 || ind >= static_cast<int>(AlarmStatus::noAlarm)) {
    return AlarmManagerStatus::invalidAlarm;
  }

  if (mAlarmsCnt[ind] == 0) {
    return AlarmManagerStatus::notSet;
  }

  mAlarmsCnt[ind]--;
  this->updateActive();

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus Pufferfish::AlarmsManager::update(uint32_t currentTime) {
  AlarmManagerStatus led_stat = mLED.update(currentTime);
  AlarmManagerStatus audi_stat = mAuditory.update(currentTime);

  if (led_stat != AlarmManagerStatus::ok) {
    return led_stat;
  }

  if (audi_stat != AlarmManagerStatus::ok) {
    return audi_stat;
  }

  return AlarmManagerStatus::ok;
}

AlarmStatus Pufferfish::AlarmsManager::getActive() {
  return mActive;
}

void Pufferfish::AlarmsManager::updateActive() {
  for (int i = 0; i < static_cast<int>(AlarmStatus::noAlarm); i++) {
    if (mAlarmsCnt[i]) {
      if (static_cast<AlarmStatus>(i) != mActive) {
        mLED.setAlarm(static_cast<AlarmStatus>(i));
        mAuditory.setAlarm(static_cast<AlarmStatus>(i));
      }

      mActive = static_cast<AlarmStatus>(i);
      return;
    }
  }

  mActive = AlarmStatus::noAlarm;
}

}
