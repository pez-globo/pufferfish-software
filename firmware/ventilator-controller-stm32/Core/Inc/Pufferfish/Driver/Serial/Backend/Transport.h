/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Frames.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/HAL/Interfaces/CRCChecker.h"
#include "Pufferfish/Protocols/Transport/CRCElements.h"
#include "Pufferfish/Protocols/Transport/Datagrams.h"
#include "Pufferfish/Protocols/Transport/Messages.h"

namespace Pufferfish::Driver::Serial::Backend {

using Application::MessageTypes;

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
    // Measurements
    {MessageTypes::sensor_measurements, Util::get_protobuf_desc<Application::SensorMeasurements>()},
    {MessageTypes::cycle_measurements, Util::get_protobuf_desc<Application::CycleMeasurements>()},
    // Parameters
    {MessageTypes::parameters, Util::get_protobuf_desc<Application::Parameters>()},
    {MessageTypes::parameters_request, Util::get_protobuf_desc<Application::ParametersRequest>()},
    // Alarm Limits
    {MessageTypes::alarm_limits, Util::get_protobuf_desc<Application::AlarmLimits>()},
    {MessageTypes::alarm_limits_request,
     Util::get_protobuf_desc<Application::AlarmLimitsRequest>()},
    // Log Events
    {MessageTypes::expected_log_event, Util::get_protobuf_desc<Application::ExpectedLogEvent>()},
    {MessageTypes::next_log_events, Util::get_protobuf_desc<Application::NextLogEvents>()},
    {MessageTypes::active_log_events, Util::get_protobuf_desc<Application::ActiveLogEvents>()},
    // Alarm Muting
    {MessageTypes::alarm_mute, Util::get_protobuf_desc<Application::AlarmMute>()},
    {MessageTypes::alarm_mute_request, Util::get_protobuf_desc<Application::AlarmMuteRequest>()},
    // Screen Status
    {MessageTypes::screen_status, Util::get_protobuf_desc<Application::ScreenStatus>()},
    {MessageTypes::screen_status_request,
     Util::get_protobuf_desc<Application::ScreenStatusRequest>()},
    // System Miscellaneous
    {MessageTypes::mcu_power_status, Util::get_protobuf_desc<Application::MCUPowerStatus>()},
    {MessageTypes::backend_connections,
     Util::get_protobuf_desc<Application::BackendConnections>()}};

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

}  // namespace Pufferfish::Driver::Serial::Backend

#include "Transport.tpp"
