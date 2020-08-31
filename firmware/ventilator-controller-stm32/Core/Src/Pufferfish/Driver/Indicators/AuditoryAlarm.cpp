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

AlarmManagerStatus AuditoryAlarm::update(uint32_t current_time) {
  reg_high_.write(out_high_);
  reg_med_.write(out_med_);
  reg_low_.write(out_low_);

  // [0, mBuzzerPulseDuty) -> on, [mBuzzerPulseDuty, mBuzzerPulsePeriod) -> off
  uint32_t pulse_duration = current_time - last_cycle_;

  // trimming the duration to be within [0, mBuzzerPulsePeriod)
  if (pulse_duration >= buzzer_pulse_period_ || reset_) {
    // NOTE: this is only accurate in the case that update() is being called
    //  fast enough (> 1 kHz). However, it guarantees that the "on" time will
    //  always be >= mBuzzerPulseDuty
    pulse_duration = 0;
    last_cycle_ = current_time;
    reset_ = false;
    buzzer_switching_ = true;
  }

  if (pulse_duration < buzzer_pulse_duty_) {
    // on
    buzzer_.write(buzzer_switching_);
    buzzer_switching_ = !buzzer_switching_;
  } else {
    // off
    buzzer_.write(false);
  }

  return AlarmManagerStatus::ok;
}

AlarmManagerStatus AuditoryAlarm::set_alarm(AlarmStatus a) {
  reset_ = true;

  switch (a) {
    case AlarmStatus::high_priority:
      out_high_ = true;
      out_med_ = true;
      out_low_ = true;
      buzzer_pulse_period_ = 0;
      buzzer_pulse_duty_ = 0;
      break;
    case AlarmStatus::medium_priority:
      out_high_ = false;
      out_med_ = true;
      out_low_ = true;
      buzzer_pulse_period_ = 0;
      buzzer_pulse_duty_ = 0;
      break;
    case AlarmStatus::low_priority:
      out_high_ = false;
      out_med_ = false;
      out_low_ = true;
      buzzer_pulse_period_ = 0;
      buzzer_pulse_duty_ = 0;
      break;
    case AlarmStatus::technical1:
      // technical1 = buzzer at 0.5 Hz with 10% duty cycle
      out_high_ = false;
      out_med_ = false;
      out_low_ = false;
      buzzer_pulse_period_ = 2000;
      buzzer_pulse_duty_ = 200;
      break;
    case AlarmStatus::technical2:
      // technical2 = buzzer at 0.25 Hz with 5% duty cycle
      out_high_ = false;
      out_med_ = false;
      out_low_ = false;
      buzzer_pulse_period_ = 4000;
      buzzer_pulse_duty_ = 200;
      break;
    case AlarmStatus::no_alarm:
      out_high_ = false;
      out_med_ = false;
      out_low_ = false;
      buzzer_pulse_period_ = 0;
      buzzer_pulse_duty_ = 0;
      break;
    default:
      return AlarmManagerStatus::invalid_alarm;
  }

  return AlarmManagerStatus::ok;
}

}  // namespace Indicators
}  // namespace Driver
}  // namespace Pufferfish
