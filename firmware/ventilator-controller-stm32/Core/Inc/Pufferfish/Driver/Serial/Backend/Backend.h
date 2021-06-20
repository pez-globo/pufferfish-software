/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Frames.h"
#include "Pufferfish/Application/LogEvents.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/HAL/Interfaces/CRCChecker.h"
#include "Pufferfish/Protocols/Application/Lists.h"
#include "Pufferfish/Protocols/Application/States.h"
#include "Pufferfish/Protocols/Transport/CRCElements.h"
#include "Pufferfish/Protocols/Transport/Datagrams.h"
#include "Pufferfish/Protocols/Transport/Messages.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "Pufferfish/Util/Containers/EnumMap.h"
#include "Pufferfish/Util/Enums.h"
#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Driver::Serial::Backend {

using Application::MessageTypes;

// Device Protocols

using MessageDescriptors = Protocols::Transport::
    ProtobufDescriptors<MessageTypes, Application::MessageTypeValues::max() + 1>;

// This relies on a EnumMap constructor which performs dynamic initialization, so it's not safe to
// use in multithreaded contexts. We don't use it in multithreaded contexts, so we can ignore the
// first clang-tidy complaint. As for the second complaint, std::pair is not marked noexcept but
// in practice it won't throw exceptions in this initializer list here.
// NOLINTNEXTLINE(bugprone-dynamic-static-initializers,cert-err58-cpp)
static const MessageDescriptors message_descriptors{
    {MessageTypes::unknown, Util::get_protobuf_desc<Util::UnrecognizedMessage>()},
    {MessageTypes::reserved, Util::get_protobuf_desc<Util::UnrecognizedMessage>()},
    {MessageTypes::sensor_measurements, Util::get_protobuf_desc<SensorMeasurements>()},
    {MessageTypes::cycle_measurements, Util::get_protobuf_desc<CycleMeasurements>()},
    {MessageTypes::parameters, Util::get_protobuf_desc<Parameters>()},
    {MessageTypes::parameters_request, Util::get_protobuf_desc<ParametersRequest>()},
    {MessageTypes::alarm_limits, Util::get_protobuf_desc<AlarmLimits>()},
    {MessageTypes::alarm_limits_request, Util::get_protobuf_desc<AlarmLimitsRequest>()},
    {MessageTypes::expected_log_event, Util::get_protobuf_desc<ExpectedLogEvent>()},
    {MessageTypes::next_log_events, Util::get_protobuf_desc<NextLogEvents>()},
    {MessageTypes::active_log_events, Util::get_protobuf_desc<ActiveLogEvents>()},
    {MessageTypes::alarm_mute, Util::get_protobuf_desc<AlarmMute>()},
    {MessageTypes::alarm_mute_request, Util::get_protobuf_desc<AlarmMuteRequest>()},
    {MessageTypes::mcu_power_status, Util::get_protobuf_desc<MCUPowerStatus>()}};

// State Synchronization

using ReceivableStates = Util::EnumValues<
    MessageTypes,
    MessageTypes::parameters_request,
    MessageTypes::alarm_limits_request,
    MessageTypes::expected_log_event,
    MessageTypes::alarm_mute_request>;

enum class StateSendEntryTypes : uint8_t { fast_sched = 0, slow_sched };
static const StateSendEntryTypes last_state_send_entry_type = StateSendEntryTypes::slow_sched;

static const auto state_send_root_sched = Util::Containers::make_array<StateSendEntryTypes>(
    StateSendEntryTypes::fast_sched,
    StateSendEntryTypes::slow_sched,
    StateSendEntryTypes::slow_sched);
static const auto state_send_fast_sched =
    Util::Containers::make_array<MessageTypes>(MessageTypes::sensor_measurements);
static const auto state_send_slow_sched = Util::Containers::make_array<MessageTypes>(
    MessageTypes::cycle_measurements,
    MessageTypes::parameters,
    MessageTypes::alarm_limits,
    MessageTypes::next_log_events,
    MessageTypes::active_log_events,
    MessageTypes::alarm_mute,
    MessageTypes::mcu_power_status);

static const uint32_t state_send_root_interval = 10;  // ms

// Backend

using CRCElementProps =
    Protocols::Transport::CRCElementProps<Driver::Serial::Backend::FrameProps::payload_max_size>;
using DatagramProps = Protocols::Transport::DatagramProps<CRCElementProps::payload_max_size>;
using Message = Protocols::Transport::Message<
    Application::StateSegment,
    Application::MessageTypeValues,
    DatagramProps::payload_max_size>;

class Receiver {
 public:
  enum class InputStatus { ok = 0, output_ready, invalid_frame_length, input_overwritten };
  enum class OutputStatus {
    available = 0,
    waiting,
    invalid_frame_length,
    invalid_frame_encoding,
    invalid_crcelement_parse,
    invalid_crcelement_crc,
    invalid_datagram_parse,
    invalid_datagram_length,
    invalid_datagram_sequence,
    invalid_message_length,
    invalid_message_type,
    invalid_message_encoding
  };

  explicit Receiver(HAL::Interfaces::CRC32 &crc32c) : crc_(crc32c), message_(message_descriptors) {}

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t new_byte);
  OutputStatus output(Message &output_message);

 private:
  using CRCReceiver = Protocols::Transport::CRCElementReceiver<FrameProps::payload_max_size>;
  using ParsedCRC = Protocols::Transport::ParsedCRCElement<FrameProps::payload_max_size>;
  using DatagramReceiver =
      Protocols::Transport::DatagramReceiver<CRCReceiver::Props::payload_max_size>;
  using ParsedDatagram = Protocols::Transport::ParsedDatagram<CRCReceiver::Props::payload_max_size>;
  using MessageReceiver =
      Protocols::Transport::MessageReceiver<Message, Application::MessageTypeValues::max() + 1>;

  FrameReceiver frame_;
  CRCReceiver crc_;
  DatagramReceiver datagram_;
  MessageReceiver message_;
};

class Sender {
 public:
  enum class Status {
    ok = 0,
    invalid_message_length,
    invalid_message_type,
    invalid_message_encoding,
    invalid_datagram_length,
    invalid_crcelement_length,
    invalid_frame_encoding,
    invalid_frame_length,
    invalid_return_code
  };

  explicit Sender(HAL::Interfaces::CRC32 &crc32c) : message_(message_descriptors), crc_(crc32c) {}

  Status transform(
      const Application::StateSegment &state_segment, FrameProps::ChunkBuffer &output_buffer);

 private:
  using CRCSender = Protocols::Transport::CRCElementSender<FrameProps::payload_max_size>;
  using DatagramSender = Protocols::Transport::DatagramSender<CRCSender::Props::payload_max_size>;
  using MessageSender = Protocols::Transport::
      MessageSender<Message, Application::StateSegment, Application::MessageTypeValues::max() + 1>;

  MessageSender message_;
  DatagramSender datagram_;
  CRCSender crc_;
  FrameSender frame_;
};

static const uint32_t connection_timeout = 500;

// TODO: move application-level logic to Application class
class Backend {
 public:
  enum class Status { ok = 0, waiting, invalid };

  Backend(
      HAL::Interfaces::CRC32 &crc32c,
      Application::Store &store,
      Application::LogEventsSender &sender)
      : receiver_(crc32c),
        sender_(crc32c),
        store_(store),
        state_sender_slow_(state_send_slow_sched, store),
        state_sender_fast_(state_send_fast_sched, store),
        state_sender_root_(state_send_root_sched, child_state_senders_),
        log_events_sender_(sender) {}

  static constexpr bool accept_message(MessageTypes type) noexcept;
  Status input(uint8_t new_byte);
  void update_clock(uint32_t current_time);
  void update_list_senders();
  Status output(FrameProps::ChunkBuffer &output_buffer);

  [[nodiscard]] bool connected() const;

 private:
  template <size_t sched_size>
  using SequentialMessageSender = Protocols::Application::
      SequentialStateSender<MessageTypes, Application::StateSegment, sched_size>;
  using ChildStateSenders = Protocols::Application::MappedStateSenders<
      StateSendEntryTypes,
      Application::StateSegment,
      static_cast<size_t>(last_state_send_entry_type) + 1>;
  using RootStateSender = Protocols::Application::SequentialStateSender<
      StateSendEntryTypes,
      Application::StateSegment,
      state_send_root_sched.size()>;

  Receiver receiver_;
  Sender sender_;

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
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
