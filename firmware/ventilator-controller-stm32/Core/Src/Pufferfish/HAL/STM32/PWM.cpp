/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Solenoid.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: March Boonyapaluk
 */

#include <Pufferfish/HAL/STM32/PWM.h>

namespace Pufferfish::HAL::STM32 {

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

uint32_t PWM::get_max_duty_cycle() {
  return __HAL_TIM_GET_AUTORELOAD(&htim_);
}

}  // namespace Pufferfish::HAL::STM32
