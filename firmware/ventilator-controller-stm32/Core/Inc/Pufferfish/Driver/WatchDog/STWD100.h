/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 */

#pragma once

#include "Pufferfish/HAL/HAL.h"

namespace Pufferfish {
namespace Driver {
namespace WatchDog {

/**
 * An abstract class for STWD100 Watch Dog module
 */

class STWD100 {

public:
  /**
    * Construct a STWD100 Watch Dog
    *
    * @param enablePin for STWD100
    * @param inputPin for STWD100
    */
  STWD100 (HAL::DigitalOutput &enablePin, HAL::DigitalOutput &inputPin )
           :mEnablenPin(enablePin),mInputPin (inputPin){
  }

  void enable(void);

  void disable(void);

  void toggleInput(void);

private:
  HAL::DigitalOutput mEnablenPin;
  HAL::DigitalOutput mInputPin;

};

}  // namespace WatchDog
}  // namespace Driver
}  // namespace Pufferfish
