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

void PWMGenerator::start(uint32_t currentTime) {
  /* Set the reset value to true to start the pulse */
  mReset = true;
  /* Update the LastCycle with currentTime to start the pulse with raising edge */
  mLastCycle = currentTime;
  /* Invoke the update function of this object */
  this->update(currentTime);
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

void PWMGenerator::stop()
{
  /* Set the reset value to false to stop the pulse */
  mReset = false;
}

void PulsedPWMGenerator::start(uint32_t currentTime)
{
  /* Start the pulse of low and high period/frequency */
  mPulsePWMGenerator1.start(currentTime);
  mPulsePWMGenerator2.start(currentTime);
}

void PulsedPWMGenerator::update(uint32_t currentTime)
{
  /* Update the pulse of low and high period/frequency */
  mPulsePWMGenerator1.update(currentTime);
  mPulsePWMGenerator2.update(currentTime);
}

bool PulsedPWMGenerator::output()
{
  /* Get the output of low and high period/frequency */
  bool outputPulse1 = mPulsePWMGenerator1.output();
  bool outputPulse2 = mPulsePWMGenerator2.output();

  /* return the AND of pulses of high and low frequency */
  return outputPulse1 & outputPulse2;
}

void PulsedPWMGenerator::stop()
{
  /* Stop the pulse of high and low frequency */
  mPulsePWMGenerator1.stop();
  mPulsePWMGenerator2.stop();
}
}  // namespace HAL
}  // namespace Pufferfish
