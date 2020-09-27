/*
 * UARTBackend.tpp
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "UART.h"

namespace Pufferfish::Driver::Serial::Backend {

// UARTBackendReceiver

template<class BufferedUART>
UARTBackendReceiver<BufferedUART>::UARTBackendReceiver(
      volatile BufferedUART &uart, BackendReceiver &serial
) : uart(uart), serial(serial) {}

template<class BufferedUART>
typename UARTBackendReceiver<BufferedUART>::Status
UARTBackendReceiver<BufferedUART>::output(
    Application::Message &outputMessage
) {
  input();
  switch (serial.output(outputMessage)) {
  case BackendReceiver::OutputStatus::waiting:
    return Status::waiting;
  case BackendReceiver::OutputStatus::invalidFrameChunkLength:
  case BackendReceiver::OutputStatus::invalidFrameCOBSLength:
    // TODO: handle error cases first
    return Status::invalid;
  case BackendReceiver::OutputStatus::invalidDatagramParse:
  case BackendReceiver::OutputStatus::invalidDatagramCRC:
  case BackendReceiver::OutputStatus::invalidDatagramLength:
    // TODO: handle error cases first
    return Status::invalid;
  case BackendReceiver::OutputStatus::invalidDatagramSequence:
    // TODO: handle warning case first
    break;
  case BackendReceiver::OutputStatus::invalidMessageLength:
  case BackendReceiver::OutputStatus::invalidMessageType:
  case BackendReceiver::OutputStatus::invalidMessageEncoding:
    // TODO: handle error cases first
    return Status::invalid;
  case BackendReceiver::OutputStatus::available:
    break;
  }
  return Status::available;
}

template<class BufferedUART>
void UARTBackendReceiver<BufferedUART>::input() {
  while (true) { // repeat until UART read buffer is empty or output is available
    uint8_t receive = 0;

    // UART
    switch (uart.read(receive)) {
    case BufferStatus::empty:
      return;
    case BufferStatus::ok:
      break;
    default:
      break;
    }

    // BackendReceiver
    switch (serial.input(receive)) {
    case BackendReceiver::InputStatus::outputReady:
      return;
    case BackendReceiver::InputStatus::invalidFrameChunkLength:
      // TODO: handle error case first
      return;
    case BackendReceiver::InputStatus::inputReady:
      break;
    }
  }
}

// UARTBackendSender

template<class BufferedUART>
UARTBackendSender<BufferedUART>::UARTBackendSender(
      volatile BufferedUART &uart, BackendSender &serial
) : uart(uart), serial(serial) {}

template<class BufferedUART>
typename UARTBackendSender<BufferedUART>::Status
UARTBackendSender<BufferedUART>::input(
    const Application::Message &inputMessage
) {
  ChunkBuffer sendOutput;
  switch (serial.transform(inputMessage, sendOutput)) {
  case BackendSender::Status::invalidMessageLength:
  case BackendSender::Status::invalidMessageType:
  case BackendSender::Status::invalidMessageEncoding:
  case BackendSender::Status::invalidDatagramLength:
  case BackendSender::Status::invalidFrameCOBSLength:
  case BackendSender::Status::invalidFrameChunkLength:
    // TODO: handle error cases first
    return Status::invalid;
  case BackendSender::Status::ok: // ready to write to UART
    break;
  }
  // blocks until everything is written to TX buffer
  for (size_t i = 0; i < sendOutput.size(); ++i) {
    uart.write_block(sendOutput.buffer[i], 100000);
  }
  return Status::ok;
}

// UARTBackendDriver

template<class BufferedUART>
UARTBackendDriver<BufferedUART>::UARTBackendDriver(
    volatile BufferedUART &uart, HAL::CRC32C &crc32c
) :
    uart(uart), crc32c(crc32c),
    receiverProtocol(crc32c), senderProtocol(crc32c),
    receiver(uart, receiverProtocol), sender(uart, senderProtocol) {}

template<class BufferedUART>
void UARTBackendDriver<BufferedUART>::setup_irq() {
  uart.setup_irq();
}

template<class BufferedUART>
typename UARTBackendDriver<BufferedUART>::Receiver::Status
UARTBackendDriver<BufferedUART>::receive(
    Application::Message &receiveMessage
) {
  return receiver.output(receiveMessage);
}

template<class BufferedUART>
typename UARTBackendDriver<BufferedUART>::Sender::Status
UARTBackendDriver<BufferedUART>::send(
    const Application::Message &sendMessage
) {
  return sender.input(sendMessage);
}

// UARTBackend

UARTBackend::UARTBackend(
    volatile HAL::LargeBufferedUART &uart, HAL::CRC32C &crc32c,
    Application::States &states
) : driver(uart, crc32c), states(states), synchronizer(states, stateSyncSchedule) {}

void UARTBackend::setup_irq() {
  driver.setup_irq();
}

void UARTBackend::receive() {
  Application::Message message;
  switch (driver.receive(message)) {
  case Driver::Receiver::Status::waiting: // nothing to do
  case Driver::Receiver::Status::invalid: // errors handled by driver
    break;
  case Driver::Receiver::Status::available:
    switch (synchronizer.input(message)) {
    case StateSynchronizer::InputStatus::invalidType:
      // TODO: handle error case
      break;
    case StateSynchronizer::InputStatus::ok:
      break;
    }
    default:
      break;
  }
}

void UARTBackend::update_clock(uint32_t currentTime) {
  synchronizer.input(currentTime);
}

void UARTBackend::send() {
  Application::Message message;
  switch (synchronizer.output(message)) {
  case StateSynchronizer::OutputStatus::waiting:
    break;
  case StateSynchronizer::OutputStatus::available:
    switch (driver.send(message)) {
    case Driver::Sender::Status::invalid: // errors handled by driver
      break;
    case Driver::Sender::Status::ok:
      break;
    }
    break;
  }
}

}
