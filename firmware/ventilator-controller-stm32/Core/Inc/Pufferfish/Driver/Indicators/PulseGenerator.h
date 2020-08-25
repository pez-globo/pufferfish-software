/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "Pufferfish/Driver/Indicators/DigitalFunction.h"

namespace Pufferfish {
namespace HAL {

/*
 * PWMGenerator subclass of DigitalFunctionGenerator Generates the pulse for
 * the provided period and duty cycle
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
   * @brief  start method updates the mReset to TRUE to start the alarm
   * @param  None
   * @return None
   */
  void start() override;

  /**
   * @brief update method updates the mSwitching private variable based on
   *        period and duty cycle for the provided currentTime
   * @param currentTime current time in micro-seconds.
   * @return None
   */
  void update(uint32_t currentTime) override;

  /*
   * @brief  returns the bool output of mSwitching
   * @param  None
   * @return TRUE/FALSE of pulse switching
   */
  bool output() override;

  /**
   * @brief stop method updates the mReset to FALSE to stop the alarm
   * @param None
   */
  void stop() override;

 private:

  /* Period/frequency and Pulse duty private variables */
  uint32_t mPulsePeriod = 0, mPulseDuty = 0;

  /* mSwitching provides the pulse high or low for the provided current time */
  bool mSwitching;

  /* Stores the last cycle period of current time */
  uint32_t mLastCycle = 0;

  /* Pulse generated based on mReset TRUE/FALSE */
  uint32_t mReset = false;
};


/**
 * PulsedPWMGenerator subclass of DigitalFunctionGenerator
 * Generates the pulse for the provided high period, low period and duty cycle
 */
class PulsedPWMGenerator : public DigitalFunctionGenerator {
 public:

  /**
   * @brief DigitalFunctionGenerator Constructor
   * @param PulsePeriod - pulse period (frequency)
   *        PulseDuty - pulse duty cycle
   */
  PulsedPWMGenerator(uint32_t HighPulsePeriod, uint32_t LowPulsePeriod,
                     uint32_t PulseDuty) : mHighPulsePeriod(HighPulsePeriod),
                     mLowPulsePeriod(LowPulsePeriod), mPulseDuty(PulseDuty) {

  }

  /**
   * @brief  start method updates the mReset to TRUE to start the alarm
   * @param  None
   * @return None
   */
  void start() override;

  /**
   * @brief update method updates the mSwitching private variable based on
   *        period and duty cycle for the provided currentTime
   * @param currentTime current time in micro-seconds.
   * @return None
   */
  void update(uint32_t currentTime) override;

  /**
   * @brief  returns the bool output of mSwitching
   * @param  None
   * @return TRUE/FALSE of pulse switching
   */
  bool output() override;

  /**
   * @brief  stop method updates the mReset to FALSE to stop the alarm
   * @param  None
   * @return None
   */
  void stop() override;

 private:

  /* high and low Period/frequency and Pulse duty private variables */
  uint32_t mHighPulsePeriod = 0, mLowPulsePeriod = 0, mPulseDuty = 0;

  /* mHighSwitching and mLowSwitching provides the pulse high or low for the
     provided current time for high and low period */
  bool mHighSwitching = false, mLowSwitching = false;

  /* Stores the last cycle period of current time for high and low period */
  uint32_t mHighLastCycle = 0, mLowLastCycle = 0;

  /* Pulse generated based on mReset TRUE/FALSE */
  uint32_t mReset = false;
};
}  // namespace HAL
}  // namespace Pufferfish
