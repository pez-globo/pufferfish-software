/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <stdint.h>
#include "Pufferfish/Util/ByteArray.h"
#include "Pufferfish/HAL/CRC.h"
#include "Frames.h"
#include "Datagrams.h"
#include "Messages.h"
#include "Pufferfish/Application/States.h"
#include "Pufferfish/Protocols/States.h"

namespace Pufferfish::Driver::Serial::Backend {

// States

using ProtobufDescriptors = Util::ProtobufDescriptors<8>;

static const ProtobufDescriptors message_descriptors = {
    // array index should match the type code value
    Util::getProtobufDescriptor<Util::UnrecognizedMessage>(), // 0
    Util::getProtobufDescriptor<Alarms>(), // 1
    Util::getProtobufDescriptor<SensorMeasurements>(), // 2
    Util::getProtobufDescriptor<CycleMeasurements>(), // 3
    Util::getProtobufDescriptor<Parameters>(), // 4
    Util::getProtobufDescriptor<ParametersRequest>(), // 5
    Util::getProtobufDescriptor<Ping>(), // 6
    Util::getProtobufDescriptor<Announcement>() // 7
};

// State Synchronization

using StateOutputScheduleEntry = Protocols::StateOutputScheduleEntry<Application::MessageTypes>;

using StateOutputSchedule = Protocols::StateOutputSchedule<Application::MessageTypes, 9>;

static const StateOutputSchedule stateSyncSchedule = {{
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
  enum class InputStatus {
    inputReady = 0, outputReady, invalidFrameChunkLength
  };
  enum class OutputStatus {
    available = 0, waiting,
    invalidFrameChunkLength, invalidFrameCOBSLength,
    invalidDatagramParse, invalidDatagramCRC, invalidDatagramLength,
    invalidDatagramSequence,
    invalidMessageLength, invalidMessageType, invalidMessageEncoding
  };

  BackendReceiver(HAL::CRC32C &crc32c);

  // Call this until it returns outputReady, then call output
  InputStatus input(uint8_t newByte);
  OutputStatus output(Application::Message &outputMessage);

protected:
  using BackendMessageReceiver = MessageReceiver<
      Application::UnionMessage, std::tuple_size<ProtobufDescriptors>::value>;

  FrameReceiver frame;
  DatagramReceiver datagram;
  BackendMessageReceiver message;
};

class BackendSender {
public:
  enum class Status {
    ok = 0,
    invalidMessageLength, invalidMessageType, invalidMessageEncoding,
    invalidDatagramLength,
    invalidFrameCOBSLength, invalidFrameChunkLength
  };

  BackendSender(HAL::CRC32C &crc32c);

  Status transform(
      const Application::Message &inputMessage, ChunkBuffer &outputBuffer
  );

protected:
  using BackendMessageSender = MessageSender<
      Application::UnionMessage, std::tuple_size<ProtobufDescriptors>::value>;

  BackendMessageSender message;
  DatagramSender datagram;
  FrameSender frame;
};

}

#include "Backend.tpp"
