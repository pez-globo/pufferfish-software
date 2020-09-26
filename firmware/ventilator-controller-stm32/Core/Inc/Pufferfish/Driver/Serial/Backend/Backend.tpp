/*
 * Serial.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_PROTOCOLS_SERIAL_TPP_
#define INC_PUFFERFISH_PROTOCOLS_SERIAL_TPP_

#include "Serial.h"

namespace Pufferfish { namespace Protocols {

// SerialReceiver

SerialReceiver::SerialReceiver(HAL::CRC32C &crc32c) :
    datagram(crc32c),
    message(applicationMessageDescriptors, numApplicationMessageDescriptors) {}

SerialReceiver::InputStatus SerialReceiver::input(uint8_t newByte) {
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

SerialReceiver::OutputStatus SerialReceiver::output(
    ApplicationMessage &outputMessage
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
  case ApplicationMessageReceiver::Status::invalidLength:
    return OutputStatus::invalidMessageLength;
  case ApplicationMessageReceiver::Status::invalidType:
    return OutputStatus::invalidMessageType;
  case ApplicationMessageReceiver::Status::invalidEncoding:
    return OutputStatus::invalidMessageEncoding;
  case ApplicationMessageReceiver::Status::ok:
    break;
  }
  return OutputStatus::available;
}

// SerialSender

SerialSender::SerialSender(HAL::CRC32C &crc32c) :
    datagram(crc32c),
    message(applicationMessageDescriptors, numApplicationMessageDescriptors) {}

SerialSender::Status SerialSender::transform(
    const ApplicationMessage &inputMessage, ChunkBuffer &outputBuffer
) {
  Datagram::PayloadBuffer tempBuffer1;
  ChunkBuffer tempBuffer2;

  // Message
  switch (message.transform(inputMessage, tempBuffer1)) {
  case ApplicationMessageSender::Status::invalidLength:
    return Status::invalidMessageLength;
  case ApplicationMessageSender::Status::invalidType:
    return Status::invalidMessageType;
  case ApplicationMessageSender::Status::invalidEncoding:
    return Status::invalidMessageEncoding;
  case ApplicationMessageSender::Status::ok:
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

} }

#endif /* INC_PUFFERFISH_PROTOCOLS_SERIAL_TPP_ */
