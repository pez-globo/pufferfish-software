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


ButtonReturn DeBounce::transform(bool input, uint32_t currentTime, bool &output)
{
  if ( currentTime < timeValidCheck( lastSampleTime, samplingPeriod )){
     return ButtonReturn::notOk;
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
   if(currentTime >= timeValidCheck( lastTimeStable,debounceTimeLimit ) )
   {
     return ButtonReturn::unKnown;
   }
   output = mOutput;

 return ButtonReturn::ok;

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

ButtonReturn Button::readButtonstate(bool &debounedOutput, EdgeState &switchStateChanged){

  bool input = mButtoninput.read();
  uint32_t msTime = Pufferfish::HAL::millis();

  ButtonReturn status= mDebouncer.transform(input, msTime, debounedOutput);
  
  /* Debounce is not success */
  if(status != ButtonReturn::ok) {
    return status;
  }
  switchStateChanged = mEdgeDetect.isSwitchSateChanged(debounedOutput);

  return  status;
}

}  // namespace Membrane
}  // namespace HAL
}  // namespace Pufferfish




