/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#include "Pufferfish/HAL/Mock/MockAnalogInput.h"

namespace Pufferfish::HAL {

ADCStatus MockAnalogInput::start() {
  if(set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  last_state_ = true;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}

ADCStatus MockAnalogInput::read(uint32_t &analog_data_read) {
  if(set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  analog_data_read = last_input_;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}

void MockAnalogInput::set_read(uint32_t set_analog_data_read) {
  /* Update the lastInput with input analog data read value */
  last_input_ = set_analog_data_read;
}

ADCStatus MockAnalogInput::stop() {
  if(set_return_status_ == ADCStatus::error) {
    /* return ADCStatus as error */
    return ADCStatus::error;
  }

  last_state_ = false;
  /* return ADCStatus as ok */
  return ADCStatus::ok;
}
void MockAnalogInput::set_return_status(ADCStatus adc_status) {
  set_return_status_ = adc_status;
}

bool MockAnalogInput::get_analog_input_state() const {
  return last_state_;
  ;
}
}  // namespace Pufferfish::HAL
