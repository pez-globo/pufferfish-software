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

namespace Pufferfish::Driver::Serial::Backend {

// States

using MessageTypes = Application::MessageTypes;
using MessageDescriptors = Protocols::Transport::
    ProtobufDescriptors<Application::MessageTypes, Application::MessageTypeValues::max()>;

// This relies on a EnumMap constructor which performs dynamic initialization, so it's not safe to
// use in multithreaded contexts. We don't use it in multithreaded contexts, so we can ignore this
// clang-tidy complaint.
// NOLINTNEXTLINE(bugprone-dynamic-static-initializers)
static const MessageDescriptors message_descriptors{
    {MessageTypes::unknown, Util::get_protobuf_desc<Util::UnrecognizedMessage>()},
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
    {MessageTypes::alarm_mute_request, Util::get_protobuf_desc<AlarmMuteRequest>()}};

// State Synchronization

using StateOutputScheduleEntry =
    Protocols::Application::StateOutputScheduleEntry<Application::MessageTypes>;

static const auto state_sync_schedule =
    Util::Containers::make_array<const StateOutputScheduleEntry>(
        StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
        StateOutputScheduleEntry{10, Application::MessageTypes::parameters},
        StateOutputScheduleEntry{10, Application::MessageTypes::parameters_request},
        StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
        StateOutputScheduleEntry{10, Application::MessageTypes::alarm_limits},
        StateOutputScheduleEntry{10, Application::MessageTypes::alarm_limits_request},
        StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
        StateOutputScheduleEntry{10, Application::MessageTypes::next_log_events},
        StateOutputScheduleEntry{10, Application::MessageTypes::active_log_events},
        StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
        StateOutputScheduleEntry{10, Application::MessageTypes::alarm_mute},
        StateOutputScheduleEntry{10, Application::MessageTypes::alarm_mute_request},
        StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
        StateOutputScheduleEntry{10, Application::MessageTypes::cycle_measurements});

// Backend
using CRCElementProps =
    Protocols::Transport::CRCElementProps<Driver::Serial::Backend::FrameProps::payload_max_size>;
using DatagramProps = Protocols::Transport::DatagramProps<CRCElementProps::payload_max_size>;
using Message = Protocols::Transport::Message<
    Application::StateSegment,
    Application::MessageTypeValues,
    DatagramProps::payload_max_size>;

using InputStates = Util::EnumValues<
    Application::MessageTypes,
    Application::MessageTypes::parameters_request,
    Application::MessageTypes::alarm_limits_request,
    Application::MessageTypes::alarm_mute_request,
    Application::MessageTypes::expected_log_event>;

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
      Protocols::Transport::MessageReceiver<Message, Application::MessageTypeValues::max()>;

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
      MessageSender<Message, Application::StateSegment, Application::MessageTypeValues::max()>;

  MessageSender message_;
  DatagramSender datagram_;
  CRCSender crc_;
  FrameSender frame_;
};

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
        synchronizer_(store, state_sync_schedule),
        log_events_sender_(sender) {}

  static constexpr bool accept_message(Application::MessageTypes type) noexcept;
  Status input(uint8_t new_byte);
  void update_clock(uint32_t current_time);
  void update_list_senders();
  Status output(FrameProps::ChunkBuffer &output_buffer);

 private:
  using StateSynchronizer = Protocols::Application::StateSynchronizer<
      Application::Store,
      Application::StateSegment,
      Application::MessageTypes,
      state_sync_schedule.size()>;

  Receiver receiver_;
  Sender sender_;
  Application::Store &store_;
  StateSynchronizer synchronizer_;
  Application::LogEventsSender &log_events_sender_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
