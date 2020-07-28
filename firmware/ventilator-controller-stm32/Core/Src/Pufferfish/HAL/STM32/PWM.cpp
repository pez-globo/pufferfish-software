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

void PWM::setDutyCycle(float duty) {
  assert_param(0.0 <= duty && duty <= 1.0);
  setDutyCycleRaw(static_cast<uint32_t>(duty * getMaxDutyCycle()));
}

void PWM::setDutyCycleRaw(uint32_t duty) {
  __HAL_TIM_SET_COMPARE(&mHtim, mChannel, duty);
}

bool PWM::start() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Start(&mHtim, mChannel);
  return stat == HAL_OK;
}

bool PWM::stop() {
  HAL_StatusTypeDef stat = HAL_TIM_PWM_Stop(&mHtim, mChannel);
  return stat == HAL_OK;
}

uint32_t PWM::getMaxDutyCycle() {
  return __HAL_TIM_GET_AUTORELOAD(&mHtim);
}

} /* namespace HAL */
} /* namespace Pufferfish */
