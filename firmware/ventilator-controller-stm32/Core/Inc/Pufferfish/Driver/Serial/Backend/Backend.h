/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/HAL/Interfaces/CRCChecker.h"
#include "States.h"
#include "Transport.h"

namespace Pufferfish::Driver::Serial::Backend {

// Backend

class Backend {
 public:
  using Status = Synchronizers::Status;

  Backend(
      HAL::Interfaces::CRC32 &crc32c,
      Application::Store &store,
      Application::LogEventsSender &log_sender)
      : receiver_(crc32c), sender_(crc32c), synchronizers_(store, log_sender) {}

  Status input(uint8_t new_byte);
  void update_clock(uint32_t current_time);
  Status output(FrameProps::ChunkBuffer &output_buffer);

  [[nodiscard]] bool connected() const;

 private:
  Receiver receiver_;
  Sender sender_;
  Synchronizers synchronizers_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
