/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/HAL/Interfaces/CRCChecker.h"
#include "Pufferfish/Protocols/Application/States.h"
#include "Pufferfish/Util/Timeouts.h"
#include "States.h"
#include "Transport.h"

namespace Pufferfish::Driver::Serial::Backend {

// Backend

static const uint32_t connection_timeout = 500;       // ms
static const uint32_t state_send_root_interval = 10;  // ms

class Synchronizers {
 public:
  enum class Status { ok = 0, waiting, invalid };

  Synchronizers(Application::Store &store, Application::LogEventsSender &log_sender)
      : store_(store),
        state_sender_slow_(state_send_slow_sched, store),
        state_sender_fast_(state_send_fast_sched, store),
        state_sender_root_(state_send_root_sched, child_state_senders_),
        log_events_sender_(log_sender) {}

  Status input(const Application::StateSegment &state_segment);
  void update_clock(uint32_t current_time);
  Status output(Application::StateSegment &state_segment);

  [[nodiscard]] bool connected() const;

 private:
  template <size_t sched_size>
  using SequentialMessageSender = Protocols::Application::
      SequentialStateSender<Application::MessageTypes, Application::StateSegment, sched_size>;
  using ChildStateSenders = Protocols::Application::MappedStateSenders<
      StateSendEntryTypes,
      Application::StateSegment,
      static_cast<size_t>(last_state_send_entry_type) + 1>;
  using RootStateSender = Protocols::Application::SequentialStateSender<
      StateSendEntryTypes,
      Application::StateSegment,
      state_send_root_sched.size()>;

  // State Synchronization (to be moved out to Application class)
  Application::Store &store_;
  SequentialMessageSender<state_send_slow_sched.size()> state_sender_slow_;
  SequentialMessageSender<state_send_fast_sched.size()> state_sender_fast_;
  ChildStateSenders child_state_senders_{
      {StateSendEntryTypes::fast_sched, &state_sender_fast_},
      {StateSendEntryTypes::slow_sched, &state_sender_slow_}};
  RootStateSender state_sender_root_;

  // List Synchronization (to be moved out to Application class)
  Application::LogEventsSender &log_events_sender_;

  // Timing (to be moved out to Application class)
  uint32_t current_time_{};
  Util::MsTimer connection_timer_{connection_timeout};
  Util::MsTimer state_send_timer_{state_send_root_interval};

  void update_list_senders();
};

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
