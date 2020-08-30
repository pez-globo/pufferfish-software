/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 *  Created on: Aug 29, 2020
 *      Author: Laxmanrao R
 */

#pragma once

#include "Pufferfish/HAL/HAL.h"


namespace Pufferfish {
namespace Driver {
namespace Membrane {

/**
 * Status of the buttons
 */
struct ButtonStatus{
  uint16_t counter;
  bool buttonCurrentValue;
  bool buttonLastValue;
  bool buttonRisingEdge;
  bool buttonFallingEdge;
  bool buttonState;
};

/**
 * Services required for when buttons are triggered high to low or low to high
 */
enum class ButtonServices {
  noAction = 0,
  actionOnRisingEdge,
  actionOnFallingEdge
};

/**
 * Membrane Buttons LED status
 */
enum class MembraneLEDStatus{
  off = 0,
  on
};
/**
 * Drives for Membrane Buttons debouncing
 */
class Buttons{

public:

  Buttons(HAL::DigitalInput &inputButton, HAL::DigitalOutput regLED)
              : mInputButton(inputButton),mregLED(regLED){

  }
    /**
    * Initialize the Button Status structure variables
    */
    void init();
    /**
     * Button status with denounce time calculation
     * @param ButtonServices new service required or not
     */
    ButtonServices buttenStatus();
    /**
     * Initialize the Button Status structure variables
     * @param MembraneLEDStatus Membrane button led status
     */
    MembraneLEDStatus ledState();
private:

    HAL::DigitalInput &mInputButton;
    HAL::DigitalOutput &mregLED;
    static ButtonStatus InputButtonState;

};
}  // namespace Buttons
}  // namespace HAL
}  // namespace Pufferfish
