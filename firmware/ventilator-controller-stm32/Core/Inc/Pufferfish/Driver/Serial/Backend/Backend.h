/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Datagrams.h"
#include "Frames.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/HAL/CRC.h"
#include "Pufferfish/Protocols/Messages.h"
#include "Pufferfish/Protocols/States.h"
#include "Pufferfish/Util/Array.h"

namespace Pufferfish::Driver::Serial::Backend {

// States

static const auto message_descriptors = Util::make_array<Util::ProtobufDescriptor>(
    // array index should match the type code value
    Util::get_protobuf_descriptor<Util::UnrecognizedMessage>(),  // 0
    Util::get_protobuf_descriptor<Alarms>(),                     // 1
    Util::get_protobuf_descriptor<SensorMeasurements>(),         // 2
    Util::get_protobuf_descriptor<CycleMeasurements>(),          // 3
    Util::get_protobuf_descriptor<Parameters>(),                 // 4
    Util::get_protobuf_descriptor<ParametersRequest>(),          // 5
    Util::get_protobuf_descriptor<Ping>(),                       // 6
    Util::get_protobuf_descriptor<Announcement>()                // 7
);

// State Synchronization

using StateOutputScheduleEntry = Protocols::StateOutputScheduleEntry<Application::MessageTypes>;

static const auto state_sync_schedule = Util::make_array<const StateOutputScheduleEntry>(
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::parameters},
    StateOutputScheduleEntry{10, Application::MessageTypes::alarms},
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::ping},
    StateOutputScheduleEntry{10, Application::MessageTypes::alarms},
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::parameters_request},
    StateOutputScheduleEntry{10, Application::MessageTypes::cycle_measurements});

// Backend

class BackendReceiver {
 public:
  enum class InputStatus { ok = 0, output_ready, invalid_frame_length };
  enum class OutputStatus {
    available = 0,
    waiting,
    invalid_frame_length,
    invalid_datagram_parse,
    invalid_datagram_crc,
    invalid_datagram_length,
    invalid_datagram_sequence,
    invalid_message_length,
    invalid_message_type,
    invalid_message_encoding
  };

  explicit BackendReceiver(HAL::CRC32C &crc32c);

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t new_byte);
  OutputStatus output(Application::Message &output_message);

 private:
  using BackendDatagramReceiver = DatagramReceiver<FrameProps::frame_payload_max_size>;
  using BackendParsedDatagram = ParsedDatagram<FrameProps::frame_payload_max_size>;
  using BackendMessageReceiver =
      Protocols::MessageReceiver<Application::Message, message_descriptors.size()>;

  FrameReceiver frame_;
  BackendDatagramReceiver datagram_;
  BackendMessageReceiver message_;
};

class BackendSender {
 public:
  enum class Status {
    ok = 0,
    invalid_message_length,
    invalid_message_type,
    invalid_message_encoding,
    invalid_datagram_length,
    invalid_frame_length,
    invalid_return_code
  };

  explicit BackendSender(HAL::CRC32C &crc32c);

  Status transform(
      const Application::Message &input_message, FrameProps::ChunkBuffer &output_buffer);

 private:
  using BackendDatagramSender = DatagramSender<FrameProps::frame_payload_max_size>;
  using BackendMessageSender =
      Protocols::MessageSender<Application::Message, message_descriptors.size()>;

  BackendMessageSender message_;
  BackendDatagramSender datagram_;
  FrameSender frame_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
