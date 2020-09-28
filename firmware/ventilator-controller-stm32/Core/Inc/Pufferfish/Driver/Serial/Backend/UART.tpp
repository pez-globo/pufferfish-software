/*
 * UARTBackend.tpp
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "UART.h"

namespace Pufferfish::Driver::Serial::Backend {

// UARTBackend

void UARTBackend::setup_irq() {
  uart_.setup_irq();
}

void UARTBackend::receive() {
  while (true) {  // repeat until UART read buffer is empty or output is available
    uint8_t receive = 0;

    // UART
    switch (uart_.read(receive)) {
      case BufferStatus::ok:
        break;
      case BufferStatus::empty:
      default:
        return;
    }

    // Backend
    switch (backend_.input(receive)) {
      case Backend::Status::invalid:
        // TODO(lietk12): handle error case first
      case Backend::Status::waiting:
        break;
      case Backend::Status::ok:
        return;
    }
  }
}

void UARTBackend::update_clock(uint32_t current_time) {
  backend_.update_clock(current_time);
}

void UARTBackend::send() {
  // Create a new output to write if needed
  if (sent_ >= send_output_.size()) {
    switch (backend_.output(send_output_)) {
      case Backend::Status::ok:  // ready to write to UART
        sent_ = 0;
        break;
      default:
        // TODO(lietk12): handle error cases first
        return;
    }
  }
  // Attempt to finish writing the current output
  HAL::AtomicSize written = 0;
  uart_.write(send_output_.buffer() + sent_, send_output_.size() - sent_, written);
  sent_ += written;
}

}  // namespace Pufferfish::Driver::Serial::Backend
