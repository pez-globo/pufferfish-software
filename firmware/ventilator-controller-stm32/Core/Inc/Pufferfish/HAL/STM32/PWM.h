/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Solenoid.h
 *
 *  Created on: Jul 9, 2020
 *      Author: March Boonyapaluk
 */

#pragma once

#include "Pufferfish/Statuses.h"
#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

/**
 * A generic PWM driver
 */
class PWM {
 public:
  /**
   * Constructs a new PWM device
   * @param htim    a timer that is handling a PWM device
   * @param channel a channel of the timer that the device is connected, must be
   * init in PWM mode
   */
  PWM(TIM_HandleTypeDef &htim, uint32_t channel)
      : htim_(htim), channel(channel) {}

  /**
   * Set a duty cycle of PWM, can be done when PWM is active
   * this function does NOT start the PWM output
   * @param duty    a number between 0.0 and 1.0 (inclusive) for the desired
   * duty cycle
   */
  PWMStatus set_duty_cycle(float duty);

  /**
   * Set a duty cycle of PWM, can be done when PWM is active
   * this function does NOT start the PWM output.
   * This is faster than setDutyCycle() as no floating point calculation is done
   * @param duty    an integer between 0 and getMaxDutyCycle() (inclusive) for
   * the desired duty cycle
   */
  void set_duty_cycle_raw(uint32_t duty);

  /**
   * Start the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  PWMStatus start();

  /**
   * Stop the PWM output
   * @return ok if the operation is successful, error code otherwise
   */
  PWMStatus stop();

  /**
   * Returns the maximum duty cycle that can be set with setDutyCycleRaw()
   * @return the maximum duty cycle
   */
  uint32_t get_max_duty_cycle();

 private:
  TIM_HandleTypeDef &htim_;
  const uint32_t channel;
};

} /* namespace HAL */
} /* namespace Pufferfish */
