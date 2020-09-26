/*
 * UARTSerial.tpp
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_UARTSERIAL_TPP_
#define INC_PUFFERFISH_UARTSERIAL_TPP_

#include "Pufferfish/UARTSerial.h"

namespace Pufferfish {

// UARTSerialReceiver

template<class BufferedUART>
UARTSerialReceiver<BufferedUART>::UARTSerialReceiver(
      volatile BufferedUART &uart, Protocols::SerialReceiver &serial
) : uart(uart), serial(serial) {}

template<class BufferedUART>
typename UARTSerialReceiver<BufferedUART>::Status
UARTSerialReceiver<BufferedUART>::output(
    Protocols::ApplicationMessage &outputMessage
) {
  input();
  switch (serial.output(outputMessage)) {
  case Protocols::SerialReceiver::OutputStatus::waiting:
    return Status::waiting;
  case Protocols::SerialReceiver::OutputStatus::invalidFrameChunkLength:
  case Protocols::SerialReceiver::OutputStatus::invalidFrameCOBSLength:
    // TODO: handle error cases first
    return Status::invalid;
  case Protocols::SerialReceiver::OutputStatus::invalidDatagramParse:
  case Protocols::SerialReceiver::OutputStatus::invalidDatagramCRC:
  case Protocols::SerialReceiver::OutputStatus::invalidDatagramLength:
    // TODO: handle error cases first
    return Status::invalid;
  case Protocols::SerialReceiver::OutputStatus::invalidDatagramSequence:
    // TODO: handle warning case first
    break;
  case Protocols::SerialReceiver::OutputStatus::invalidMessageLength:
  case Protocols::SerialReceiver::OutputStatus::invalidMessageType:
  case Protocols::SerialReceiver::OutputStatus::invalidMessageEncoding:
    // TODO: handle error cases first
    return Status::invalid;
  case Protocols::SerialReceiver::OutputStatus::available:
    break;
  }
  return Status::available;
}

template<class BufferedUART>
void UARTSerialReceiver<BufferedUART>::input() {
  while (true) { // repeat until UART read buffer is empty or output is available
    uint8_t receive = 0;

    // UART
    switch (uart.read(receive)) {
    case BufferReadStatus::empty:
      return;
    case BufferReadStatus::ok:
      break;
    }

    // SerialReceiver
    switch (serial.input(receive)) {
    case Protocols::SerialReceiver::InputStatus::outputReady:
      return;
    case Protocols::SerialReceiver::InputStatus::invalidFrameChunkLength:
      // TODO: handle error case first
      return;
    case Protocols::SerialReceiver::InputStatus::inputReady:
      break;
    }
  }
}

// UARTSerialSender

template<class BufferedUART>
UARTSerialSender<BufferedUART>::UARTSerialSender(
      volatile BufferedUART &uart, Protocols::SerialSender &serial
) : uart(uart), serial(serial) {}

template<class BufferedUART>
typename UARTSerialSender<BufferedUART>::Status
UARTSerialSender<BufferedUART>::input(
    const Protocols::ApplicationMessage &inputMessage
) {
  Protocols::ChunkBuffer sendOutput;
  switch (serial.transform(inputMessage, sendOutput)) {
  case Protocols::SerialSender::Status::invalidMessageLength:
  case Protocols::SerialSender::Status::invalidMessageType:
  case Protocols::SerialSender::Status::invalidMessageEncoding:
  case Protocols::SerialSender::Status::invalidDatagramLength:
  case Protocols::SerialSender::Status::invalidFrameCOBSLength:
  case Protocols::SerialSender::Status::invalidFrameChunkLength:
    // TODO: handle error cases first
    return Status::invalid;
  case Protocols::SerialSender::Status::ok: // ready to write to UART
    break;
  }
  // blocks until everything is written to TX buffer
  uart.writeBlock(sendOutput.buffer, sendOutput.size()); // returns 0 when called without timeout
  return Status::ok;
}

// UARTSerialDriver

template<class BufferedUART>
UARTSerialDriver<BufferedUART>::UARTSerialDriver(
    volatile BufferedUART &uart, HAL::CRC32C &crc32c
) :
    uart(uart), crc32c(crc32c),
    receiverProtocol(crc32c), senderProtocol(crc32c),
    receiver(uart, receiverProtocol), sender(uart, senderProtocol) {}

template<class BufferedUART>
void UARTSerialDriver<BufferedUART>::setupIRQ() {
  uart.setupIRQ();
}

template<class BufferedUART>
typename UARTSerialDriver<BufferedUART>::Receiver::Status
UARTSerialDriver<BufferedUART>::receive(
    Protocols::ApplicationMessage &receiveMessage
) {
  return receiver.output(receiveMessage);
}

template<class BufferedUART>
typename UARTSerialDriver<BufferedUART>::Sender::Status
UARTSerialDriver<BufferedUART>::send(
    const Protocols::ApplicationMessage &sendMessage
) {
  return sender.input(sendMessage);
}

// UARTStateSynchronizer

UARTStateSynchronizer::UARTStateSynchronizer(
    volatile HAL::LargeBufferedUART &uart, HAL::CRC32C &crc32c,
    Protocols::ApplicationStates &states
) : driver(uart, crc32c), states(states), synchronizer(states) {}

void UARTStateSynchronizer::setupIRQ() {
  driver.setupIRQ();
}

void UARTStateSynchronizer::receive() {
  Protocols::ApplicationMessage message;
  switch (driver.receive(message)) {
  case Driver::Receiver::Status::waiting: // nothing to do
  case Driver::Receiver::Status::invalid: // errors handled by driver
    break;
  case Driver::Receiver::Status::available:
    switch (synchronizer.input(message)) {
    case Protocols::StateSynchronizer::InputStatus::invalidType:
      // TODO: handle error case
      break;
    case Protocols::StateSynchronizer::InputStatus::ok:
      break;
    }
  }
}

void UARTStateSynchronizer::updateClock(uint32_t currentTime) {
  synchronizer.input(currentTime);
}

void UARTStateSynchronizer::send() {
  Protocols::ApplicationMessage message;
  switch (synchronizer.output(message)) {
  case Protocols::StateSynchronizer::OutputStatus::waiting:
    break;
  case Protocols::StateSynchronizer::OutputStatus::available:
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

#endif /* INC_PUFFERFISH_UARTSERIAL_TPP_ */
