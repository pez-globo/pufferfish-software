/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include "Pufferfish/HAL/Interfaces/PWM.h"


namespace Pufferfish {
namespace HAL {

PWMStatus PWM::set_duty_cycle(float duty) {
  if (0.0 < duty || duty > 1.0) {
    return PWMStatus::invalid_duty_cycle;
  }

  set_duty_cycle_raw(static_cast<uint32_t>(duty * get_max_duty_cycle()));
  return PWMStatus::ok;
}

} /* namespace HAL */
} /* namespace Pufferfish */
