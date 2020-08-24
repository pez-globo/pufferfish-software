/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include "Pufferfish/HAL/STM32/PulseGenerator.h"

/**
 * @brief  Inline function to validate the value in the range of max and min
 * @param  value input value to validate the range
 * @param  max input value of maximum range
 * @param  min input value of minimum range
 * @return TRUE/FALSE based on value is in range of max and min values
 */
inline bool valueMaxMin(uint32_t value, uint32_t max, uint32_t min)
{
    if ((value > max) or (value < min))
    {
      return true;
    }
    else
    {
      return false;
    }
}

namespace Pufferfish {
namespace HAL {

void PWMGenerator::update(uint32_t currentTime) {

  /* validate the pulse duration */
  uint32_t pulseDuration = currentTime - mLastCycle;

  /* trimming the duration to be within [0, mPulsePeriod) */
  if (pulseDuration >= mPulsePeriod){
    /* update the mLastCycle with current time */
    mLastCycle = currentTime;
  }
  /* Trimming the high or low based on pulse duty */
  if(pulseDuration <= mPulseDuty){
    mSwitching = true;
  }
  else
  {
    mSwitching = false;
  }

  /* Validate the saturation of pulseDuration of frequency with in max and min range */
  if (valueMaxMin(pulseDuration, mPulsePeriod, 0) == true)
  {
    mLastCycle = currentTime;
  }
}

bool PWMGenerator::output()
{
  return mSwitching;
}

void PulsedPWMGenerator::update(uint32_t currentTime) {

  uint32_t pulseDuration;

  /* validate the pulse duration for high period */
  pulseDuration = currentTime - mHighLastCycle;

  /* trimming the duration to be within [0, mHighPulsePeriod) */
  if (pulseDuration >= mHighPulsePeriod){
    /* update the mLastCycle with current time */
    mHighLastCycle = currentTime;
  }

  /* Trimming the high or low based on pulse duty */
  if(pulseDuration <= mPulseDuty){
    mHighSwitching = true;
  }
  else
  {
    mHighSwitching = false;
  }

  /* validate the pulse duration for low period */
  pulseDuration = currentTime - mLowLastCycle;

  /* trimming the duration to be within [0, mLowPulsePeriod) */
  if (pulseDuration >= mLowPulsePeriod){
    /* update the mLastCycle with current time */
    mLowLastCycle = currentTime;
  }

  /* Trimming the high or low based on pulse duty */
  if(pulseDuration <= mPulseDuty){
    mLowSwitching = true;
  }
  else
  {
    mLowSwitching = false;
  }

  /* Validate the saturation of pulseDuration of high frequency with in max and min range */
  if (valueMaxMin(pulseDuration, mHighPulsePeriod, 0) == true)
  {
    mHighLastCycle = currentTime;
  }

  /* Validate the saturation of pulseDuration of low frequency with in max and min range */
  if (valueMaxMin(pulseDuration, mLowPulsePeriod, 0) == true)
  {
    mLowLastCycle = currentTime;
  }
}

bool PulsedPWMGenerator::output()
{
  return mHighSwitching & mLowSwitching;
}

}  // namespace HAL
}  // namespace Pufferfish
