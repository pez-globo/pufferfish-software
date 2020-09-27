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
#include "Pufferfish/Util/ByteArray.h"

namespace Pufferfish::Driver::Serial::Backend {

// States

using ProtobufDescriptors = Util::ProtobufDescriptors<8>;

static const ProtobufDescriptors message_descriptors = {
    // array index should match the type code value
    Util::get_protobuf_descriptor<Util::UnrecognizedMessage>(),  // 0
    Util::get_protobuf_descriptor<Alarms>(),                     // 1
    Util::get_protobuf_descriptor<SensorMeasurements>(),         // 2
    Util::get_protobuf_descriptor<CycleMeasurements>(),          // 3
    Util::get_protobuf_descriptor<Parameters>(),                 // 4
    Util::get_protobuf_descriptor<ParametersRequest>(),          // 5
    Util::get_protobuf_descriptor<Ping>(),                       // 6
    Util::get_protobuf_descriptor<Announcement>()                // 7
};

// State Synchronization

using StateOutputScheduleEntry = Protocols::StateOutputScheduleEntry<Application::MessageTypes>;

using StateOutputSchedule = Protocols::StateOutputSchedule<Application::MessageTypes, 9>;

static const StateOutputSchedule state_sync_schedule = {{
    {10, Application::MessageTypes::sensor_measurements},
    {10, Application::MessageTypes::parameters},
    {10, Application::MessageTypes::alarms},
    {10, Application::MessageTypes::sensor_measurements},
    {10, Application::MessageTypes::ping},
    {10, Application::MessageTypes::alarms},
    {10, Application::MessageTypes::sensor_measurements},
    {10, Application::MessageTypes::parameters_request},
    {10, Application::MessageTypes::cycle_measurements},
}};

// Backend

class BackendReceiver {
 public:
  enum class InputStatus { input_ready = 0, output_ready, invalid_frame_chunk_length };
  enum class OutputStatus {
    available = 0,
    waiting,
    invalid_frame_chunk_length,
    invalid_frame_cobs_length,
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

 protected:
  using BackendMessageReceiver =
      Protocols::MessageReceiver<Application::Message, std::tuple_size<ProtobufDescriptors>::value>;

  FrameReceiver frame_;
  DatagramReceiver datagram_;
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
    invalid_frame_cobs_length,
    invalid_frame_chunk_length
  };

  explicit BackendSender(HAL::CRC32C &crc32c);

  Status transform(const Application::Message &input_message, ChunkBuffer &output_buffer);

 protected:
  using BackendMessageSender =
      Protocols::MessageSender<Application::Message, std::tuple_size<ProtobufDescriptors>::value>;

  BackendMessageSender message_;
  DatagramSender datagram_;
  FrameSender frame_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Backend.tpp"
