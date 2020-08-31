/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Solenoid.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: March Boonyapaluk
 */

#include "Pufferfish/HAL/STM32/PWM.h"

#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

PWMStatus PWM::set_duty_cycle(float duty) {
  if (duty < 0.0 || duty > 1.0) {
    return PWMStatus::invalid_duty_cycle;
  }
  set_duty_cycle_raw(static_cast<uint32_t>(duty * get_max_duty_cycle()));
  return PWMStatus::ok;
}

void PWM::set_duty_cycle_raw(uint32_t duty) {
  __HAL_TIM_SET_COMPARE(&htim_, channel, duty);
}

PWMStatus PWM::start() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Start(&htim_, channel);
  return stat == HAL_OK ? PWMStatus::ok : PWMStatus::hal_error;
}

PWMStatus PWM::stop() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Stop(&htim_, channel);
  return stat == HAL_OK ? PWMStatus::ok : PWMStatus::hal_error;
}

uint32_t PWM::get_max_duty_cycle() { return __HAL_TIM_GET_AUTORELOAD(&htim_); }

} /* namespace HAL */
} /* namespace Pufferfish */
