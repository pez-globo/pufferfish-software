/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Protocols/Application/Events.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "Pufferfish/Util/Enums.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Serial::Backend {

using Application::MessageTypes;

using ReceivableStates = Util::EnumValues<
    MessageTypes,
    MessageTypes::parameters_request,
    MessageTypes::alarm_limits_request,
    MessageTypes::expected_log_event,
    MessageTypes::alarm_mute_request>;

enum class StateSendEntryTypes : uint8_t { realtime_sched = 0, event_sched, main_sched };
static const StateSendEntryTypes last_state_send_entry_type = StateSendEntryTypes::main_sched;

static const auto state_send_root_sched = Util::Containers::make_array<StateSendEntryTypes>(
    StateSendEntryTypes::realtime_sched,
    StateSendEntryTypes::event_sched,
    StateSendEntryTypes::main_sched);
static const auto state_send_realtime_sched =
    Util::Containers::make_array<MessageTypes>(MessageTypes::sensor_measurements);
static const auto state_send_main_sched = Util::Containers::make_array<MessageTypes>(
    MessageTypes::cycle_measurements,
    MessageTypes::parameters,
    MessageTypes::alarm_limits,
    MessageTypes::next_log_events,
    MessageTypes::active_log_events,
    MessageTypes::alarm_mute,
    MessageTypes::mcu_power_status);

static const uint32_t connection_timeout = 500;       // ms
static const uint32_t state_send_root_interval = 10;  // ms

class Synchronizers {
 public:
  enum class Status { ok = 0, waiting, invalid };

  Synchronizers(Application::Store &store, Application::LogEventsSender &log_sender)
      : store_(store),
        state_sender_main_(state_send_main_sched, store),
        event_sender_(state_send_main_sched, store),
        state_sender_realtime_(state_send_realtime_sched, store),
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
  using ChangedEventSender = Protocols::Application::StateChangeEventSender<
      Application::MessageTypes,
      Application::StateSegment,
      state_send_main_sched.size(),
      Application::MessageTypeValues::max() + 1>;
  using ChildStateSenders = Protocols::Application::MappedStateSenders<
      StateSendEntryTypes,
      Application::StateSegment,
      static_cast<size_t>(last_state_send_entry_type) + 1>;
  using RootStateSender = Protocols::Application::SequentialStateSender<
      StateSendEntryTypes,
      Application::StateSegment,
      state_send_root_sched.size()>;

  // State Synchronization
  Application::Store &store_;
  SequentialMessageSender<state_send_main_sched.size()> state_sender_main_;
  ChangedEventSender event_sender_;
  SequentialMessageSender<state_send_realtime_sched.size()> state_sender_realtime_;
  ChildStateSenders child_state_senders_{
      {StateSendEntryTypes::realtime_sched, &state_sender_realtime_},
      {StateSendEntryTypes::event_sched, &event_sender_},
      {StateSendEntryTypes::main_sched, &state_sender_main_}};
  RootStateSender state_sender_root_;

  // List Synchronization
  Application::LogEventsSender &log_events_sender_;

  // Timing & Connection Change Tracking
  uint32_t current_time_{};
  Util::MsTimer state_send_timer_{state_send_root_interval};
  Util::MsTimer connection_timer_{connection_timeout};
  bool prev_backend_connected_ = false;

  void update_list_senders();
  void handle_new_connections(bool backend_connected);
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "States.tpp"
