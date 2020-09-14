/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda s
 */

#include <Pufferfish/HAL/Mock/MockPWM.h>

namespace Pufferfish {
namespace HAL {

PWMStatus MockPWM::setDutyCycle(float duty) {
  if (0.0 < duty || duty > 1.0) {
    return PWMStatus::invalidDutyCycle;
  }
  mLastDuty = duty;
  setDutyCycleRaw(static_cast<uint32_t>(duty * getMaxDutyCycle()));
  return PWMStatus::ok;
}

float MockPWM::getDutyCycle()
{
  return mLastDuty;
}

void MockPWM::setDutyCycleRaw(uint32_t duty) {
  mLastRawDuty = duty;
}

float MockPWM::getDutyCycleRaw()
{
  return mLastRawDuty;
}

PWMStatus MockPWM::start() {
  mState = true;
  return PWMStatus::ok;
}

bool MockPWM::getPWMState() {
  return mState;
}
PWMStatus MockPWM::stop() {
  mState = false;
  return PWMStatus::ok;
}

uint32_t MockPWM::getMaxDutyCycle() {
  return mLastMaxDuty;
}

void MockPWM::setMaxDutyCycle(uint32_t duty){
  mLastMaxDuty = duty;
}


} /* namespace HAL */
} /* namespace Pufferfish */
