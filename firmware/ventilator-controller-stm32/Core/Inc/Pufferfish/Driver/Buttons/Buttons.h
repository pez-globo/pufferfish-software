/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 *  Created on: Aug 29, 2020
 *      Author: Laxmanrao R
 */

#pragma once

#include "Pufferfish/HAL/HAL.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/HAL/STM32/Time.h"

namespace Pufferfish {
namespace Driver {
namespace MembraneButton {

enum class ButtonStatus{
  ok = 0, /// Ok if debounce is success
  notOk, /// notOk if current time exceeds sampling period
  unKnown /// Fault, if input state debouncing more the maximum debounce time limit
};

enum class EdgeState{
    noEdge = 0, /// Button is in stable state
    risingEdge, /// Button triggered on rising edge
    fallingEdge /// Button triggered on falling edge
};

/**
 * Abstract class for Debounce calculation
 */
class DeBounce
{

public:

  DeBounce(){
  }
  /**
   * Calculate the debounce time for input button
   * @param input button state High or Low
   * @param current_time
   * @param output state of the debounced output
   * @return ok on success, error code otherwise
   */
  ButtonStatus transform(bool input, uint32_t current_time, bool &output);

private:
  /**
   * Check time overflow conditions
   * @param nowTime debouncer current time
   * @param lastTime debouncer last time
   * @param addFactor factor added to the last time
   * @return true or false
   */
  bool timeValidCheck(uint32_t nowTime, uint32_t lastTime, uint32_t addFactor);
      static const uint32_t debounceTimeLimit =  2000;
      uint32_t samplingPeriod = 1;
      uint32_t lastSampleTime;
      uint8_t integrator;
      uint32_t lastTimeStable;
      const uint8_t maxIntegratorSamples = 100;
      bool mOutput;

};

/**
 * Abstract class for Edge state transition
 */
class EdgeDetection
{

public:
  EdgeDetection(){
  }

  /**
   * Checking switch state transition
   * @param state debounced output
   * @return rising edge on Low to High or falling edge on High to Low
   */
  EdgeState isSwitchSateChanged(bool state);
private:
  bool lastState;

};

/**
 * Abstract class for Membrane button input
 */
class Button
{
public:
  Button(HAL::DigitalInput &buttoninput, DeBounce &debouncer)
    : mButtoninput(buttoninput),
      mDebouncer(debouncer) {
  }

  /**
   * Read button state
   * @param debounedOutput debounced output state
   * @param EdgeState rising edge on Low to High or falling edge on High to Low
   * @return rising edge on Low to High or falling edge on High to Low
   */
  ButtonStatus readButtonstate(bool &debounedOutput, EdgeState &switchStateChanged);

private:

  HAL::DigitalInput &mButtoninput;
  DeBounce &mDebouncer;
  EdgeDetection mEdgeDetect;
};

}  // namespace MembraneButton
}  // namespace HAL
}  // namespace Pufferfish
