/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Backend.h"

namespace Pufferfish::Driver::Serial::Backend {

// BackendReceiver

BackendReceiver::BackendReceiver(HAL::CRC32C &crc32c) :
    datagram(crc32c),
    message(Application::message_descriptors) {}

BackendReceiver::InputStatus BackendReceiver::input(uint8_t newByte) {
  switch (frame.input(newByte)) {
  case FrameReceiver::InputStatus::outputReady:
    return InputStatus::outputReady;
  case FrameReceiver::InputStatus::invalidChunkLength:
    return InputStatus::invalidFrameChunkLength;
  case FrameReceiver::InputStatus::inputReady:
    break;
  }
  return InputStatus::inputReady;
}

BackendReceiver::OutputStatus BackendReceiver::output(
    Application::Message &outputMessage
) {
  ChunkBuffer tempBuffer1;
  Datagram::PayloadBuffer tempBuffer2;

  // Frame
  switch (frame.output(tempBuffer1)) {
  case FrameReceiver::OutputStatus::waiting:
    return OutputStatus::waiting;
  case FrameReceiver::OutputStatus::invalidChunkLength:
    return OutputStatus::invalidFrameChunkLength;
  case FrameReceiver::OutputStatus::invalidCOBSLength:
    return OutputStatus::invalidFrameCOBSLength;
  case FrameReceiver::OutputStatus::available:
    break;
  }

  // Datagram
  Datagram receiveDatagram(tempBuffer2);
  switch (datagram.transform(tempBuffer1, receiveDatagram)) {
  case DatagramReceiver::Status::invalidParse:
    return OutputStatus::invalidDatagramParse;
  case DatagramReceiver::Status::invalidCRC:
    return OutputStatus::invalidDatagramCRC;
  case DatagramReceiver::Status::invalidLength:
    return OutputStatus::invalidDatagramLength;
  case DatagramReceiver::Status::invalidSequence:
    // TODO: emit a warning about invalid sequence
  case DatagramReceiver::Status::ok:
    break;
  }

  // Message
  switch (message.transform(tempBuffer2, outputMessage)) {
  case Application::MessageReceiver::Status::invalidLength:
    return OutputStatus::invalidMessageLength;
  case Application::MessageReceiver::Status::invalidType:
    return OutputStatus::invalidMessageType;
  case Application::MessageReceiver::Status::invalidEncoding:
    return OutputStatus::invalidMessageEncoding;
  case Application::MessageReceiver::Status::ok:
    break;
  }
  return OutputStatus::available;
}

// BackendSender

BackendSender::BackendSender(HAL::CRC32C &crc32c) :
    message(Application::message_descriptors),
    datagram(crc32c) {}

BackendSender::Status BackendSender::transform(
    const Application::Message &inputMessage, ChunkBuffer &outputBuffer
) {
  Datagram::PayloadBuffer tempBuffer1;
  ChunkBuffer tempBuffer2;

  // Message
  switch (message.transform(inputMessage, tempBuffer1)) {
  case Application::MessageSender::Status::invalidLength:
    return Status::invalidMessageLength;
  case Application::MessageSender::Status::invalidType:
    return Status::invalidMessageType;
  case Application::MessageSender::Status::invalidEncoding:
    return Status::invalidMessageEncoding;
  case Application::MessageSender::Status::ok:
    break;
  }

  // Datagram
  switch (datagram.transform(tempBuffer1, tempBuffer2)) {
  case DatagramSender::Status::invalidLength:
    return Status::invalidDatagramLength;
  case DatagramSender::Status::ok:
    break;
  }

  // Frame
  switch (frame.transform(tempBuffer2, outputBuffer)) {
  case FrameSender::Status::invalidCOBSLength:
    return Status::invalidFrameCOBSLength;
  case FrameSender::Status::invalidChunkLength:
    return Status::invalidFrameChunkLength;
  case FrameSender::Status::ok:
    break;
  }
  return Status::ok;
}

}
