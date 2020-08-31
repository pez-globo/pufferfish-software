/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * AlarmsManager.h
 *
 *  Created on: Jun 23, 2020
 *      Author: March Boonyapaluk
 */

#pragma once

#include "Pufferfish/Driver/Indicators/AlarmDevice.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Types.h"

namespace Pufferfish {

/**
 * Manages and prioritize all alarms raised by the system
 */
class AlarmsManager {
 public:
  /**
   * Constructs a new AlarmsManager object
   * @param led         LED alarm
   * @param auditory    Buzzer and Auditory alarm
   */
  AlarmsManager(Driver::Indicators::AlarmDevice &led,
                Driver::Indicators::AlarmDevice &auditory)
      : alarms_cnt_{},
        active_(AlarmStatus::no_alarm),
        led_(led),
        auditory_(auditory) {}

  /**
   * Adds an alarm to the currently active list of alarms
   * @param a   type of alarm to add
   * @return invalidAlarm if `a` is not a valid alarm, ok otherwise
   */
  AlarmManagerStatus add(AlarmStatus a);

  /**
   * Removes an alarm from the currently active list of alarms
   * @param a   type of alarm to remove
   * @return invalidAlarm if `a` is not a valid alarm,
   *    notSet if `a` is currently not active, ok otherwise
   */
  AlarmManagerStatus remove(AlarmStatus a);

  /**
   * Performs necessary updates on the alarm indicators,
   *    must be called with frequency at least 500 Hz
   * @param currentTime current system time, in ms
   * @return ok if the update is successful, error code otherwise
   */
  AlarmManagerStatus update(uint32_t current_time);

  /**
   * Get the alarm currently outputted by the system
   * @return the alarm currently displayed
   */
  AlarmStatus get_active();

 private:
  uint32_t alarms_cnt_[static_cast<int>(AlarmStatus::no_alarm)];
  AlarmStatus active_;
  bool updated_ = false;

  Driver::Indicators::AlarmDevice &led_;
  Driver::Indicators::AlarmDevice &auditory_;

  void update_active();
};

}  // namespace Pufferfish
