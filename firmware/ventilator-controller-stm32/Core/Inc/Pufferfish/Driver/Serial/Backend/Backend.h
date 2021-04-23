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
#include "Pufferfish/Protocols/CRCElements.h"
#include "Pufferfish/Protocols/Datagrams.h"
#include "Pufferfish/Protocols/Lists.h"
#include "Pufferfish/Protocols/Messages.h"
#include "Pufferfish/Protocols/States.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/Enums.h"

namespace Pufferfish::Driver::Serial::Backend {

// States

static const auto message_descriptors = Util::make_array<Util::ProtobufDescriptor>(
    // array index should match the type code value
    Util::get_protobuf_descriptor<Util::UnrecognizedMessage>(),  // 0
    Util::get_protobuf_descriptor<Util::UnrecognizedMessage>(),  // 1
    Util::get_protobuf_descriptor<SensorMeasurements>(),         // 2
    Util::get_protobuf_descriptor<CycleMeasurements>(),          // 3
    Util::get_protobuf_descriptor<Parameters>(),                 // 4
    Util::get_protobuf_descriptor<ParametersRequest>(),          // 5
    Util::get_protobuf_descriptor<AlarmLimits>(),                // 6
    Util::get_protobuf_descriptor<AlarmLimitsRequest>(),         // 7
    Util::get_protobuf_descriptor<ExpectedLogEvent>(),           // 8
    Util::get_protobuf_descriptor<NextLogEvents>(),              // 9
    Util::get_protobuf_descriptor<ActiveLogEvents>()             // 10
);

// State Synchronization

using StateOutputScheduleEntry = Protocols::StateOutputScheduleEntry<Application::MessageTypes>;

static const auto state_sync_schedule = Util::make_array<const StateOutputScheduleEntry>(
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
    StateOutputScheduleEntry{10, Application::MessageTypes::cycle_measurements});

// Backend
using CRCElementProps =
    Protocols::CRCElementProps<Driver::Serial::Backend::FrameProps::payload_max_size>;
using DatagramProps = Protocols::DatagramProps<CRCElementProps::payload_max_size>;
using Message = Protocols::Message<
    Application::StateSegment,
    Application::MessageTypeValues,
    DatagramProps::payload_max_size>;

using InputStates = Util::EnumValues<
    Application::MessageTypes,
    Application::MessageTypes::parameters_request,
    Application::MessageTypes::alarm_limits_request,
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

  explicit Receiver(HAL::CRC32 &crc32c) : crc_(crc32c), message_(message_descriptors) {}

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t new_byte);
  OutputStatus output(Message &output_message);

 private:
  using CRCReceiver = Protocols::CRCElementReceiver<FrameProps::payload_max_size>;
  using ParsedCRC = Protocols::ParsedCRCElement<FrameProps::payload_max_size>;
  using DatagramReceiver =
      Protocols::DatagramReceiver<CRCReceiver::Props::payload_max_size>;
  using ParsedDatagram =
      Protocols::ParsedDatagram<CRCReceiver::Props::payload_max_size>;
  using MessageReceiver = Protocols::MessageReceiver<Message, message_descriptors.size()>;

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

  explicit Sender(HAL::CRC32 &crc32c) : message_(message_descriptors), crc_(crc32c) {}

  Status transform(
      const Application::StateSegment &state_segment, FrameProps::ChunkBuffer &output_buffer);

 private:
  using CRCSender = Protocols::CRCElementSender<FrameProps::payload_max_size>;
  using DatagramSender =
      Protocols::DatagramSender<CRCSender::Props::payload_max_size>;
  using MessageSender =
      Protocols::MessageSender<Message, Application::StateSegment, message_descriptors.size()>;

  MessageSender message_;
  DatagramSender datagram_;
  CRCSender crc_;
  FrameSender frame_;
};

class Backend {
 public:
  enum class Status { ok = 0, waiting, invalid };

  Backend(HAL::CRC32 &crc32c, Application::States &states, Application::LogEventsSender &sender)
      : receiver_(crc32c),
        sender_(crc32c),
        states_(states),
        synchronizer_(states, state_sync_schedule),
        log_events_sender_(sender) {}

  static constexpr bool accept_message(Application::MessageTypes type) noexcept;
  Status input(uint8_t new_byte);
  void update_clock(uint32_t current_time);
  void update_list_senders();
  Status output(FrameProps::ChunkBuffer &output_buffer);

 private:
  using StateSynchronizer = Protocols::StateSynchronizer<
      Application::States,
      Application::StateSegment,
      Application::MessageTypes,
      state_sync_schedule.size()>;

  Receiver receiver_;
  Sender sender_;
  Application::States &states_;
  StateSynchronizer synchronizer_;
  Application::LogEventsSender &log_events_sender_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
