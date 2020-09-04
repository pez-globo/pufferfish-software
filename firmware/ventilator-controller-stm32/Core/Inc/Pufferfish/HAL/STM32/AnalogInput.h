/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include "Pufferfish/Statuses.h"
#include "stm32h7xx_hal.h"

namespace Pufferfish {
namespace HAL {

/**
 * Represents a Analog input in STM32
 */
class AnalogInput {
 public:
  /**
   * @brief Constructs a new Analog Input
   * @param hadc and tTimeout
   */
  AnalogInput(ADC_HandleTypeDef &hadc, uint32_t t_timeout)
      : adc_input_(hadc), timeout_(t_timeout) {}

  /**
   * @brief Calls HAL_ADC_Start
   * @param  None
   * @return ADCStatus returns error/ok/busy
   */
  ADCStatus start();

  /**
   * @brief Reads a Analog conversion data
   * @param  AnalogDataRead, Out parameter returns the read data
   * @return ADCStatus returns error/ok/timeout
   */
  ADCStatus read(uint32_t &analog_data_read);

  /**
   * Calls HAL_ADC_Stop
   * @param  None
   * @return ADCStatus returns error/ok
   */
  ADCStatus stop();

 private:
  ADC_HandleTypeDef &adc_input_;
  uint32_t timeout_ = 0;
};

}  // namespace HAL
}  // namespace Pufferfish
