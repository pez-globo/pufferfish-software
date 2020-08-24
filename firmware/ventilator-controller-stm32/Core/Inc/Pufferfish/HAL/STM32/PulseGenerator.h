/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "Pufferfish/HAL/STM32/DigitalFunction.h"

namespace Pufferfish {
namespace HAL {

/*
 * PWMGenerator subclass of DigitalFunctionGenerator
 * Generates the pulse for the provided period and duty cycle
 */
class PWMGenerator : public DigitalFunctionGenerator {
 public:

  /*
   * @brief DigitalFunctionGenerator Constructor
   * @param PulsePeriod - pulse period (frequency)
   *        PulseDuty - pulse duty cycle
  */
  PWMGenerator(uint32_t PulsePeriod, uint32_t PulseDuty)
   : mPulsePeriod(PulsePeriod), mPulseDuty(PulseDuty) {

  }

  /**
  * @brief update method updates the mSwitching private variable based on
  * period and duty cycle for the provided currentTime.
  * @param currentTime current time in micros.
  */
  void update(uint32_t currentTime) override;

  /*
   * @brief returns the bool output of mSwitching
   */
  bool output() override;

 private:

  /* Period/frequency and Pulse duty private variables */
  uint32_t mPulsePeriod = 0, mPulseDuty = 0;

  /* mSwitching provides the pulse high or low for the provided current time */
  bool mSwitching;

  /* Stores the last cycle period of current time */
  uint32_t mLastCycle = 0;
};


/*
 * PulsedPWMGenerator subclass of DigitalFunctionGenerator
 * Generates the pulse for the provided high period, low period and duty cycle
 */
class PulsedPWMGenerator : public DigitalFunctionGenerator {
 public:

  /*
   * @brief DigitalFunctionGenerator Constructor
   * @param PulsePeriod - pulse period (frequency)
   *        PulseDuty - pulse duty cycle
  */
  PulsedPWMGenerator(uint32_t HighPulsePeriod, uint32_t LowPulsePeriod, uint32_t PulseDuty)
   : mHighPulsePeriod(HighPulsePeriod), mLowPulsePeriod(LowPulsePeriod), mPulseDuty(PulseDuty) {

  }

  /**
  * @brief update method updates the mSwitching private variable based on
  * period and duty cycle for the provided currentTime.
  * @param currentTime current time in micros.
  */
  void update(uint32_t currentTime) override;

  /*
   * @brief returns the bool output of mHighSwitching AND mLowSwitching
   */
  bool output() override;

 private:

  /* high and low Period/frequency and Pulse duty private variables */
  uint32_t mHighPulsePeriod = 0, mLowPulsePeriod = 0, mPulseDuty = 0;

  /* mHighSwitching and mLowSwitching provides the pulse high or low for the provided current time
   * for high and low period
   */
  bool mHighSwitching = false, mLowSwitching = false;

  /* Stores the last cycle period of current time for high and low period */
  uint32_t mHighLastCycle = 0, mLowLastCycle = 0;
};
}  // namespace HAL
}  // namespace Pufferfish
