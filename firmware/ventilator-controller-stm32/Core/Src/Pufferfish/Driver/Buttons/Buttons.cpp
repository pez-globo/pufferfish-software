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
namespace Membrane {


void Buttons::init()
{
  /* FIXME: Need to find what is initial status of the buttons*/
  InputButtonState.buttonFallingEdge = false;
  InputButtonState.buttonLastValue = false;
  InputButtonState.buttonRisingEdge = false;
  InputButtonState.buttonState = false;

}

ButtonServices Buttons::buttenStatus()
{
  static const uint16_t debounceTime = 30; /// Need to fix debounce time
  InputButtonState.buttonCurrentValue = mInputButton.read();
  /// Check input button status as changed or not
  if(InputButtonState.buttonCurrentValue == InputButtonState.buttonLastValue)
  {
    if(InputButtonState.counter < 0xffff){
      InputButtonState.counter++;
    }
  }
  else
  {
    InputButtonState.counter = 0;
  }
  ///Check debounce time has expired
  if(InputButtonState.counter >= debounceTime)
  {
    if(InputButtonState.buttonState != false){
      InputButtonState.buttonState = true;
    }
  }
  if(InputButtonState.buttonState != false)
  {
    if((InputButtonState.buttonCurrentValue == true)
      &&(InputButtonState.buttonLastValue == false)){
      InputButtonState.buttonRisingEdge = true;
    }else if((InputButtonState.buttonCurrentValue == false)
        &&(InputButtonState.buttonLastValue == true)){
        InputButtonState.buttonFallingEdge = true;
      }else{
        InputButtonState.buttonRisingEdge = false;
        InputButtonState.buttonFallingEdge = false;
      }
  }

  InputButtonState.buttonLastValue = InputButtonState.buttonCurrentValue;
  if(InputButtonState.buttonRisingEdge ==  true)
  {
    return ButtonServices::actionOnRisingEdge;
  }else if(InputButtonState.buttonRisingEdge ==  true){
    return ButtonServices::actionOnFallingEdge;
  }
  ///FIXME: Required Button stuck fault
  return ButtonServices::noAction;
}

MembraneLEDStatus Buttons::ledState()
{
  ButtonServices status = this->buttenStatus();
  if(status == ButtonServices::actionOnRisingEdge)
  {
    mregLED.write(true);
    return MembraneLEDStatus::on;
  }else if(status == ButtonServices::actionOnFallingEdge){
    mregLED.write(false);
    return MembraneLEDStatus::off;
  }else{
    mregLED.write(false);
    return MembraneLEDStatus::off;
  }
}

}  // namespace Membrane
}  // namespace HAL
}  // namespace Pufferfish




