/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Solenoid.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: March Boonyapaluk
 */

#include "stm32h7xx_hal.h"
#include "Pufferfish/HAL/STM32/PWM.h"

namespace Pufferfish {
namespace HAL {

PWMStatus PWM::setDutyCycle(float duty) {
  if (0.0 < duty || duty > 1.0) {
    return PWMStatus::invalidDutyCycle;
  }
  setDutyCycleRaw(static_cast<uint32_t>(duty * getMaxDutyCycle()));
  return PWMStatus::ok;
}

void PWM::setDutyCycleRaw(uint32_t duty) {
  __HAL_TIM_SET_COMPARE(&mHtim, mChannel, duty);
}

PWMStatus PWM::start() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Start(&mHtim, mChannel);
  return stat == HAL_OK ? PWMStatus::ok : PWMStatus::halError;
}

PWMStatus PWM::stop() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Stop(&mHtim, mChannel);
  return stat == HAL_OK ? PWMStatus::ok : PWMStatus::halError;
}

uint32_t PWM::getMaxDutyCycle() {
  return __HAL_TIM_GET_AUTORELOAD(&mHtim);
}

} /* namespace HAL */
} /* namespace Pufferfish */
