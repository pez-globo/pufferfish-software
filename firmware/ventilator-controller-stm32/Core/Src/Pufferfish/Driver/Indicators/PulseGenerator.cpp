/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include "Pufferfish/Driver/Indicators/PulseGenerator.h"

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

void PWMGenerator::start() {
  /* Set the reset value to true to start the pulse */
  mReset = true;
}

void PWMGenerator::update(uint32_t currentTime) {

  /* validate the pulse duration */
  uint32_t pulseDuration = currentTime - mLastCycle;

  /* Validate the reset value start the pulse */
  if (mReset == true)
  {
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

  }
  else
  {
    mSwitching = false;
  }
  /* Validate the saturation of pulseDuration of frequency with in maximum
     and minimum range */
  if (valueMaxMin(pulseDuration, mPulsePeriod, 0) == true)
  {
    mLastCycle = currentTime;
  }
}

bool PWMGenerator::output()
{
  return mSwitching;
}

void PWMGenerator::stop() {
  /* Set the reset value to false to stop the pulse */
  mReset = false;
}

void PulsedPWMGenerator::start() {
  /* Set the reset value to true to start the pulse */
  mReset = true;
}

void PulsedPWMGenerator::update(uint32_t currentTime) {

  /* Validate the high pulse duration */
  uint32_t highPulseDuration = currentTime - mHighLastCycle;

  /* Validate the low pulse duration */
  uint32_t lowPulseDuration = currentTime - mLowLastCycle;

  /* Validate the reset value start the pulse */
  if (mReset == true)
  {

    /* trimming the duration to be within [0, mHighPulsePeriod) */
    if (highPulseDuration >= mHighPulsePeriod){
      /* update the mLastCycle with current time */
      mHighLastCycle = currentTime;
    }

    /* Trimming the high or low based on pulse duty */
    if(highPulseDuration <= mPulseDuty){
      mHighSwitching = true;
    }
    else
    {
      mHighSwitching = false;
    }

    /* validate the pulse duration for low period */

    /* trimming the duration to be within [0, mLowPulsePeriod) */
    if (lowPulseDuration >= mLowPulsePeriod){
      /* update the mLastCycle with current time */
      mLowLastCycle = currentTime;
    }

    /* Trimming the high or low based on pulse duty */
    if(lowPulseDuration <= mPulseDuty){
      mLowSwitching = true;
    }
    else
    {
      mLowSwitching = false;
    }
  }
  else
  {
    mLowSwitching = false;
    mHighSwitching = false;
  }

  /* Validate the saturation of pulseDuration of high frequency with in maximum
       and minimum range */
  if (valueMaxMin(highPulseDuration, mHighPulsePeriod, 0) == true)
  {
    mHighLastCycle = currentTime;
  }

  /* Validate the saturation of pulseDuration of low frequency with in maximum
       and minimum range */
  if (valueMaxMin(lowPulseDuration, mLowPulsePeriod, 0) == true)
  {
    mLowLastCycle = currentTime;
  }

}

bool PulsedPWMGenerator::output()
{
  return mHighSwitching & mLowSwitching;
}

void PulsedPWMGenerator::stop() {
  /* Set the reset value to false to stop the pulse */
  mReset = false;
}
}  // namespace HAL
}  // namespace Pufferfish
