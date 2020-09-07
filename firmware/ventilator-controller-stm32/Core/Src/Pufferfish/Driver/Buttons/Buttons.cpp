/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Buttons.cpp
 *
 *  Created on: Aug 29, 2020
 *      Author: Laxmanrao R
 */


#include "Pufferfish/Driver/Buttons/Buttons.h"

namespace Pufferfish {
namespace Driver {
namespace MembraneButton {


ButtonStatus DeBounce::transform(bool input, uint32_t currentTime, bool &output)
{
  if(this->timeValidCheck(currentTime, lastSampleTime, samplingPeriod)){
      return ButtonStatus::notOk;
   }

  lastSampleTime = currentTime;

  /**
   * Update the integrator based on the input signal
   */
  if (input == 0)
  {
    if (integrator > 0){
        integrator--;
    }
  }
  else if (integrator < maxIntegratorSamples){
      integrator++;
  }
  /**
   * Update the integrator based on the input signal
   */
  if (integrator == 0){
    mOutput = 0;
    lastTimeStable = currentTime;
  } else if (integrator >= maxIntegratorSamples) {
    mOutput = 1;
    lastTimeStable = currentTime;
    integrator = maxIntegratorSamples;  /* defensive code if integrator got corrupted */
  }
  /**
   * Report switch fault if debounce time exceeds the maximum limit
   */
    if(!this->timeValidCheck(currentTime, lastTimeStable, debounceTimeLimit)){
      return ButtonStatus::notOk;
   }
   output = mOutput;

 return ButtonStatus::ok;
}

EdgeState EdgeDetection::isSwitchSateChanged(bool state)
{

  if (state != lastState) {
    /* Update the last state */
    lastState = state;
    /* check for state is changed */
    if (state == true) {
      /* return the EdgeState as rising edge */
      return EdgeState::risingEdge;
    } else {
      /* return the EdgeState as falling edge */
      return EdgeState::fallingEdge;
    }
  }
  return EdgeState::noEdge;
}

ButtonStatus Button::readButtonstate(bool &debounedOutput, EdgeState &switchStateChanged){

  bool input = mButtoninput.read();
  uint32_t msTime = Pufferfish::HAL::millis();

  ButtonStatus status= mDebouncer.transform(input, msTime, debounedOutput);
  
  /* Debounce is not success */
  if(status != ButtonStatus::ok) {
    return status;
  }
  switchStateChanged = mEdgeDetect.isSwitchSateChanged(debounedOutput);

  return  status;
}


// This method returns
// "true" --> if nowTime < (lastTime + addFactor)
// "false" --> if nowTime >= (lastTime + addFactor)

bool DeBounce::timeValidCheck(uint32_t nowTime, uint32_t lastTime, uint32_t addFactor)
{
  uint32_t upperTimeLimit = lastTime + addFactor;

  // nowTime: crossed 32 bit boundary ; upperTimeLimit: NOT crossed 32 bit boundary
  if((nowTime < lastTime) && (upperTimeLimit > lastTime)){
    return false; // Current time is  > reference range
  }

  // nowTime: NOT crossed 32 bit boundary ; upperTimeLimit: crossed 32 bit boundary
  if((nowTime > lastTime) && (upperTimeLimit < lastTime)){
    return true; // Current time is  < reference range
  }

  //both nowTime and upperTimeLimit: either crossed 32 bit boundary OR NOT crossed 32 bit boundary
  if(nowTime < upperTimeLimit){
    return true; // Current time is  < reference range
  }
  else{
    return false; // Current time is  >= reference range
  }
 }

}  // namespace Membrane
}  // namespace HAL
}  // namespace Pufferfish




